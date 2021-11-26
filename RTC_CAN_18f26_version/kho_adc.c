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

#include "kho_adc.h"

uint8_t initADC(void)
{
    uint8_t returnValue;

    returnValue = 0;

    TRISA |= 0b00000001;
    ANCON0 |= 0b00000001;
    ANCON1 = 0x00;

    ADCON0 = 0b00000000;
    ADCON1 = 0b00000000;
    ADCON2 = 0b10101010;


    ADCON0 = 0b00000001;

    PIR1 |= 0b01000000;
    PIE1 &= ~0b01000000;
    IPR1 &= ~0b01000000;

    dataADCalculationFlag = 0;
    dataADCaquasitionFlag = 0;

    return returnValue;
}

void adcStartAquasition(void)
{
    
}