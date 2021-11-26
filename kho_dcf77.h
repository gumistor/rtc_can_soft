/* 
 * File:   kho_dcf77.h
 * Author: Krzysztof
 *
 * Created on 5 stycze? 2017, 20:28
 */

#ifndef KHO_DCF77_H
#define	KHO_DCF77_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_SYNCH_ATTEMPTS  0x1F
#define MAX_SMALL_ATTEMPTS  0x1FFFFF

uint8_t timeCounter;
uint8_t counterEnabled;
uint8_t startSynch;
uint8_t strobeDetected;
uint8_t dcf77Synchronized;

uint16_t checkStatus;

volatile uint8_t isRaise;
volatile uint8_t old_systemTick_diff;

void dcf77_init(void);
void dcf77_dateTimeReadout(void);

#ifdef	__cplusplus
}
#endif

#endif	/* KHO_DCF77_H */

