/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "user.h"
#include "kho_can.h"
#include "kho_i2c.h"
#include "kho_ds1340.h"

//#include "sensorsFunctions.h"
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

#define HIGH 0x01
#define LOW  0x00

void InitializeOutputs (void)
{
    //ADCON0=0x00; 
    ADCON1=0x0F; 

    TRISC = 0x00;
    TRISB = 0x00;
    TRISA = 0x00;

    PORTC = 0x00;
    PORTB = 0x00;
    PORTA = 0x00;

    LATC = 0x00;
    LATB = 0x00;
    LATA = 0x00;
    
}

void InitializeTimer (void) {
    //set interrupts
    INTCON  &= ~(0b00100000); //disable TMR0IE overflow interrupt
    INTCON2 |= 0b00000100; //set TMR0IP overflow interrupt priority to high
    //set pins
    //
    //set internal registers
    T0CON = 0b00000111;
    TMR0H = 0x9E;
    TMR0L = 0xBC; //rate
    //enable periphelia
    T0CON = 0b10000111;
    systemTick = 0;
    old_systemTick = 0;
    //enable interrupts
    INTCON  |= 0b00100000; //enable TMR0IE overflow interrupt
}

void InitApp(void)
{
    //setPortAPinOn(4);
    
    RCON = 0b10000000; //RCONbits.IPEN = 1; //turn on int priorities
    INTCON2 = 0b00000000;
    INTCON3 = 0b00000000;
    INTCON  = 0b11001000;//enable high and low interrupts RB port int enable
    PIE1 = 0b00000000;
    PIE2 = 0b00000000;
    PIE3 = 0b00000000;
    PIE4 = 0b00000000;
    PIE5 = 0b00000000;
    IPR1 = 0x00;
    IPR2 = 0x00;
    IPR3 = 0x00;
    IPR4 = 0x00;
    IPR5 = 0x00;

    InitializeOutputs();
    InitializeTimer();
    //initCAN();
    initI2C();
    DS1340_init();
    
    TRISCbits.TRISC7 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB5 = 1;
    
    LATCbits.LATC7 = 0;
    LATBbits.LATB6 = 0;
    LATBbits.LATB5 = 0;
    //initialize interrupts
    IOCB = 0b01100000;
    WPUB = 0b01100000;
    INTCON  |= 0b11001000; //enable high priority inerupts|enables low priority interrupts
    INTCON2 &= ~(0b10000000);
}