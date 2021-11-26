/* 
 * File:   kho_gp2y10.h
 * Author: Krzysztof
 *
 * Created on 5 luty 2017, 17:48
 */

#ifndef KHO_GP2Y10_H
#define	KHO_GP2Y10_H

#ifdef	__cplusplus
extern "C" {
#endif

uint16_t gp2y10_pm100_concentration;

uint8_t initGp2y10(void);
void gp2y10_gather_data(void);

int16_t gp2y10_pm100_concentration_table[128];
int16_t gp2y10_pm100_zeroValue;
uint8_t gp2y10_pm100_concentration_table_index;

enum sensorStates {gp2_idle, gp2_prepareData, gp2_convertData, gp2_wait};

enum sensorStates sensorState;

#ifdef	__cplusplus
}
#endif

#endif	/* KHO_GP2Y10_H */

