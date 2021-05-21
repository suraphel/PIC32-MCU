/* 
* File:   adc.c
 * configure ADC bits to read analog input in scan mode from pin A2-A5, J11
 * using the peripheral bus clock at 40MHz. Implying at sampling time per
 * ADC reading at 1400 nanosecond, and reading every input once per 5600 ns. 
 * Initialization follows the receipe at page 481 in the PIC32 reference manual
 * 
 * Further this file enables reading of temperature and current.
 * 
 * Note, AN0 and AN1 shorted circuited during testing.
 * 
*/
#include <xc.h>


void adc_init(void){ 
    // 1. Configure the analog port pins in AD1PCFG<15:0> (see 17.4.1).
    TRISB = 0xFFFF; // set PORTB's <3:0> to input, on reset input.
    ANSELB = 0xFFFF;  // set PORTB's <3:0> to analog, on reset input.
    
    // 2. Select the analog inputs to the ADC multiplexers in AD1CHS<32:0> (see 17.4.2)
    AD1CHSbits.CH0NA = 0; // Determines the negative input for MUX A
    // AD1CHSbits.CHOSA - ignored in scan mode
    
    // 3. Select the format of the ADC result using FORM<2:0> (AD1CON1<10:8>) (see 17.4.3).Determines the data output format. We usually use either. 
    //0b100 32-bit unsigned integer. 0b000 16-bit unsigned integer (the default).
    AD1CON1bits.FORM = 0b000;
    
    // 4. Select the sample clock source using SSRC<2:0> (AD1CON1<7:5>) (see 17.4.4).
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 1;
    
    // 5. Select the voltage reference source using VCFG<2:0> (AD1CON2<15:13>) (see 17.4.7).
    AD1CON2bits.VCFG = 0b000;
    
    // 6. Select the Scan mode
    AD1CON2bits.CSCNA = 1; // checked
    AD1CSSL = 0x003C; //Current Adds pin 29-32 // Adds pin 30: 0x001F;   //Original: 0x001F enable analog scan on AN0-AN3
    
    // 7.  Set the number of conversions per interrupt.
    AD1CON2bits.SMPI = 0b011; //Original: 0b0011;
    
    // 8. Set Buffer Fill mode using BUFM
     AD1CON2bits.BUFM = 0;
     
     // 9. Select the MUX to be connected to the ADC in ALTS
     AD1CON2bits.ALTS = 0; // checked
     
     // 10.  Select the ADC clock source using ADRC
     AD1CON3bits.ADRC = 0;
     
     // 11.  Select the sample time using SAMC
     AD1CON3bits.SAMC = 0b00010;
     
     // 12.  Select the sample time using SAMC<4:0> (AD1CON3<12:8>), if auto-convert is to be used (see 17-2).
     AD1CON3bits.ADCS = 0b00000001;
     
     // 13.Turns ON the ADC module using AD1CON1 (see 17.4.14).
     AD1CON1bits.ON = 1;
     
     // 14. Not required.
     
     // 15. Start the conversion sequence by initiating sampling (see 17.4.15).
     AD1CON1bits.ASAM = 1;
     
}

unsigned short read_dynamo_sensor(void){ // dynamo temperature at pin 32, J11 
    return ADC1BUF0;    //read the ADC buffer with the result
}

unsigned short read_smps_temp(void){ // SMPS temperature at pin 31, J11 
    return ADC1BUF1;    //read the ADC buffer with the result
}

double read_bms_control_ref(void){ // BMS reference at pin 30, J11
    const double voltage_increase_per_ampere = 0.066; // defines the voltage increase when the current increases by one.
    const double adc_bit_resolution = 1024;
    const double max_input_voltage = 3.3; 
    double bms_adc_reading = ADC1BUF2;
    double bms_voltage = (bms_adc_reading / adc_bit_resolution) * max_input_voltage;
    double bms_current = bms_voltage / voltage_increase_per_ampere;
    return bms_current;    //return the converted BMS current for comparison
}

double read_shunt_current(void){ // Shunt reading at pin 29, J11
    const double zero_ampere = 0.25; // Shunt reading start at 0.25 V,i.e. 0 Ampere
    const double adc_bit_resolution = 1024;
    const double voltage_increase_per_ampere = 0.027; // defines the voltage increase when the current increases by one.
    const double max_input_voltage = 3.3; 
    double shunt_adc_reading = ADC1BUF3; 
    double shunt_voltage = (shunt_adc_reading / adc_bit_resolution) * max_input_voltage;
    double shunt_current = (shunt_voltage - zero_ampere) / voltage_increase_per_ampere;
    if(shunt_current < 0){
        shunt_current = 0;
    }
    return shunt_current;    //return the converted shunt current for comparison
}
