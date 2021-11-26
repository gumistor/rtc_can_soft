/* 
 * File:   sensorsFunctions.h
 * Author: Krzysztof
 *
 * Created on 29 stycze? 2016, 20:09
 */

#ifndef SENSORSFUNCTIONS_H
#define	SENSORSFUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

//Address of AD converter - needed to get T1,D1 values
#define DS1340_I2C_ID        0x68 << 1

#define DS1340_WRITE    DS1340_I2C_ID | 0x00
#define DS1340_READ     DS1340_I2C_ID | 0x01


#define READOUT_SLEEP   0x00
#define READOUT_ON      0x01
#define READOUT_OFF     0x02
#define READOUT_ERROR   0x03

#define DISPLAY_TIME    0x00
#define DISPLAY_DATE    0x01
#define DISPLAY_WEEK    0x02

uint8_t sensor_readout_start = READOUT_ON;

struct timeDateStucture
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t week;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} timeDateStucture;

typedef struct 
{
    uint8_t second_lsn;
    uint8_t second_msn;
    uint8_t minute_lsn;
    uint8_t minute_msn;
    uint8_t hour_lsn;
    uint8_t hour_msn;
    uint8_t day_lsn;
    uint8_t day_msn;
    uint8_t month_lsn;
    uint8_t month_msn;
    uint8_t year_lsn;
    uint8_t year_msn;
    uint8_t week;
} time_struct_digits;

volatile uint8_t DS1440_Config_byte;
volatile uint8_t DS1440_Oscilator_byte;

volatile uint8_t second_lsn;
volatile uint8_t second_msn;
volatile uint8_t minute_lsn;
volatile uint8_t minute_msn;
volatile uint8_t hour_lsn;
volatile uint8_t hour_msn;

volatile uint8_t displayTime;

volatile uint8_t displayNoTimeTimeout;
volatile uint8_t displayNoTimeTimeout_value = 5;
//volatile uint8_t second_full;
//volatile uint8_t minute_full;
//volatile uint8_t hour_full;

//volatile uint8_t day_of_month_full;
//volatile uint8_t day_full;
//volatile uint8_t month_full;
//volatile uint8_t year_full;

void DS1340_getConfig(void);
void DS1340_getOscilator(void);
void DS1340_getTime(time_struct_digits *);

void DS1340_setConfig(uint8_t, uint8_t);
void DS1340_setOscilator(uint8_t);
void DS1340_setTime(uint8_t, uint8_t, uint8_t);
void DS1340_setDate(uint8_t, uint8_t, uint8_t, uint8_t);

uint8_t DS1340_checkTime(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

uint8_t DS1340_init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SENSORSFUNCTIONS_H */

