/* 
 * File:   uart.h
 * Initializes the UART1 TX module.
 * Remember to select output pin in main, e.g: RPF0R = 0b0011; //UART U1TX PIN 7, J11
 * This file is currently only implementing UART1 TX, requires further developmxent
 * for any practical use beyond reading test data.
 */

#ifndef UART_H
#define	UART_H
void uart1_tx_init_9600(void); // initialize UART TX at a 9600 baud rate with 8-bit, no parity bit and a stop bit, without RTS and CLTS and not using interrupt.
void transmit_uart1 (const unsigned short * data,const unsigned short array_size_bytes); // send a unsigned short array: need the array and the size of it.
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

