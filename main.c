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
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "kho_ds1340.h"
#include "kho_can.h"
#include "kho_dcf77.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void doSensorFrame(void)
{
    //sendTXbuffer(TX0BUFFER, hour_full,minute_full,second_full,0xFF,day_of_month_full,year_full,month_full,day_full,0x08);

    sendTXbuffer(TX0BUFFER, timeDateStucture.hour, timeDateStucture.minute,
            timeDateStucture.second,0xFF,
            timeDateStucture.week, timeDateStucture.day, timeDateStucture.month,
            timeDateStucture.year,0x08);
}

void doDisplayFrame(void)
{
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
                    case 0x00: //setup time in DS1340 RTC
                        DS1340_setTime(CAN_data_received.CAN_data[1],
                                CAN_data_received.CAN_data[2],
                                CAN_data_received.CAN_data[3]);
                        break;
                    case 0x01: //setup date in DS1340 RTC
                        DS1340_setDate(CAN_data_received.CAN_data[1],
                                CAN_data_received.CAN_data[2],
                                CAN_data_received.CAN_data[3],
                                CAN_data_received.CAN_data[4]);
                        break;
                    case 0x02: //setup config byte of DS1340
                        DS1340_setConfig(CAN_data_received.CAN_data[1],
                                CAN_data_received.CAN_data[2]);
                        break;
                    case 0x03:  // setup oscilator to work of DS1340
                        DS1340_setOscilator(0x00);
                        break;
                    case 0x04:
                        if(displayTime == DISPLAY_TIME)
                        {
                            displayTime = DISPLAY_DATE;
                            displayNoTimeTimeout = displayNoTimeTimeout_value;
                        }
                        else if(displayTime == DISPLAY_DATE)
                        {
                            displayTime = DISPLAY_WEEK;
                            displayNoTimeTimeout = displayNoTimeTimeout_value;
                        }
                        else
                        {
                            displayTime = DISPLAY_TIME;
                        }
                        break;
                    case 0xFE:  // setup time after dcf77
                        dcf77_dateTimeReadout();
                        configureTXbuffer(TX2BUFFER, CAN_ADDR_CONFIG_RES         | can_node_class<<8 |    device_id,1);
                        sendTXbuffer(TX2BUFFER, 0xEE,checkStatus>>8,checkStatus,0x00,0x00,0x00,0x00,0x00,0x03);
                        break;
                    case 0xFF:
                        EEPROM_WRITE(0xFF,0xFF);
                        for(i=0; i<2000; i++);
                        Reset();
                        break;
                    default:    // no action
                        break;

                    }
                    //configureTXbuffer(TX2BUFFER, CAN_ADDR_CONFIG_RES         | can_node_class<<8 |    device_id,1);
                    //sendTXbuffer(TX2BUFFER, 0x02, CAN_data_received.CAN_address>>24,CAN_data_received.CAN_address>>16,CAN_data_received.CAN_address>>8,CAN_data_received.CAN_address,CAN_data_received.CAN_DLC,
                    //CAN_data_received.CAN_data[0],CAN_data_received.CAN_data[1],0x08);
                break;
            case CAN_ADDR_REQUEST:

            break;
            case CAN_ADDR_RESPONSE:

            break;
            case CAN_ADDR_NM:
                //nm frame received
                //clear wait to sleep frame
                //configureTXbuffer(TX2BUFFER, CAN_ADDR_CONFIG_RES         | can_node_class<<8 |    device_id,1);
                //sendTXbuffer(TX2BUFFER, 0x07, CAN_data_received.CAN_address>>24,CAN_data_received.CAN_address>>16,CAN_data_received.CAN_address>>8,CAN_data_received.CAN_address,CAN_data_received.CAN_DLC,
                //CAN_data_received.CAN_data[0],CAN_data_received.CAN_data[1],0x08);
            break;
            default:
                ;
        }
        CAN_data_received_flag = CAN_DATA_EMPTY;
    }
}

void main(void)
{
    uint16_t old_systemTick;
    //initialisation
    ConfigureOscillator();
    InitApp();
    DS1340_init();

    while(1) {
        if(READOUT_ON == sensor_readout_start)
        {
            doERRORS();
            doDisplayFrame();   //send display data on change
            
            if(systemTick - old_systemTick > 0)
            {
                //every single tick, every 125ms
              //  dcf77_countTime();
              //  dcf77_startSynch();
              //  dcf77_waitSynchStrobe();
                if(systemTick & 0x0001)
                {
                    //every two signle ticks, every 250ms
                    DS1340_getTime();   //time readout
                }
                if((systemTick & 0x0003) == 0x0002)
                {
                    //every four signle ticks, every 500ms
                    doSensorFrame();    //send current time and date
                }
                if((systemTick & 0x0007) == 0x0004)
                {
                    doNM(0x01);
                    if(displayNoTimeTimeout)
                    {
                        displayNoTimeTimeout--;
                    }
                    else
                    {
                        displayTime = DISPLAY_TIME;
                    }
                    //every eight signle ticks, every 10000ms
                    //if(DS1340_checkTime(0xFF, 0xFF, 0x56, 0xFF, 0xFF, 0xFF, 0xFF)||
                    //        DS1340_checkTime(0xFF, 0xFF, 0x26, 0xFF, 0xFF, 0xFF, 0xFF))
                    //{
                        //dcf77_dateTimeReadout();
                        //if(checkStatus)
                        //{
                            
                        //}
                    //}
                }
                old_systemTick = systemTick;
            }
            else
            {
                //error, lost tick
            }
            doCAN();
        }
        else
        {
            //do nothing
        }
    }
}

