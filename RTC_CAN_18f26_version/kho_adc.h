/* 
 * File:   kho_adc.h
 * Author: Krzysztof
 *
 * Created on 5 luty 2017, 17:51
 */

#ifndef KHO_ADC_H
#define	KHO_ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t dataADCalculationFlag;
uint8_t dataADCaquasitionFlag;
int16_t dataADC;

uint8_t initADC(void);
void adcStartAquasition(void);


#ifdef	__cplusplus
}
#endif

#endif	/* KHO_ADC_H */

