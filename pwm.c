/* 
 * File:   pwm.c
 * 
 * Enables the output compare module to output
 * PWM signals with about 4900Hz with the Timer 2 module.
 * Output pin selection is performed main.c
 * 
 * See in main.c:
 * RPD0R = 0b1100;  // set pin RD0 to PWM OC1
 * RPD1R = 0b1011;  // set pin RD1 to PWM OC2
 * RPD2R = 0b1011;  // set pin RD2 to PWM OC3
 * 
 * Further, this header enables control
 * of the SMPS control signal.
 */

#include <xc.h>

void pwm_init_pin_RD0(){ // initialize a PWM output signal from pin RD0 on the I/O expansion board starter kit
// Output Compare pin-1 Config
    OC1CONbits.SIDL = 0;    // Stop or not stop in Idle Mode bit
    OC1CONbits.OC32 = 0;    // 32-bit Compare Mode bit 1 = OCxR and/or OCxRS are used for comparisons to the 32-bit timer source 0 = OCxR and OCxRS are used for comparisons to the 16-bit timer source
    OC1CONbits.OCFLT = 0;   // PWM Fault Condition Status bit(2) 1 = PWM Fault condition has occurred (cleared in HW only) 0 = No PWM Fault condition has occurred
    OC1CONbits.OCTSEL = 0;  // Output Compare Timer Select bit
    OC1CONbits.OCM = 0b110; // OCM: Output Compare Mode Select bits 111 = PWM mode on OCx; Fault pin enabled 110 = PWM mode on OCx; Fault pin disabled
    OC1R = 0;               // Writing the initial value for the duty cycle of the PWM-signal, thereafter read-only signal
    OC1RS = 0b0;         // The next duty cycle. The value loaded to the OC1R as the newly set duty cycle. 

    OC1CONbits.ON = 1;      // Enabling the OC1-pin. Output Compare Peripheral On bit(1) 1 = Output Compare peripheral is enabled 0 = Output Compare peripheral is disabled
}
    
void pwm_init_pin_RD1(){ // initialize a PWM output signal from pin RD1 on the I/O expansion board starter kit
// Output Compare pin-2 Config
   OC2CONbits.SIDL = 0;    
    OC2CONbits.OC32 = 0;    
    OC2CONbits.OCFLT = 0;   
    OC2CONbits.OCTSEL = 0;  
    OC2CONbits.OCM = 0b110; 
    OC2R = 0;               
    OC2RS = 0b0;         

    OC2CONbits.ON = 1;      
 
}

 
void pwm_init_pin_RD2(){  // initialize a PWM output signal from pin RD2 on the I/O expansion board starter kit  
// Output Compare pin-3 Config
    OC3CONbits.SIDL = 0;    
    OC3CONbits.OC32 = 0;    
    OC3CONbits.OCFLT = 0;  
    OC3CONbits.OCTSEL = 0;  
    OC3CONbits.OCM = 0b110; 
    OC3R = 0;               
    OC3RS = 0b0;   

    OC3CONbits.ON = 1;    
}


void pwm_init_tmr2 (){  // initialize the clock for a 10-bit PWM output resolution
// Timer 2 Configuration:
    T2CONbits.ON = 0;           // disbale TIMER 2
    PR2 = 0b1111111111;         // PWM period 1023-bit = 11 1111 1111
    T2CONbits.SIDL = 0;         // Stop in Idle Mode bit
    T2CONbits.TGATE = 0;        // TGATE: Timer Gated Time Accumulation Enable bit
    T2CONbits.TCKPS = 0b011;    // How many clock cycles per one increment on counter, count is stored in the TMR2
    T2CONbits.T32  = 0;         //2-Bit Timer Mode Select bit(2) 1 = Odd numbered and even numbered timers form a 32-bit timer 0 = Odd numbered and even numbered timers form a separate 16-bit timer
    T2CONbits.TCS = 0;          // TCS: Timer Clock Source Select bit 1 = External clock from TxCKI pin 0 = Internal peripheral clock
    TMR2 = 0x0001;              // Start counting from 1      
    
    T2CONbits.ON = 1;           // Activating the Timer2 counter. Timer On bit(1)
}



void pwm_init(){ // initialize all the pins to PWM
    pwm_init_pin_RD0();
    pwm_init_pin_RD1();
    pwm_init_pin_RD2();
}


/******************************FUNCTIONS***************************************/

void delay_smps_control_zero(void) { // safety delay for testing, ToDo: use RTCC for this in final version, not necessary to keep MCU busy
  int j;
  for (j = 0; j < 100000; j++) {
      ;
  }
}

unsigned short smps_control_zero(unsigned short smps_control_signal){ // sets the SMPS output to zero current
    while(OC1RS > 0){
        smps_control_signal--; // ramp down
        OC1RS = smps_control_signal;
        delay_smps_control_zero(); // Protects the SMPS against rapid changes. ToDo: Remove when RTCC is implemented
    }
    smps_control_signal = 0; // Safety: ensure that the control signal is zero.
    return smps_control_signal;
}


unsigned short smps_increase_current(unsigned short smps_control_signal){
    if (smps_control_signal < 1023){ // Ramp up 
        smps_control_signal ++; // If the duty cycle is not at max, increase
        OC1RS = smps_control_signal; // Write new duty cycle
        return smps_control_signal;
    } 
}

unsigned short smps_reduce_current(unsigned short smps_control_signal){
    if (smps_control_signal > 0){ // Ramp up 
        smps_control_signal --; // If the duty cycle is not at max, increase
        OC1RS = smps_control_signal; // Write new duty cycle
        return smps_control_signal;
    } 
 }

void fan_control(const int mode){
    if(mode){
        OC2RS = 1023; // turn fan on full speed
    }
    else {
        OC2RS = 0; // turn fan off
    }
}
