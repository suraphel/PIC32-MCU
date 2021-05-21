/* 
 * File:   main.c
 * 
 * This is the main file of the vcrs_embedded_system
 * embedded software version 2. See the thesis
 * report ch.5 for details. It main purpose is to
 * run the system and perform the MCU device configurations
 * and output pin selection.
 * 
 * The project applies at PIC32MX570F512L MCU.
 * Programming is performed on the bare metal, and
 * at the time of writing the PWM and ADC module has been
 * verified functioning. However, several aspects of the 
 * of the Embedded software version 2 has not been implemented.
 * The not implemented parts are marked with ToDo, and
 * some dummy variables as place holders in the various
 * source files.
 * 
 * Some of the not implemented functionalities relies on hardware
 * that is currently not implemented in the system. However, from
 * the MCU's perspective, the RTCC, CAN, I²C and Interrupt modules
 * have not been implemented. Determining which variables that 
 * needs to be allocated into the EEPROM memory shall also be researched.
 * Lastly, implementing the UART (RS232) and the USB module of the MCU
 * would be useful for retrieving system information while debugging and
 * inspecting system behavior.
 * 
 * Note, pin 33, 34 and 6 on the I/O expansion board of the 
 * MCU appears to be damaged
 */




#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    // needed for boolean variables --> see XC32_compiler_user_guide 
#include <xc.h>                 // Load the header for the processor
#include "vcrs_state_machine.h" // primary main logic of the VCRS
#include "adc.h"
#include "pwm.h"
#include "uart.h"
#includr "config.c"

void delay(void);

int main(void) {
    // initialize output pins
    RPD0R = 0b1100;  // set pin RD0 to PWM OC1
    RPD1R = 0b1011;  // set pin RD1 to PWM OC2
    RPD2R = 0b1011;  // set pin RD2 to PWM OC3

   adc_init();
   pwm_init();
   pwm_init_tmr2 ();
    while(1){
        run_vcrs();
        delay(); // for safety, protecting against to rapid changes in the SMPS control signal
    }
    
  return 0;
}



void delay(void) {
  int j;
  for (j = 0; j < 100000; j++) {
      ;
  }
}


