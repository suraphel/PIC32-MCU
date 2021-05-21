/* 
 * File:   adc.h
/* 
 * configure ADC bits to read analog input in scan mode from pin A2-A5 (pin 32-29, J11 on
 * expansion board), using the peripheral bus clock at 40MHz. Implying at sampling time per
 * ADC reading at 1400 nanosecond, and reading every input once per 5600 ns. 
 * Initialization follows the receipe at page 481 in the PIC32 reference manual
 * 
 * Further this file enables reading of temperature and current.
 * 
 * Note, AN0 and AN1 shorted circuited during testing.
*/

#ifndef ADC_H
#define	ADC_H

void adc_init(void); // initialize ADC module in scan mode on pin A2-A5
unsigned short read_dynamo_sensor(void); // read dynamo sensor temperature, pin A2 (pin 32, J11)
unsigned short read_smps_temp(void);    // reads SMPS temperature at pin A3 (pin 31, J11)
double read_bms_control_ref(void); // read BMS current request
double read_shunt_current(void); // read shunt current request
#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

