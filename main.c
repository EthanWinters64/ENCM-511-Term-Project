
/*
 * File: main.c
 * Author: Nicholas Lam, Carter Marcelo, Ethan Winters
 * 
 * Created: September 25, 2022 
 */
#include "IOs.h"
#include "game.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "UART2.h"
#include "ADC.h"
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz: FRC means Fast RC oscillator
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = OFF  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE// Primary oscillator mode is disable
// WDT
#pragma config FWDTEN = OFF // WDT is off
#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected
//MCLR/RA5 CONTROL
#pragma config MCLRE = OFF // RA5 pin configured as input, MCLR reset on RA5 diabled
//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin
//JTAG FICD Register
#pragma config BKBUG = ON // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger
// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768

void main(void){
    NewClk(500);
    IOinit();
    InitUART2();
    while(1){
        Display2String(" Press any Button to Start Game       \r", 1);
        delay_ms(10000); // if nothing is pressed for 10 seconds, the game goes into deep sleep
        if(TMR2_FLAG == 1){
            Display2String(" Sleeping                                 \r", 1);
            Sleep();
            Display2String("Awake! -", 1);
        }
        uint16_t raw = do_ADC();
        int difficulty = (raw / 200);
        if(difficulty <= 0){ // this simply removes the 0 spot on the potentiometer from the difficulty calculations
            difficulty = 1;
        }
        Display2String(" Starting Game with Difficulty: ", 1);
        Display2Dec(difficulty);
        Display2String("\r", 1);
        delay_ms(2000);
        play(difficulty);
        Display2String(" Thanks for Playing!               \r", 1);
        delay_ms(2000);
        break;
    }
    return;
}