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
#include "kho_dcf77.h"

//#include "sensorsFunctions.h"
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

#define HIGH 0x01
#define LOW  0x00

void InitializeTimer (void) {
    //set interrupts
    INTCON  &= ~(0b00100000); //disable TMR0IE overflow interrupt
    INTCON2 |= 0b00000100; //set TMR0IP overflow interrupt priority to high
    //set pins

    TRISAbits.TRISA4 = 1;
    //set internal registers
    //T0CON = 0b00000111;
    T0CON = 0b01101111;
  // TMR0H = 0xF9;
  //  TMR0L = 0xEB; //rate
    TMR0H = 0xFF;
    TMR0L = SYSTEM_TICK; //rate 512
    //enable periphelia
    //T0CON = 0b10000111;
    T0CON = 0b11101111;
    systemTick = 0;
    old_systemTick = 0;
    //enable interrupts
    INTCON  |= 0b00100000; //enable TMR0IE overflow interrupt
}

void InitializePWM (void)
{
  //set interrupts
  //
  //set pins
  TRISCbits.TRISC2 = 0;      /* configure the CCP1 modul */
  //set internal registers
  PR2 = 0x7A;                /* initialize the PWM period */
  TMR2 = 0x00;
  CCPR1L = 0x7A >> 1;             /* initialize the PWM duty cycle */
  T2CON = 0x000;              /* postscale 1:1, Timer0 OFF, prescaler 1 */
  //enable periphelia
  CCP1CON = 0x0F;            /* turn the PWM on */
  //enable interrupts
  //
}

void InitializeOutputs (void)
{
    ADCON1 = 0x0F;

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


void InitApp(void)
{
    //setPortAPinOn(4);
    
    RCON = 0b10000000; //RCONbits.IPEN = 1; //turn on int priorities
    INTCON2 = 0b00000000;
    INTCON3 = 0b00000000;
    INTCON  = 0b11000000;//enable high and low interrupts
    PIE1 = 0b00000000;
    PIE2 = 0b00000000;
    IPR1 = 0x00;
    IPR2 = 0x00;
    
    //RCON    |= 0b10000000; //set priority levels on interrupts on
    //INTCON  |= 0b00000000; //turn off all interrupts

    InitializeOutputs();
    //InitializeCAN();
    initCAN();
    
    InitializeTimer();
    initI2C();
   // InitializePWM();
    dcf77_init();
    //initialize interrupts
    INTCON  |= 0b11000000; //enable high priority inerupts|enables low priority interrupts
}