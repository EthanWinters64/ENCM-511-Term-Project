#include "TimeDelay.h"
#ifndef BUTTON0
#define BUTTON0 ~PORTAbits.RA4
#endif
#ifndef BUTTON1
#define BUTTON1 ~PORTBbits.RB4
#endif
#ifndef BUTTON2
#define BUTTON2 ~PORTAbits.RA2
#endif
#ifndef LED0
#define LED0 LATBbits.LATB8 
#endif
#ifndef LED1 
#define LED1 LATBbits.LATB7
#endif
#ifndef LED2
#define LED2 LATBbits.LATB9
#endif
#ifndef IOS_H
#define	IOS_H
int buttonPress();
void IOinit();
void IOcheck();
void  __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);
#endif	/* IOS_H */

