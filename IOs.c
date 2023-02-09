#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"

//Authors: Nicholas Lam, Carter Marcelo, Ethan Winters
//// CONFIGURATION BITS - PRE-PROCESSOR DIRECTIVES ////
// Code protection
int CN0flag = 0;
int CN1flag = 0;
int CN2flag = 0;

//Need to assign unique value to 
void CLKinit(){
    //ensures no clock division set
    CLKDIVbits.RCDIV0=0;
    CLKDIVbits.RCDIV1=0;
    CLKDIVbits.RCDIV2=0; 
}
void REFOinit(){
    REFOCONbits.ROEN = 0; // Ref oscillator is enabled
    REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep
    REFOCONbits.ROSEL = 1; // Output base clock showing clock switching
    REFOCONbits.RODIV = 0;
}
void T2init(){
    T2CONbits.TCS=0; //use primary oscillators
    T2CONbits.T32=0;//T2 & T3 are 16 bit timers
    T2CONbits.TCKPS=0b11; //pre-scaler set to 1:256
    T2CONbits.TSIDL=0; // Allow timer to increment while in idle
}
void IRinit(){
    SRbits.IPL=0; //set CPU interrupt priority to 0
    IEC0bits.T2IE = 1; //enable Timer 2 interrupts
    IEC1bits.CNIE = 1; //enable Input Change Notification Interrupts;
    IEC4bits.HLVDIE = 1;    
    IPC1bits.T2IP = 6; //set Timer 2 Interrupt flag to highest priority (7)
    IPC4bits.CNIP = 7;
    INTCON1bits.NSTDIS=1; //disable nested interrupt
}
void IOinit(){
    CLKinit();
    REFOinit();
    IRinit();
    T2init();
    TRISAbits.TRISA2 = 1; //Setting input pin : RA2 (first button)
    TRISAbits.TRISA4 = 1; //Setting input pin : RA4 (third button)
    TRISBbits.TRISB4 = 1; //Setting input pin : RB4 (second button)
    TRISBbits.TRISB8 = 0; //Setting output pin: RB8 (LED0)
    TRISBbits.TRISB7 = 0; //Setting output pin: RB7 (LED1)
    TRISBbits.TRISB9 = 0; //Setting output pin: RB9 (LED2)
    //IO Initializations
    CNPU2bits.CN30PUE = 1; //Set input pins up
    CNPU1bits.CN0PUE = 1;      
    CNPU1bits.CN1PUE = 1; 
    AD1PCFG = 0xFFFF;  // Turn all analog pins to digital
    
    
    CNEN2bits.CN30IE = 1; // enable CN for RA2
    CNEN1bits.CN0IE = 1; // enable CN for RA4
    CNEN1bits.CN1IE = 1;
    
    IPC4bits.CNIP = 6; // set interrupt priority
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
   
}


void IOcheck(){
    if(BUTTON0 == 0){
        //CN0flag = 1;
    }
    else if(BUTTON1 == 0){
        //CN1flag = 1;
    }
    else if(BUTTON2 == 0){
        //CN2flag = 1;
    }
}

void  __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    if((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)){
        CN0flag = 1;
        CN1flag = 0;
        CN2flag = 0;
    }
    
    else if((PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 0) && (PORTAbits.RA2 == 1)){
        CN0flag = 0;
        CN1flag = 1;
        CN2flag = 0;
    }
    
    else if((PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 0)){
        CN0flag = 0;
        CN1flag = 0;
        CN2flag = 1;
    }
   
    IFS1bits.CNIF=0;
}


