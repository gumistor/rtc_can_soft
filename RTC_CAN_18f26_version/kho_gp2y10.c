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
#include "kho_gp2y10.h"

uint8_t initGp2y10(void)
{
    uint8_t returnValue;

    returnValue = 0;

    initADC();

    gp2y10_pm100_concentration = 0;
    gp2y10_pm100_concentration_table_index = 0;
    
    PORTC &= ~0b00000100;

    sensorState = gp2_idle;
    
    gp2y10_pm100_zeroValue = 591;
    
    return returnValue;
}

void gp2y10_gather_data(void)
{
    uint32_t currentValue;
    uint8_t index;
    
  //  if(sensorState == gp2_convertedData)
  //  {
        //dataADCalculationFlag = 0;
        //dataADCaquasitionFlag = 0;
   //     sensorState = gp2_wait;

        //gp2y10_pm100_concentration = ((dataADC*9)/50)-180;
        //currentValue = dataADC*9; // *10 mv
        //if(currentValue > 9000)
        //{
        //    currentValue = (currentValue - 9000)/50; 
        //}   
        //else
        //{
        //    currentValue = 0;
        //}
        currentValue = 0;

        gp2y10_pm100_zeroValue = 46;
        
        for(index=0; index < 128; index++)
        {
            currentValue = currentValue + gp2y10_pm100_concentration_table[index];
            //if(gp2y10_pm100_concentration_table[index] < gp2y10_pm100_zeroValue && gp2y10_pm100_zeroValue - gp2y10_pm100_concentration_table[index] < 100)
            //{
            //    gp2y10_pm100_zeroValue = gp2y10_pm100_concentration_table[index];
            //}
            //currentValue -= gp2y10_pm100_zeroValue;
        }
        
        //currentValue = gp2y10_pm100_zeroValue;
        
        currentValue *= 9;
        
        currentValue = currentValue/50;
        
        
        
        //currentValue = currentValue ;
        gp2y10_pm100_concentration = (currentValue >> 7) - gp2y10_pm100_zeroValue;
        
  //  }

    if(sensorState == gp2_idle)
    {
        sensorState = gp2_prepareData;
        PORTC |= 0b00000100;
        //adcStartAquasition();
        PIE1 |= 0b00000010;
        T2CON = 0x05;
        //dataADCalculationFlag = 0;
        //dataADCaquasitionFlag = 1;
    }
}

/*
 *
 */

