/* 
 * File:   vcrs_state_machine.c
 * This file includes the control logic of the VCRS.
 * The logic is encapsulated with the state machine.
 * Functionality either not implemented are commented as "ToDo"
 * with a suggestion of further development. For testing purposes
 * dummy variables are used as placeholders.
 * 
 * Note: As it was discovered that the SMPS temperature sensor
 * output voltage reduces (increases) as the temperature increases (reduces)
 * The ADC reading becomes about 500-510 in room temperature, when the
 * SMPS temperature sensor is applied. Note, this sensor is does not have
 * a linear output.
 * Several relational operators are opposite of the logic presented in the
 * ASM chart in Chapter 5 (Embedded System Developement) in the report.
 * 
 * In addition, in the SAFETY_CHECK state, an additional function
 * call ("smps_control_zero()") is placed to ensure that the SMPS is 
 * ramped down slowly if an error occurs.
 * 
*/
#include <stdbool.h>    /* needed for boolean variables --> see XC32_compiler_user_guide 
                        * p.273 (DS50002799A-page 273), Note compiler needs to be in compatability mode.*/
#include <limits.h>     /*defines the ranges of values which can be represented by
                        * the integer types XC32_user_guide p.136*/
#include "pwm.h"
#include "adc.h"
#include <xc.h>

/*********************VARIABLES***************************/
unsigned int start_time = 0; //holds start time - so system knows when to send info to cloud. ToDo: use RTCC instead with indefinite repeat --> see datasheet ch.21 and reference Maunual p.886 - ch.29
unsigned int current_time = 0; // dummy variable for testing -->ToDo: use RTCC instead
const short TIME_INTERVAL_BETWEEN_SENDING = 1000; // dummy variable - the time interval between each transmission of data to cloud --> ToDo: use RTCC instead

short dynamo_temp = 0; //holds current value of dynamo temperature reading
short smps_temp = 0; // holds current/last reading the SMPS temperature. Note, SMPS temperature sensor: closer to 0 implies higher temperature.
short mcu_temp = 500; // holds current/last MCU temperature sensor reading



/**************************************************************************************/
const short MAX_DYNAMO_TEMP = 200; //Upper bound dynamo temperature sensor reader. Room temperature is about 500-510 in ADC reading ToDo: value have to be set dependent on dynamo sensor applied 
const short MIN_DYNAMO_TEMP = 800; //lower bound dynamo temperature sensor reader. ToDo: value have to be set dependent on dynamo sensor installed 

const short MAX_SMPS_TEMP = 200; //upper bound dynamo temperature sensor reader.ToDo: SMPS temperature sensor is not linear. Research temperature reading to celsius conversion.
const short MIN_SMPS_TEMP = 800; //lower bound dynamo temperature sensor reader. ToDo: SMPS temperature sensor is not linear. Research temperature reading to celsius conversion.

const short START_FAN_SMPS_TEMP = 400; // at this temperature the fan shall start. 
const short START_FAN_MCU_TEMP = 400;  // at this temperature the fan shall start. ToDo: MCU temperature hardware sensor not implemented

const double MAX_CURRENT = 25; // Assume 10-bit resolution of analog input --> 1023 = max current the BMS (shunt) can request (provide)
const double MIN_CURRENT = 0; // Assume 10-bit resolution of analog input --> 0 = min 0V current the BMS (shunt) can request (provide)

const short SEND_ERROR_MSG = 5; // numbers of error needed in row to send an error message to the cloud
const short SAFETY_APPROVED = 2; // number of safety checks approved in row to move to the temperature state


bool smps_enable = 0; // ToDo: hardware not in place.
unsigned char current_state = 0; // holds the current state of the state machine
unsigned short safe_cnt = 0;    // count number of successful safety checks after one error has occurred in row, reset when an error occurs
unsigned short error_cnt = 0;   // count number of sequential error/faulty readings detected in row, reset when a successful safety check occurs


double bms_control_ref = 0;  // holds BMS control reference (current request)
double shunt_current = 0; // holds the last reading of the shunt current
float shunt_bms_compare = 0; // holds the difference between bms_control_ref and shunt_bms_compare, Floating-point data types, XC32_user_guide p.138

unsigned char error_ID = 0; // holds the current error ID message, 0 = no error


/***************************Enumerations*****************************************/

enum{   // fan_control
    OFF,
    ON
};

enum{   // smps_enable signal
    NOT_CHARGE,
    CHARGE
};

enum {  // States:
    INITIALIZE,     // start state when powering up/after power loss
    TEMPERATURE,    // state checking dynamo, SMPS and MCU temperatures and controlling fans
    MEASURE,        // reading current measurement by shunt and current requested by BMS
    CHARGING,       // set the charging current that the SMPSs provides to the BMS
    SAFETY_CHECK,   // performs a safety check, to ensure all temperature are within range
    ERROR_MSG,      // send error message if system error occurs x number of time.
    READ_BMS,       // read the data from the BMS over CAN bus
    SEND_TO_CLOUD   // transmits the data to the cloud service
};


/*************************VCRS State Machine***********************************/

void run_vcrs(void){
    static unsigned short smps_control_signal;


    switch(current_state) {
        
        case INITIALIZE: 
            //ToDo: start_time = current_time(); // use RTCC instead with indefinite repeat --> see datasheet ch.21 and reference Maunual p.886 - ch.29
            smps_control_signal = smps_control_zero(smps_control_signal);    // sets the SMPS output slowly to zero current, if not already at zero.
            smps_enable = NOT_CHARGE; // ToDo: Currently not implemented in hardware and software.
            current_state = TEMPERATURE; // NEXT STATE
            break;
         
            
        case TEMPERATURE:
            dynamo_temp = read_dynamo_sensor(); // ToDo: Missing hardware to allow function returning dynamo temperature sensor reading. Note, Currently using SMPS temperature sensor Pin 32, J11
            if ((dynamo_temp > MAX_DYNAMO_TEMP) && (dynamo_temp <= MIN_DYNAMO_TEMP)){
                
                smps_temp = read_smps_temp(); // read the 
                //ToDO: mcu_temp = read_mcu_temp(); // ToDo: Hardware not in place and ADC reading not implemented on the ADC module (adc.h)
                
                if((smps_temp <= START_FAN_SMPS_TEMP) || (mcu_temp <= START_FAN_MCU_TEMP)){   //turn fan ON
                    fan_control(ON); // turn on fan, ToDo: Implement fan hardware, PWM shall control it (e.g. use Pin 20, J11 on extension board.
                }
                
                else{   // turn fan OFF
                    fan_control(OFF); // turn fan off. ToDo: Implement fan hardware, PWM shall control it.
                }
                
                if((smps_temp > MAX_SMPS_TEMP) && (smps_temp <= MIN_SMPS_TEMP)){ // check if SMPS temperature is within range
                       current_state = MEASURE; 
                }
                    
                else {  // SMPS has to high/low temperature go to SAFETY_CHECK
                        safe_cnt = 0;
                        error_cnt = 1;
                        smps_enable = NOT_CHARGE;
                        current_state = SAFETY_CHECK;
                }
            }
            
            else { // dynamo temperature out of range --> safety check
                safe_cnt = 0;
                error_cnt = 1;
                smps_enable = NOT_CHARGE;
                current_state = SAFETY_CHECK;
            }
            break;
            
            
        case MEASURE:
            bms_control_ref = read_bms_control_ref(); 
            
            if((bms_control_ref <= MAX_CURRENT) && (bms_control_ref >= MIN_CURRENT)){   // Max >= bms_control_ref >= 0V
                shunt_current = read_shunt_current();
                
                if((shunt_current <= MAX_CURRENT) && (shunt_current >= MIN_CURRENT)){ // Max >= shunt_current >= 0V
                    shunt_bms_compare = bms_control_ref - shunt_current; // Compare BMS requested current and Shunt current
                    smps_enable = CHARGE;
                    current_state = CHARGING;
                }
                
                else { // shunt current reading out of range, move to SAFETY_CHECK
                    safe_cnt = 0;
                    error_cnt = 1;
                    smps_enable = NOT_CHARGE;
                    current_state = SAFETY_CHECK;
                }
            }
            
            else{   // BMS current reading out of range, move to SAFETY_CHECK
                safe_cnt = 0;
                error_cnt = 1;
                smps_enable = NOT_CHARGE;
                current_state = SAFETY_CHECK;
            }
            break;
            
            
        case CHARGING:
            if(shunt_bms_compare == 0){ //shunt_bms_compare = 0 --> do not change anything
            }    
            else { // check if current output shall increase/reduce
                
                if((shunt_bms_compare <= MAX_CURRENT) && (shunt_bms_compare > MIN_CURRENT)){   // increase current output form SMPS
                smps_control_signal = smps_increase_current(smps_control_signal); 
                }
                else{ // reduce current output from SMPS
                smps_control_signal = smps_reduce_current(smps_control_signal);
                }
            }
            

            if (TIME_INTERVAL_BETWEEN_SENDING < current_time - start_time){
                current_state = READ_BMS;
            }
            else{ // move back to temperature state again
                current_state = TEMPERATURE;
            }
            break;
            
        case SAFETY_CHECK:
            smps_control_signal = smps_control_zero(smps_control_signal);    // sets the SMPS output slowly to zero current if not already at zero. ToD: This could be a separate state
            if(error_cnt == SEND_ERROR_MSG){ // check if number of error has reached fixed value --> send error_msg to user
                current_state = ERROR_MSG;
            }
            else if (safe_cnt == SAFETY_APPROVED){ //safety_check successful
                current_state = TEMPERATURE;
            }
            else if((dynamo_temp > MAX_DYNAMO_TEMP) || (dynamo_temp < MIN_DYNAMO_TEMP)){ //dynamo temperature of out range?
                error_cnt++;
                error_ID = 1;
                safe_cnt = 0;
                current_state = SAFETY_CHECK;
            }
            else if((smps_temp > MAX_SMPS_TEMP) || (smps_temp < MIN_SMPS_TEMP)){ //SMPS temperature of out range?
                error_cnt++;
                error_ID = 2;
                safe_cnt = 0;
                current_state = SAFETY_CHECK;
            }
            else if((bms_control_ref > MAX_CURRENT) || (bms_control_ref < MIN_CURRENT)){ ////BMS current of out range?
                error_cnt++;
                error_ID = 3;
                safe_cnt = 0;
                current_state = SAFETY_CHECK;
            }
            else if((shunt_current > MAX_CURRENT) || (shunt_current < MIN_CURRENT)){ //Shunt current out of range?
                error_cnt++;
                error_ID = 3;
                safe_cnt = 0;
                current_state = SAFETY_CHECK;
            }
            else {   // else all values within range, safety approved
                safe_cnt++;
                error_ID = 0;
                error_cnt = 0;
                current_state = SAFETY_CHECK;
            }
            break;
            
        case ERROR_MSG:
            //ToDo: send_error_msg(error_ID);
            error_cnt = 0;
            current_state = SAFETY_CHECK;
            break;
            
        case READ_BMS:
            //ToDo: read_can_bus_bms();
            current_state = SEND_TO_CLOUD;
            break;
            
        case SEND_TO_CLOUD:
            //ToDo: transmit_to_cloud();
            current_state = TEMPERATURE;
            break;     
            
        default:
            safe_cnt = 0;
            error_cnt = 1;
            smps_enable = NOT_CHARGE;
            current_state = SAFETY_CHECK;
            
    }
}


