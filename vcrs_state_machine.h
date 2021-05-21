/* 
 * File:   vcrs_state_machine.h
 * 
 * This file provides the interface to the control logic
 * of the VCRS system, i.e. the VCRS state machine
 * available in the report, Chapter 5.
 * 
 * Note, some discrepancies are present, explicitly
 * mentioned in the vcrs_state_machine.c source file.
 */

#ifndef VCRS_STATE_MACHINE_H    // checks if this header file is already included
#define	VCRS_STATE_MACHINE_H    // if this header file is not included, define it

void run_vcrs(void);

#ifdef	__cplusplus /*makes sure the extern "C" will work when compiling C++ programs,
                     * and not C programs. This allows both C and C++ programs
                     * to share the same header file. */
extern "C" {    /*Defining a C++ function as extern "C",
                 *  tells the compiler to generate function
                 *  names and function calls that are compatible
                 *  with the old "C" standard. 
                 * This way a C program can call C++ programs and the other way round. */
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* VCRS_STATE_MACHINE_H */

