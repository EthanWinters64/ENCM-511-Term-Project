#include "xc.h"
#include "ADC.h"

uint16_t do_ADC(void){
    uint16_t value; // use this 16-bit register to hold the converted digital output ADC1BUFO
    AD1CON1bits.ADON = 1; // turn on the ADC module
    AD1CON1bits.FORM1 = 0; 
    AD1CON1bits.FORM0 = 0; // set the output format to int
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 0; 
    
    AD1CON2bits.VCFG = 0b000; // selects AVDD, AVSS as Vref
    AD1CON2bits.CSCNA = 0; 
    AD1CON2bits.BUFM = 0; 
    AD1CON2bits.ALTS = 0; 
    
    AD1CON3bits.ADRC = 0; // use system clock
    AD1CON3bits.SAMC = 0b11000; 
    AD1CON3bits.ADCS = 0b11110;
    
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SA = 0b0101;
    
    AD1PCFGbits.PCFG5 = 0; 
    
    AD1CON1bits.SAMP = 1; // start sampling
    while(AD1CON1bits.DONE == 0){ // wait
    }
    value = ADC1BUF0;
    AD1CON1bits.SAMP = 0; // stop sampling
    AD1CON1bits.ADON = 0; // turn off the ADC module
    return(value);
}
