/* 
 * File:   TimeDelay.h
 * Author: cartermarcelo
 *
 * Created on October 13, 2022, 10:52 PM
 */
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#ifndef TIMEDELAY_H
#define	TIMEDELAY_H
#endif
#ifndef SCALER
#define SCALER 1.024
extern int TMR2_FLAG;
void delay_ms(uint16_t time_ms);
void  __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
#endif	/* TIMEDELAY_H */

