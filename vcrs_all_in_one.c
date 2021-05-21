//config files
//DEVCON3:0

#include config.c;

// vcrs files
// vcrs.h

#include vcrs.h;


unsigned int currrent_time = 0;
unsigned int start_time = 0;

// pre start charging check list
// system temp (units are in voltage)

const short smps_temp_max = 800;
const short smps_temp_min = 200;

const short dynamo_temp_max = 800;
const short dynamo_temp_min = 200;

const short shunt = 0; 

const short bms_max_request = 25;
const short bms_min_request = 0;

const short error_message_cnt = 0;
const short max_error_broadcast = 7; 
const short safe_state_restored = 0;


const short smps_fan_activates = 450;
const short mcu_fan_activates = 450;

//----------------------------------------
// as switch does not take in string it needs to be converted to int!



enum {
    on, off

};

enum {
    charge, notcharge
};

enum {
    init,
    Temperature
    Measurement
    Charging
    safe_state
    Error_message
};






// ------------------------------------------------//
#include <stdio.h>
int main()
{
    int i = 2;
    switch (i)
    {
    case 1:
        printf("Case1 ");
    case 2:
        printf("Case2 ");
    case 3:
        printf("Case3 ");
    case 4:
        printf("Case4 ");
    default:
        printf("Default ");
    }
    return 0;
}

switch (current_state){

case init: {
    start_time = current_time;
    smps_control_zero = smps_control_signal(smps_control_zero);
    current_state = temperature;     
}

case Temperature: 
    dynamo_temp = read_dynamo();

case 3:


}















// adc file 
// adc.h 

//adc.c



void read_dynamo(void) {
    return ADC1BUF0;
}

void read_smps(void) {
    return ADC1BUF1;
}