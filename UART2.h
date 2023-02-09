/* 
 * File:   UART2.h
 * Author: EthanWinters
 *
 * Created on October 27, 2022, 11:14 AM
 */

#ifndef UART2_H
#define	UART2_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void InitUART2(void);
void XmitUART2(char, unsigned int);

void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void); 

void Display2Hex(unsigned int); // to output ints in base 16
void Display2Hex32(unsigned long int); // to output long ints in base 16
void Display2Dec(unsigned int); // to output ints in base 10
void Display2String(char*, int); // to output words and/or letters

#endif	/* UART2_H */

