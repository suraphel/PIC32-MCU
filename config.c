/* DEVCFG0: Device Configuration Word 0*/
#pragma config DEBUG = OFF
#pragma config JTAGEN = OFF
#pragma config ICESEL = ICS_PGx2
#pragma config PWP =OFF
#pragma config BWP = ON
#pragma config CP = OFF

/* DEVCFG1: Device Configuration Word 1*/
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config IESO = OFF 
#pragma config POSCMOD = OFF
#pragma config OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1
#pragma config FCKSM = CSDCMD
#pragma config WDTPS = PS1
#pragma config WINDIS = OFF
#pragma config FWDTEN = OFF // watchdog not enabled
#pragma config FWDTWINSZ = WINSZ_25

/* DEVCFG2: Device Configuration Word 2*/
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config UPLLIDIV = DIV_2
#pragma config UPLLEN = OFF
#pragma config FPLLODIV = DIV_2

/* DEVCFG3: Device Configuration Word 3*/
#pragma config USERID = 0xffff
#pragma config PMDL1WAY = OFF
#pragma config IOL1WAY = OFF
#pragma config FUSBIDIO = OFF
#pragma config FVBUSONIO = OFF