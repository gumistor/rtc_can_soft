/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <string.h>
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "kho_can.h"
#include "kho_i2c.h"
#include "kho_ds1340.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void doSensorFrame(void)
{

}

void doDisplayFrame(void)
{
    /*
    static uint8_t dots_value = 0x00;
    static uint8_t last_second_lsn;

    if(second_lsn & 0x01) dots_value = 0x14;
    else dots_value = 0x00;

    if(last_second_lsn != second_lsn)
    {
        configureTXbuffer(TX1BUFFER, CAN_ADDR_DISPLAY         | can_node_class<<8 |    device_id,1);
        if(displayTime == DISPLAY_TIME)
        { 
            sendTXbuffer(TX1BUFFER, second_lsn,second_msn,minute_lsn,minute_msn,
                hour_lsn,hour_msn,dots_value,0xFF,0x08);
        }
        else if(displayTime == DISPLAY_DATE)
        {
            sendTXbuffer(TX1BUFFER, (timeDateStucture.year) & 0x0F,(timeDateStucture.year >> 4) & 0x0F,
                (timeDateStucture.month) & (0x0F),(timeDateStucture.month >> 4) & 0x0F,
                (timeDateStucture.day) & (0x0F),(timeDateStucture.day >> 4) & 0x0F,0x14,0xFE,0x08);
        }
        else
        {
            sendTXbuffer(TX1BUFFER, (timeDateStucture.week) & 0x0F, 0xFF, 0xFF,0xFF,0xFF,0xFF,0x02,0xFD,0x08);
        }
    }
    last_second_lsn = second_lsn;
    */
}


void doCAN(void)
{
    uint16_t i;
    
    if(CAN_data_received_flag == CAN_DATA_FULL)
    {
        switch(CAN_data_received.CAN_address & 0x1FFF0000)
        {
            case CAN_ADDR_ACTUATOR:
                //no action is needed
            break;
            case CAN_ADDR_DISPLAY:
                //no action is needed
            break;
            case CAN_ADDR_CONFIG_REQ:
                switch(CAN_data_received.CAN_data[0])
                    {
                    case 0x00:
                        break;
                    case 0x01:
                        break;
                    case 0x02: 
                        break;
                    case 0x03: 
                        break;
                    case 0x04:
                        break;
                    case 0xFE:  
                        break;
                    case 0xFF:
                        EEPROM_WRITE(0xFF,0xFF);
                        for(i=0; i<2000; i++);
                        Reset();
                        break;
                    default:    // no action
                        break;
                    }
                break;
            case CAN_ADDR_REQUEST:

            break;
            case CAN_ADDR_RESPONSE:

            break;
            case CAN_ADDR_NM:
            break;
            default:
                ;
        }
        CAN_data_received_flag = CAN_DATA_EMPTY;
    }
}

#define _sendBitEnd_old(mask) PORTC |= 0b00000100;\
        if(mask)\
        {\
            Nop();\
            Nop();\
            Nop();\
            Nop();\
            Nop();\
            PORTC &= ~0b00000100;\
        }\
        else\
        {\
            PORTC &= ~0b00000100;\
            Nop();\
            Nop();\
            Nop();\
        }

#define _sendBitEnd(mask) PORTA |= 0b00000001;\
        if(mask)\
        {\
            Nop();\
            Nop();\
            Nop();\
            Nop();\
            Nop();\
            PORTA &= ~0b00000001;\
        }\
        else\
        {\
            PORTA &= ~0b00000001;\
            Nop();\
            Nop();\
            Nop();\
        }

#define _sendBit(mask) _sendBitEnd(mask)\
        Nop();\
        Nop();\
        Nop();\
        Nop();\
        Nop();\
        Nop();\
        Nop();\
        Nop();\
        Nop();

void static sendLEDStatus(uint8_t g,uint8_t r,uint8_t b)
{
        _sendBit(r & 0x80)
        _sendBit(r & 0x40)
        _sendBit(r & 0x20)
        _sendBit(r & 0x10)
        _sendBit(r & 0x08)
        _sendBit(r & 0x04)
        _sendBit(r & 0x02)
        _sendBit(r & 0x01)
        _sendBit(g & 0x80)
        _sendBit(g & 0x40)
        _sendBit(g & 0x20)
        _sendBit(g & 0x10)
        _sendBit(g & 0x08)
        _sendBit(g & 0x04)
        _sendBit(g & 0x02)
        _sendBit(g & 0x01)
        _sendBit(b & 0x80)
        _sendBit(b & 0x40)
        _sendBit(b & 0x20)
        _sendBit(b & 0x10)
        _sendBit(b & 0x08)
        _sendBit(b & 0x04)
        _sendBit(b & 0x02)
        _sendBitEnd(b & 0x01)
}

void static sendLEDStatusHS(uint8_t g,uint8_t r,uint8_t b)
{
        PORTC |= 0b00000100;
        if(r & 0x80)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }   
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(r & 0x40)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(r & 0x20)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(r & 0x10)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(r & 0x08)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(r & 0x04)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(r & 0x02)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(r & 0x01)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();

       PORTC |= 0b00000100;
        if(g & 0x80)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }   
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(g & 0x40)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(g & 0x20)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(g & 0x10)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(g & 0x08)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(g & 0x04)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(g & 0x02)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(g & 0x01)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        
       PORTC |= 0b00000100;
        if(b & 0x80)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }   
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(b & 0x40)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(b & 0x20)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(b & 0x10)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(b & 0x08)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(b & 0x04)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(b & 0x02)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        PORTC |= 0b00000100;
        if(b & 0x01)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
            PORTC &= ~0b00000100;
        }    
        else
        {
            PORTC &= ~0b00000100;
            Nop();
            Nop();
            Nop();
        }
        //Nop();
        //Nop();
        //Nop();
        //Nop();
        //Nop();
        //Nop();
        //Nop();
        //Nop();
        //Nop();
}

static uint8_t r_matrix_buffer[3][8*32] =
{
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
},
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
},
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}
};

static uint8_t empty_pattern[40] = 
{
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00
};

static uint8_t full_pattern[40] = 
{
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00
};

static uint8_t dots_pattern[16] =
{
    0x00,0x00,
    0x01,0x01,
    0x01,0x01,
    0x00,0x00,
    0x00,0x00,
    0x01,0x01,
    0x01,0x01,
    0x00,0x00
};

static uint8_t dot_pattern[16] =
{
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x01,0x01,
    0x01,0x01
};

static uint8_t days_pattern[7][72] = 
{
{
    0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00, 0x00,
    0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x00,0x00,
    0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x00,
    0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x00,
    0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00
},    
{
    0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,
    0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,
    0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,
    0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,
    0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,
    0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x00,0x00,
    0x01,0x01,0x00,0x01,0x01,0x00,0x01,0x00,0x00,
    0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x00
},
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
},
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
},
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
},
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
},
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}
};

static uint8_t digits_pattern[10][32] =
{
{
    0x00,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01, 
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x00
},
{
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x01,0x01,
    0x00,0x01,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01
},
{
    0x00,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x01,0x00,
    0x00,0x01,0x00,0x00,
    0x01,0x00,0x00,0x00,
    0x01,0x01,0x01,0x01
},
{
    0x00,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x00,0x01,0x00,
    0x00,0x00,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x00
},
{
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x01,0x00,
    0x00,0x01,0x00,0x00,
    0x01,0x00,0x00,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x01,0x01,0x01,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01
},
{
    0x01,0x01,0x01,0x01,
    0x01,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,
    0x01,0x01,0x01,0x00,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x00
},
{
    0x00,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x00,
    0x01,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x00
},
{
    0x01,0x01,0x01,0x01,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x01,0x00,
    0x00,0x00,0x01,0x00,
    0x00,0x01,0x00,0x00,
    0x00,0x01,0x00,0x00,
    0x01,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00
},
{
    0x00,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x00
},
{
    0x00,0x01,0x01,0x00,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x01,
    0x00,0x00,0x00,0x01,
    0x01,0x00,0x00,0x01,
    0x00,0x01,0x01,0x00
}
};

void clear_display_memory(void)
{
    memset(&r_matrix_buffer, 0, sizeof(r_matrix_buffer));
}

uint8_t get_pixel_index(uint8_t row, uint8_t column)
{
    return (8*column-row+1-column%2*(9-2*row))-1;
}

void update_display_buffer_with_sign_buffer(uint8_t *disp, 
                                            uint8_t *sign, 
                                            uint8_t column, 
                                            uint8_t size_of_sign, 
                                            uint8_t value)
{
    uint8_t index = 0;
    
    for(uint8_t row_index=1; row_index<9; row_index++)
    {
        for(uint8_t column_index=column; column_index<column+size_of_sign; column_index++)
        {
            if(*(sign+index))
                *(disp+get_pixel_index(row_index, column_index)) = value;
            else
                *(disp+get_pixel_index(row_index, column_index)) = 0;
            index++;
        }
    }
}

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} colour_struct;

colour_struct cBlack = {0,0,0};
colour_struct cGreen = {0,1,0};
colour_struct cRed = {1,0,0};
colour_struct cBlue = {0,0,1};

void update_display_with_word(uint8_t column,uint8_t value, colour_struct *colour)
{
    update_display_buffer_with_sign_buffer(r_matrix_buffer[0],days_pattern[value],column, sizeof(days_pattern[value])>>3,colour->r);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[1],days_pattern[value],column, sizeof(days_pattern[value])>>3,colour->g);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[2],days_pattern[value],column, sizeof(days_pattern[value])>>3,colour->b);
}

void update_display_with_number(uint8_t column,uint8_t value, colour_struct *colour)
{
    update_display_buffer_with_sign_buffer(r_matrix_buffer[0],digits_pattern[value],column, sizeof(digits_pattern[value])>>3,colour->r);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[1],digits_pattern[value],column, sizeof(digits_pattern[value])>>3,colour->g);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[2],digits_pattern[value],column, sizeof(digits_pattern[value])>>3,colour->b);
}

void update_display_with_dots(uint8_t column, colour_struct *colour)
{
    update_display_buffer_with_sign_buffer(r_matrix_buffer[0],dots_pattern,column, sizeof(dots_pattern)>>3,colour->r);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[1],dots_pattern,column, sizeof(dots_pattern)>>3,colour->g);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[2],dots_pattern,column, sizeof(dots_pattern)>>3,colour->b);
}

void update_display_with_dot(uint8_t column, colour_struct *colour)
{
    update_display_buffer_with_sign_buffer(r_matrix_buffer[0],dot_pattern,column, sizeof(dot_pattern)>>3,colour->r);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[1],dot_pattern,column, sizeof(dot_pattern)>>3,colour->g);
    update_display_buffer_with_sign_buffer(r_matrix_buffer[2],dot_pattern,column, sizeof(dot_pattern)>>3,colour->b);
}

void update_display(uint8_t *update_flag,uint8_t wait_time)
{
    uint32_t y;
    uint16_t x;

    if(*update_flag)
    {
        for(x=0;x<(256);x++)
        {
            sendLEDStatus(r_matrix_buffer[0][x],
                            r_matrix_buffer[1][x],
                            r_matrix_buffer[2][x]);
        }
        for(y = 0; y <wait_time; y++);
        *update_flag = 0;
    }
}



void display_time(uint8_t *update_flag,time_struct_digits *time, uint8_t *dot_flag, colour_struct *digits, colour_struct *dots, uint8_t startPoint)
{    
    clear_display_memory();
    update_display_with_number(startPoint,time->hour_msn,digits);
    update_display_with_number(startPoint+5,time->hour_lsn,digits);
    update_display_with_number(startPoint+13,time->minute_msn,digits);
    update_display_with_number(startPoint+18,time->minute_lsn,digits);
    
    if(*dot_flag = 1 - *dot_flag)
    {
        update_display_with_dots(startPoint+10,dots);
    }
    else
    {
        update_display_with_dots(startPoint+10,&cBlack);
    }
    *update_flag = 1;
}

void display_date(uint8_t *update_flag,time_struct_digits *time, colour_struct *digits, colour_struct *dots, uint8_t startPoint)
{    
    clear_display_memory();
    update_display_with_word(startPoint,time->week,digits);
    update_display_with_number(startPoint+10,time->day_msn,dots);
    update_display_with_number(startPoint+15,time->day_lsn,dots);
    update_display_with_number(startPoint+23,time->month_msn,dots);
    update_display_with_number(startPoint+28,time->month_lsn,dots);  
    update_display_with_dot(startPoint+20,dots);
    *update_flag = 1;
}

void display_year(uint8_t *update_flag,time_struct_digits *time, colour_struct *digits, uint8_t startPoint)
{    
    clear_display_memory();
    update_display_with_number(startPoint,2,digits);
    update_display_with_number(startPoint+5,0,digits);
    update_display_with_number(startPoint+10,time->year_msn,digits);
    update_display_with_number(startPoint+15,time->year_lsn,digits);  
    *update_flag = 1;
}

void display_seconds(uint8_t *update_flag,time_struct_digits *time, colour_struct *digits, uint8_t startPoint)
{    
    clear_display_memory();
    update_display_with_number(startPoint,time->second_msn,digits);
    update_display_with_number(startPoint+5,time->second_lsn,digits);
    *update_flag = 1;
}

typedef  enum dataShowType {none=0, time, seconds, date, year, temp_in, temp_out, hum, press} dataShowType;

typedef enum buttonDescription {unpressed=0, pressed, left, right, pressed_left, pressed_right, pressed_long} buttonDescription;

buttonDescription evaluateButton(void)
{
    static uint8_t buttonStatus_old = 0;
    buttonDescription return_value = unpressed;

    //static uint8_t counter = 0;
    
    if(buttonStatus != buttonStatus_old)
    {
        if((buttonStatus & 0x20) && !(buttonStatus_old & 0x20))
        {
            return_value = pressed;
        }
        if(!(buttonStatus & 0x40) && (buttonStatus_old & 0x40))
        {
            if(PORTC & 0x80)
            {
                return_value = right;
            }
            else
            {
                return_value = left;
            }
        }
        buttonStatus_old = buttonStatus;
        
    }
    
    //rotationStatus = 0;    
    //buttonStatus = 0;
    
    return return_value;
}

dataShowType switch_data(buttonDescription button_status)
{
    static dataShowType currentStatus = none;
    if(currentStatus != none)
    {
        if(button_status == left)
        {
            if(currentStatus == time)
            {
                currentStatus = year;
            }
            else
            {
                currentStatus--;
            }
        }    
        if(button_status == right)
        {
            if(currentStatus == year)
            {
                currentStatus = time;
            }
            else
            {
                currentStatus++;
            }
        }
    }
    if(button_status == pressed)
    {
        if(currentStatus == none)
        {
            currentStatus = time;
        }
        else
        {
            currentStatus = none;
        }
    }
    return currentStatus;
}

void main(void)
{
    uint16_t old_systemTick;
    uint8_t sensor_readout_start;
    uint8_t update_flag = 1;

    uint8_t dots_flag = 0x00;
    dataShowType current_data_on_display = none;
    time_struct_digits timeBCD;
    
    ConfigureOscillator();
    InitApp();
    
    //DS1340_setTime(0x08,0x51,0x00);
    //DS1340_setDate(0, 0x31, 0x05, 0x18);
    
    sensor_readout_start = READOUT_ON;
    
    while(1) {        
        if(READOUT_ON == sensor_readout_start)
        {
            //update_flag = 1;
            //INTCON &= ~(0b00100000);        //disable interrupts from timer
            //OSCTUNEbits.PLLEN = 1;
            //while(!OSCCONbits.HFIOFS);
            update_display(&update_flag,200);
            //OSCTUNEbits.PLLEN = 0;
            //while(!OSCCONbits.HFIOFS);
            //INTCON |= 0b00100000;
            
         //   doDisplayFrame();   //send display data on change
            current_data_on_display = switch_data(evaluateButton());
            
            if(systemTick - old_systemTick > 0)
            {
                
                
                
                //every single tick, every 500ms
                
                if(systemTick & 0x0001)
                {
                    switch(current_data_on_display)
                    {
                        case time:
                        {
                            DS1340_getTime(&timeBCD);
                            display_time(&update_flag, &timeBCD, &dots_flag, &cGreen, &cBlue,6);
                        } break;
                        case seconds:
                        {
                            DS1340_getTime(&timeBCD);
                            display_seconds(&update_flag, &timeBCD, &cBlue,19);
                        } break;
                        case date:
                        {
                            DS1340_getTime(&timeBCD);
                            display_date(&update_flag, &timeBCD, &cBlue, &cRed,1);
                        } break;
                        case year:
                        {
                            DS1340_getTime(&timeBCD);
                            display_year(&update_flag, &timeBCD, &cBlue, 9);
                        } break;
                        default:
                            display_time(&update_flag, &timeBCD, &dots_flag, &cGreen, &cRed,6);
                            //current_data_on_display = time;
                    }
                    

                    
                    
                                        //every four signle ticks, every 500ms
               //     doSensorFrame();    //send current time and date
               //     doNM(0x01);
              //      doERRORS();
                }
                old_systemTick = systemTick;
            }
            else
            {
                //error, lost tick
            }
            //doCAN();
        }
        else
        {
            //do nothing
        }
    }
    

}

