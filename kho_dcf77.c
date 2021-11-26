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

#include "kho_dcf77.h"
#include "kho_ds1340.h"
#include "user.h"

/* The function inits all peryphelia needed to run UKF receiver. */
void dcf77_init(void)
{
    TRISCbits.TRISC2    = 0x00;
    TRISBbits.TRISB0    = 0x01;

    INTCON2 |= 0b11000000; //intedg0 = 1 int0f = 0 int0e = 0
    INTCON &= ~0b00010010;

    PORTCbits.RC2 = 0x01;   //turn off dcf77 receiver
            //INTCON<0> RBIF
}

void dcf77_countTime(void)
{
    if(counterEnabled)
    {
        timeCounter++;  //add new tick
    }
}

void dcf77_startSynch(void)
{
    if(startSynch && !counterEnabled)
    {
        counterEnabled = 0x01;
        strobeDetected = 0x00;
        dcf77Synchronized = 0x00;
        timeCounter = 0;        //reset counter to zero
        INTCON |= 0b00001000;   //enable interrupt
        PORTCbits.RC2 = 0x00;   //turn on dcf77 receiver
    }
}

void dcf77_waitSynchStrobe(void)
{
    if(strobeDetected && startSynch)
    {
        if(timeCounter > 12 && timeCounter < 16)
        {
            dcf77Synchronized = 0x01;
        }
        else
        {
            counterEnabled = 0x00;
        }
    }
}

/* The function starts to read data from UKF device. */
void dcf77_dateTimeReadout(void)
{
    uint8_t hour, minute, second;
    uint8_t year, month, day, week;
    uint8_t dataPosition;
    uint8_t dataReceived[59];
    uint8_t lowTime,highTime;
    uint8_t sysT_start, sysT_stop;
    uint8_t attemptCounter;
    uint32_t smallAttemptCounter;
    
    isRaise = 0x00; //no strobe detected
    dataPosition = 0;//init index

    INTCON |= 0b00010000;   //enable interrupt
    PORTCbits.RC2 = 0x00;   //turn off dcf77 received
    
    sysT_start = systemTick;
    attemptCounter = 0;
    checkStatus = 0;
    while(attemptCounter < MAX_SYNCH_ATTEMPTS)
    {
        smallAttemptCounter = 0;
        while(isRaise != 0x02 && smallAttemptCounter++ != MAX_SMALL_ATTEMPTS<<2) ; //wait for falling edge
        if((smallAttemptCounter) < MAX_SMALL_ATTEMPTS) //if falling edge found
        {
            checkStatus |= (0x01<<15); //falling edge found
            sysT_start = systemTick; //start counter
            smallAttemptCounter = 0;
            while(isRaise != 0x01 && smallAttemptCounter++ != MAX_SMALL_ATTEMPTS) ; //wait for rising edge
            if((smallAttemptCounter) < MAX_SMALL_ATTEMPTS) //if rising edge found
            {
                checkStatus |= (0x01<<14); //rising edge found
                old_systemTick_diff = systemTick - sysT_start; //calculate no of impulses
                sysT_start = systemTick;
                if(old_systemTick_diff > 11 && old_systemTick_diff < 17) //check if value in the scope
                {
                    checkStatus |= (0x01<<13); //synchronization bit found
                    break;
                }
                else
                {
                    attemptCounter++;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if(checkStatus == 0xE000)
    {
        while(dataPosition < 57)
        {
            smallAttemptCounter = 0;
            while(isRaise != 0x02 && smallAttemptCounter++ != MAX_SMALL_ATTEMPTS) ; //wait for falling edge
            if((smallAttemptCounter) < MAX_SMALL_ATTEMPTS) //if falling edge found
            {
                old_systemTick_diff = systemTick - sysT_start; //calculate no of impulses
                highTime = old_systemTick_diff;
                sysT_start = systemTick;
                smallAttemptCounter = 0;
                while(isRaise != 0x01 && smallAttemptCounter++ != MAX_SMALL_ATTEMPTS) ; //wait for rising edge
                if((smallAttemptCounter) < MAX_SMALL_ATTEMPTS) //if falling edge found
                {
                    old_systemTick_diff = systemTick - sysT_start; //calculate no of impulses
                    lowTime = old_systemTick_diff;
                    sysT_start = systemTick;
                    if(lowTime > 1 && lowTime < 3)
                    {
                        dataReceived[dataPosition++] = 1;
                        checkStatus++;
                    }
                    if(lowTime < 2)
                    {
                        dataReceived[dataPosition++] = 0;
                        checkStatus++;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        if((checkStatus & 0x00011111)==57)
        {
            if(1) //checksums
            {
                checkStatus |= (0x01<<13); //checksums correct
                second = 0x00;
                minute =    dataReceived[21] << 0;
                            dataReceived[22] << 1;
                            dataReceived[23] << 2;
                            dataReceived[24] << 3;
                            dataReceived[25] << 4;
                            dataReceived[26] << 5;
                            dataReceived[27] << 6;
                hour =      dataReceived[29] << 0;
                            dataReceived[30] << 1;
                            dataReceived[31] << 2;
                            dataReceived[32] << 3;
                            dataReceived[33] << 4;
                            dataReceived[34] << 4;
                day =       dataReceived[36] << 0;
                            dataReceived[37] << 1;
                            dataReceived[38] << 2;
                            dataReceived[39] << 3;
                            dataReceived[40] << 4;
                            dataReceived[41] << 5;
                month =     dataReceived[45] << 0;
                            dataReceived[46] << 1;
                            dataReceived[47] << 2;
                            dataReceived[48] << 3;
                            dataReceived[49] << 4;
                year =      dataReceived[50] << 0;
                            dataReceived[51] << 1;
                            dataReceived[52] << 2;
                            dataReceived[53] << 3;
                            dataReceived[54] << 4;
                            dataReceived[55] << 5;
                            dataReceived[56] << 6;
                            dataReceived[57] << 7;
                          week = dataReceived[26];
                          if(month < 12 && day < 32 && hour < 25)
                          {
                              checkStatus |= (0x01<<12); //data i time correct
                              //DS1340_setTime(hour, minute, second);
                              //DS1340_setDate(week, day, month, year);
                          }
            }
        }

    }
    else
    {
        hour = 1 ;
        minute = 2 ;
        second = 3;
        year = 4 ;
        month = 5;
        day = 6 ;
        week = 7;
    }
    
    INTCON &= ~0b00010000;   //disable interrupt
    PORTCbits.RC2 = 0x01;   //turn off dcf77 receiver

    DS1340_setTime(hour, minute, second);
    DS1340_setDate(week, day, month, year);
}