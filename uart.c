/* 
 * File:   uart.h
 * Initializes the UART1 TX module.
 * Remember to select output pin in main, e.g: RPF0R = 0b0011; //UART U1TX PIN 7, J11
 * This file is currently only implementing UART1 TX, requires further developement
 * for any practical use beyond reading test data.
 */


#include <xc.h>

void uart1_tx_init_9600(void){ // baud rate of 9600, see guide reference manual ch.21 p.18
  //  U1MODEbits.ON  = 0; // disable the UART for safety
    
// 1. Initialize the UxBRG register for the appropriate baud rate
    U1BRG = 0x103; // 259 --> with 40MHz PBCLK and 16x oversampling scheme --> baud rate 9600bits/s
    U1MODEbits.BRGH = 0; // 16x oversampling scheme

// 2. Set the number of data and Stop bits, and parity selection by writing to the PDSEL<1:0> bits (UxMODE<2:1>) and STSEL bit (UxMODE<0>).
    U1MODEbits.UEN = 0b00; //?
    U1MODEbits.PDSEL = 0b00;
    U1MODEbits.STSEL = 0;

//3. Interrupts 
    //not in use

//4. Enable transmission
    U1STAbits.UTXEN = 1;

    //U1STAbits.URXEN = 1; // enable U1RX
// 6. Enable UART module
    //U1MODEbits.LPBACK = 1; // enable loopback, connect Tx to Rx interally - useful during testing
    U1MODEbits.ON  = 1;
}

void transmit_int_uart1 ( unsigned short * data, const unsigned short array_size_bytes){ // transmit integer array   
    unsigned char start_msg = '%';
    const unsigned int initial_address = (unsigned int *) data; // all pointer variables are 32-bit, independent of data type 
    unsigned char * char_data_array = (unsigned char *) data; // type cast array to char array
    
    while(char_data_array < (initial_address + array_size_bytes)){ // not transmitted all data
        // 1. send start byte
        U1TXREG = start_msg;
        
        while (U1STAbits.UTXBF){ // do not continue if FIFO buffer is full           
        }
        // 2. send the data
        U1TXREG = *char_data_array++; // get value, thereafter increment to next address (index of array)
        U1TXREG = *char_data_array++; 
        
        // 3. send end of line character
        U1TXREG = '\n'; // send end of transmission
        start_msg++; // change start char of msg
        }
}
