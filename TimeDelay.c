#include "TimeDelay.h"
#include "IOs.h"

int TMR2_FLAG = 0;
void  __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    TMR2_FLAG = 1;
    IFS0bits.T2IF = 0; //interrupt
    T2CONbits.TON = 0;
    TMR2=0;
    return;
}
void delay_ms(uint16_t time_ms){
    //configure Timer2
    TMR2_FLAG = 0;
    PR2 = SCALER*time_ms;
    T2CONbits.TON = 1;
    TMR2=0;
    Idle();
    return;
}
void wait_for_press(uint16_t time_ms){
    TMR2_FLAG = 0;
    PR2 = SCALER*time_ms;
    T2CONbits.TON = 1;
    Idle();
    return;
}