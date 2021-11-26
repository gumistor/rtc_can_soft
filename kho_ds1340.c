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

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "kho_ds1340.h"
#include "kho_i2c.h"

void DS1340_getConfig(void) {
    uint8_t DS1340_SET_ADDRESS[2] = {DS1340_WRITE, 0x07};
    uint8_t DS1340_REQUEST_DATA[1] = {DS1340_READ};
    uint8_t DS1340_DATA_RECEIVED[1] = {0xFF};

    I2C_SendAndReceive(DS1340_SET_ADDRESS,0x02, &DS1340_DATA_RECEIVED, 0x00);
    I2C_SendAndReceive(DS1340_REQUEST_DATA,0x01, &DS1340_DATA_RECEIVED, 0x01);
    
    DS1440_Config_byte = DS1340_DATA_RECEIVED[0];
}

void DS1340_getOscilator(void) {
    uint8_t DS1340_SET_ADDRESS[2] = {DS1340_WRITE, 0x09};
    uint8_t DS1340_REQUEST_DATA[1] = {DS1340_READ};
    uint8_t DS1340_DATA_RECEIVED[1] = {0xFF};

    I2C_SendAndReceive(DS1340_SET_ADDRESS,0x02, &DS1340_DATA_RECEIVED, 0x00);
    I2C_SendAndReceive(DS1340_REQUEST_DATA,0x01, &DS1340_DATA_RECEIVED, 0x01);

    DS1440_Oscilator_byte = DS1340_DATA_RECEIVED[0];
}

void DS1340_getTime(void) {
    uint8_t DS1340_SET_ADDRESS[2] = {DS1340_WRITE, 0x00};
    uint8_t DS1340_REQUEST_DATA[1] = {DS1340_READ};
    uint8_t DS1340_DATA_RECEIVED[1] = {0xFF};

    I2C_SendAndReceive(DS1340_SET_ADDRESS,0x02, &DS1340_DATA_RECEIVED, 0x00);
    I2C_SendAndReceive(DS1340_REQUEST_DATA,0x01, (uint8_t *)&timeDateStucture, 0x07);

    timeDateStucture.second     &= 0x7F;
    timeDateStucture.minute     &= 0x7F;
    timeDateStucture.hour       &= 0x3F;
    timeDateStucture.week       &= 0x07;
    timeDateStucture.day        &= 0x3F;
    timeDateStucture.month      &= 0x1F;
    timeDateStucture.year       &= 0xFF;

    second_lsn  = timeDateStucture.second&0x0F;
    second_msn  = timeDateStucture.second>>4&0x0F;
    minute_lsn  = timeDateStucture.minute&0x0F;
    minute_msn  = timeDateStucture.minute>>4&0x0F;
    hour_lsn    = timeDateStucture.hour&0x0F;
    hour_msn    = timeDateStucture.hour>>4&0x0F;
}

void DS1340_setConfig(uint8_t config, uint8_t timeout) {
    uint8_t DS1340_SET_DATA_ADDRESS[3] = {DS1340_WRITE, 0x07, 0x00};
    uint8_t DS1340_DATA_RECEIVED[1] = {0xFF};

    DS1340_SET_DATA_ADDRESS[2] = config;

    config &= 0x3F;
    config |= 0x40;

    EEPROM_WRITE(0x02, config);
    EEPROM_WRITE(0x03, timeout);

    I2C_SendAndReceive(DS1340_SET_DATA_ADDRESS,0x03, &DS1340_DATA_RECEIVED, 0x00);

    displayNoTimeTimeout_value = timeout;
}

void DS1340_setOscilator(uint8_t config) {
    uint8_t DS1340_SET_DATA_ADDRESS[3] = {DS1340_WRITE, 0x00, 0x00};
    uint8_t DS1340_DATA_RECEIVED[1] = {0xFF};

    DS1340_SET_DATA_ADDRESS[2] = config;

    I2C_SendAndReceive(DS1340_SET_DATA_ADDRESS,0x03, &DS1340_DATA_RECEIVED, 0x00);
}

void DS1340_setTime(uint8_t hour, uint8_t minute, uint8_t second) {
    uint8_t DS1340_SET_DATA[5] = {DS1340_WRITE, 0x00, 0x00, 0x00, 0x00};
    uint8_t DS1340_DATA_RECEIVED[1] = {0xFF};

    DS1340_SET_DATA[2] = 0x7F & second;
    DS1340_SET_DATA[3] = 0x7F & minute;
    DS1340_SET_DATA[4] = 0x3F & hour;

    I2C_SendAndReceive(DS1340_SET_DATA,0x05, &DS1340_DATA_RECEIVED, 0x00);
}

void DS1340_setDate(uint8_t week, uint8_t day, uint8_t month, uint8_t year) {
    uint8_t DS1340_SET_DATA[6] = {DS1340_WRITE, 0x03, 0x00, 0x00, 0x00, 0x00};
    uint8_t DS1340_DATA_RECEIVED[1] = {0xFF};

    DS1340_SET_DATA[2] = 0x07 & week;
    DS1340_SET_DATA[3] = 0x3F & day;
    DS1340_SET_DATA[4] = 0x1F & month;
    DS1340_SET_DATA[5] = year;

    I2C_SendAndReceive(DS1340_SET_DATA,0x06, &DS1340_DATA_RECEIVED, 0x00);
}

uint8_t DS1340_checkTime(uint8_t hour, uint8_t minute, uint8_t second,
        uint8_t week , uint8_t day, uint8_t month, uint8_t year)
{
    uint8_t return_value;

    return_value = 1;

    if(timeDateStucture.hour != hour && hour != 0xFF)
    {
        return_value = 0;
    }
    if(timeDateStucture.minute != minute && minute != 0xFF)
    {
        return_value = 0;
    }
    if(timeDateStucture.second != second && second != 0xFF)
    {
        return_value = 0;
    }
    if(timeDateStucture.week != week && week != 0xFF)
    {
        return_value = 0;
    }
    if(timeDateStucture.day != day && day != 0xFF)
    {
        return_value = 0;
    }
    if(timeDateStucture.month != month && month != 0xFF)
    {
        return_value = 0;
    }
    if(timeDateStucture.year != year && year != 0xFF)
    {
        return_value = 0;
    }

    return return_value;
}

uint8_t DS1340_init(void)
{
    uint8_t storedConfigData, timeout;

    sensor_readout_start = READOUT_ON;

    storedConfigData = EEPROM_READ(0x02);
    timeout = EEPROM_READ(0x03);

    DS1340_getConfig();
    if(DS1440_Config_byte != storedConfigData)
    {
        DS1440_Config_byte = storedConfigData;
        DS1340_setConfig(DS1440_Config_byte, timeout);
    }
    DS1340_getOscilator();
    if(!(DS1440_Oscilator_byte && 0x40))
    {
        DS1440_Oscilator_byte &= 0x7F;
        DS1340_setOscilator(DS1440_Oscilator_byte);
    }

    displayTime = DISPLAY_TIME;
    displayNoTimeTimeout_value = timeout;
    //displayNoTimeTimeout = displayNoTimeTimeout_value;

    return sensor_readout_start;
}