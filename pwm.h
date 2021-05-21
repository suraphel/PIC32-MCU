/* 
 * File:   pwm.h
 * 
 * Enables the output compare module to output
 * PWM signals at approximate 4900Hz using the Timer 2 module.
 * Output pin selection is performed main.c
 * 
 * Further, this header enables control
 * of the SMPS control signal.
 */

#ifndef PWM_H
#define	PWM_H
void pwm_init(void);
void pwm_init_tmr2 ();

unsigned short smps_control_zero(unsigned short smps_control_signal);
unsigned short smps_increase_current(unsigned short smps_control_signal);
unsigned short  smps_reduce_current(unsigned short smps_control_signal);
void fan_control(const int mode);
#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

