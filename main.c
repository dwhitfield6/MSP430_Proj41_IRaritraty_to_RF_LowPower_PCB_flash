#include <msp430.h>
#include "functions.h"

#define BatVoltage BIT0
#define Switch1 BIT1
#define LED0 BIT3
#define LED1 BIT2
#define IRrec BIT4
#define IRrec1 BIT5
#define TX BIT6

#define ArraySize 60

//#define CHNL_E



#ifdef CHNL_E
const unsigned char TXdataE[12] = {0,2,2,2,2,2,0,0,1,0,0,0}; // channel E
#else
const unsigned char TXdataD[12] = {2,2,2,2,2,2,0,0,1,0,0,0}; // channel D (A1 A0 x x x x x x)
#endif

unsigned char receive[ArraySize];
char i=0;
int timeout =0;

unsigned char differences =ArraySize;
int amount =2;

int ledcount= 1000;
unsigned char hold =0;
unsigned char holdArrayamount =0;
int *addr = (int*)0x1040; // Address of the flash memory segment starting
int *addrArray = (int*)receive; // Address of the flash memory segment starting

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	  P1DIR |= (LED1 + LED0 + TX);                            // Set all pins but RXD to output
	  P1OUT &= ~(LED1 + LED0 + TX);
	  P1REN |= Switch1;
	  P1OUT |= Switch1;
	  P1IE |= IRrec; // P1.3 interrupt enabled
	  P1IES &= ~IRrec; // P1.3 interrupt enabled
	  P1IFG &= ~IRrec; // P1.3 IFG cleared
	  P1IE |= IRrec1; // P1.3 interrupt enabled
	  P1IES |= IRrec1; // P1.3 interrupt enabled
	  P1IFG &= ~IRrec1; // P1.3 IFG cleared

	  TA0CTL = TASSEL_2 + MC_2 + ID_3;           // SMCLK/8, upmode

	  _BIS_SR(LPM4_bits + GIE); // Enter LPM3 w/interrupt

	  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts
	while(1)
	{
		_delay_cycles(100);

timeout++;
if(timeout == 1000)
{
	TA0CTL |= TACLR;
	hold = readSwitchP1(Switch1);

	holdArrayamount =i;

	if(hold == 1)
	{
			ArrayCopy(receive,(int*)addr, holdArrayamount);
			BlinkLED(LED1);
	}
	differences = ArrayCompareFlash(addrArray, addr, amount, ArraySize);
	_delay_cycles(10);
//	differences2 = ArrayCompare(receive, HOLDreceive, amount, holdArrayamount);
	_delay_cycles(10);
	resetBuffer(receive, ArraySize);
	_delay_cycles(10);
	i=0;
	if(differences == 0)
	{

		P1OUT |= LED1;
	    P1OUT &= ~LED0;
#ifdef CHNL_E

		send_rf(TX, TXdataE);
		_delay_cycles(1000);
		send_rf(TX, TXdataE);
		_delay_cycles(1000);
		send_rf(TX, TXdataE);
		_delay_cycles(1000);
		send_rf(TX, TXdataE);

#else

		send_rf(TX, TXdataD);
		_delay_cycles(1000);
		send_rf(TX, TXdataD);
		_delay_cycles(1000);
		send_rf(TX, TXdataD);
		_delay_cycles(1000);
		send_rf(TX, TXdataD);
#endif


	}
	else
	{
		P1OUT |= LED0;
		P1OUT &= ~LED1;
	}
	ledcount =0;
}
if(timeout >1050)
{
timeout =1050;
}



ledcount++;
if(ledcount == 1000)
{
	P1OUT &= ~(LED0 + LED1);
	_BIS_SR(LPM4_bits + GIE); // Enter LPM3 w/interrupt
}
if(ledcount > 1050)
{
ledcount = 1050;
}


}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	_BIC_SR(LPM4_EXIT); // wake up from low power mode
	if(i>=2)
	{
    receive[i-2] = (TAR)>>3;
	}
	if((i-2) < (ArraySize -1))
			{
			i++;
			}

  TA0CTL |= TACLR;
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IFG &= ~IRrec1; // P1.3 IFG cleared
  timeout=0;
}

