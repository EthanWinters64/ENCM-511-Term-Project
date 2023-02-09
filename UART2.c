#include "xc.h"
#include "UART2.h"
#include "string.h"
unsigned int clkval;

void InitUART2(void){ // initializes the UART port
    TRISBbits.TRISB0=0; // Set U2TX/RB0/pin 4 as UART TX
	TRISBbits.TRISB1=1; // Set U2RX/RB1/pin 5 as UART RX
	LATBbits.LATB0=1;

	// configure UART mode i.e Parity, char length etc 
    U2MODE = 0b1000000000001000;
/*    
	U2MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
	U2MODEbits.USIDL = 0;	// Bit13 Continue in Idle
	U2MODEbits.IREN = 0;	// Bit12 No IR translation
	U2MODEbits.RTSMD = 0;	// Bit11 Simplex Mode
	U2MODEbits.UEN = 0;		// Bits8,9 TX,RX enabled, CTS,RTS not
	U2MODEbits.WAKE = 0;	// Bit7 Interrupt not generated on U2RX pin in sleep
	U2MODEbits.LPBACK = 0;	// Bit6 No Loop Back
	U2MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
	U2MODEbits.RXINV = 0;	// Bit4 IdleState = 1
	U2MODEbits.BRGH = 1;	// Bit3 4 clocks per bit period - Hi speed mode
	U2MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
	U2MODEbits.STSEL = 0;	// Bit0 One Stop Bit
 */
    //configure baud rate based on sys clock
	if (OSCCONbits.COSC == 0b110)
	{
		U2BRG = 12;	// gives a baud rate of 4807.7 Baud with 500kHz clock; Set Baud to 4800 on realterm
	}
	else if (OSCCONbits.COSC == 0b101)
	{
		U2BRG = 12;	// gives a baud rate of 300 Baud with 32kHz clock; set Baud to 300 on realterm
	}
	else if (OSCCONbits.COSC == 0b000)
	{
		U2BRG=103;	// gives a baud rate of 9600 with 8MHz clock; set Baud to 9600 on real term
	}
	// Initialize UART Status reg - Tx interrupt control
	U2STA = 0b0010000000000000;
    
    /*
    U2STAbits.UTXISEL1 = 1;	//Bit15 Int when Char is transferred (1/2 config!)
    U2STAbits.UTXISEL0 = 1;	//Generate interrupt with last character shifted out of U2TXREG buffer
	U2STAbits.UTXINV = 0;	//Bit14 N/A, IRDA config
	U2STAbits.UTXBRK = 0;	//Bit11 Disabled
	U2STAbits.UTXEN = 0;	//Bit10 TX pins controlled by periph
	U2STAbits.UTXBF = 0;	//Bit9 *Read Only Bit*
	U2STAbits.TRMT = 0;		//Bit8 *Read Only bit*
	U2STAbits.URXISEL = 0;	//Bits6,7 Int. on character recieved
	U2STAbits.ADDEN = 0;	//Bit5 Address Detect Disabled
	U2STAbits.RIDLE = 0;	//Bit4 *Read Only Bit*
	U2STAbits.PERR = 0;		//Bit3 *Read Only Bit*
	U2STAbits.FERR = 0;		//Bit2 *Read Only Bit*
	U2STAbits.OERR = 0;		//Bit1 *Read Only Bit*
	U2STAbits.URXDA = 0;	//Bit0 *Read Only Bit*
    */
//    //Configure Interrupts (for Tx)
//    IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
//    IPC7bits.U2TXIP = 3; // UART2 TX interrupt has interrupt priority 3-4th highest priority
//    IEC1bits.U2TXIE = 1;	// Disable Transmit Interrupts

    U2MODEbits.UARTEN = 1;
	U2STAbits.UTXEN = 1;
    
	return;
}
void XmitUART2(char CharNum, unsigned int repeatNo)
{	
   // InitUART2();	//Initialize UART2 module at start of main()
   
	while(repeatNo!=0) 
	{
		while(U2STAbits.UTXBF==1)	//Just loop here till the FIFO buffers have room for one more entry
		{
			Idle();
		}	
		U2TXREG=CharNum;	//Move Data to be displayed in UART FIFO buffer
		repeatNo--;
	}
	while(U2STAbits.TRMT==0)	//Turn off UART2 upon transmission of last character; also can be Verified in interrupt subroutine U2TXInterrupt()
	{
		
	}
	// U2MODEbits.UARTEN = 0;	// turn off UART module to save power - OPTIONAL

	return;
}

void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void) { // an interrupt service routine for UART TX
	IFS1bits.U2TXIF = 0;
}

void Display2Hex(unsigned int Num){
    char count;
    char buff = 0x00;
    XmitUART2(' ',1);  // Output the initial space
    XmitUART2('0',1);  
    XmitUART2('x',1);  // Output the initial syntax for a Hex value
    for(count = 3; count >= 0; count--){
        buff = ((Num >> (4*count)) & 0x000F);
        if(buff >= 0x0A){ // adjust for higher hex values (A-F)/(10-15)
            buff = buff +0x37;
        }
        else{ // adjust for smaller values for hex (0-9)
            buff = buff +0x30; 
        }
        XmitUART2(buff, 1); // output the buffer containing the hex value
    }
    XmitUART2(' ', 1); // output a space to finish the stream
    Num = 0x0000; // clear the input
    return;
}

void Display2Hex32(unsigned long int LongNum){
    char count;
    char buff = 0x00;
    XmitUART2(' ',1);  // Output the initial space
    XmitUART2('0',1);  
    XmitUART2('x',1);  // Output the initial syntax for a Hex value
    for(count = 7; count >= 0; count--){
        buff = ((LongNum >> (4*count)) & 0x000F);
        if(buff >= 0x0A){ // adjust for higher hex values (A-F)/(10-15)
            buff = buff +0x37;
        }
        else{ // adjust for smaller values for hex (0-9)
            buff = buff +0x30; 
        }
        XmitUART2(buff, 1); // output the buffer containing the hex value
    }
    XmitUART2(' ', 1); // output a space to finish the stream
    LongNum = 0x0000; // clear the input
    return;
}

void Display2Dec(unsigned int Num){
    char remain[5] = {0,0,0,0,0}; //use this array to store remainders
    char remain1;
    uint8_t i = 0;
    for(i = 0; i < 5; i++){
        remain[i] = (Num % 10);
        Num = (Num / 10);
    }
    XmitUART2(' ', 1); // output a space to start the stream
    for(i = 5; i>0; i--)
    {
        remain1 = remain[i-1];
        XmitUART2((remain1 + 48),1);
    }
    XmitUART2(' ',1);
    
    return;
}

void Display2String(char *str, int repeatnum) 
{
    unsigned int i;
    unsigned int j;
   // XmitUART2(0x0A,2);  //LF
   // XmitUART2(0x0D,1);  //CR 
    for(j=0; j< repeatnum; j++){
        for (i=0; i<= strlen(str); i++)
        {
        XmitUART2(str[i], 1);
        }
    // XmitUART2(0x0A,2);  //LF
    // XmitUART2(0x0D,1);  //CR 
    }
    return;
}