#include "functions.h"
#include <msp430.h>

extern int *addr;
extern int *addr2;

void resetBuffer(unsigned  char* Array, unsigned char amount)
{
	int i ;
	for(i =0; i<(amount -1);i++)
	{
		Array[i] =0;
	}
}

void send_rf(unsigned char pin, const unsigned char* data)
{
unsigned char i=0;

for(i=0;i<12;i++)
{
if(data[i] == 0)
{
	zero(pin);
}
else if(data[i] ==1)
{
	one(pin);
}
else
{
	f(pin);
}

}

P1OUT |= pin;
_delay_cycles(250);
P1OUT &= ~pin;
_delay_cycles(7000);
}

void zero(unsigned char pin)
{
	P1OUT |= pin;
	_delay_cycles(250);
	P1OUT &= ~pin;
	_delay_cycles(690);
	P1OUT |= pin;
	_delay_cycles(250);
	P1OUT &= ~pin;
	_delay_cycles(690);
}
void one(unsigned char pin)
{
	P1OUT |= pin;
	_delay_cycles(810);
	P1OUT &= ~pin;
	_delay_cycles(210);
	P1OUT |= pin;
	_delay_cycles(810);
	P1OUT &= ~pin;
	_delay_cycles(210);
}
void f(unsigned char pin)
{
	P1OUT |= pin;
	_delay_cycles(250);
	P1OUT &= ~pin;
	_delay_cycles(700);
	P1OUT |= pin;
	_delay_cycles(810);
	P1OUT &= ~pin;
	_delay_cycles(200);
}

unsigned char ArrayCompareFlash(int* Array1,int* Array2, int amount, char characters)
{
	int i, *address;
	int oneN =0;
	int twoN=0;
	int oneCN =0;
	int twoCN=0;
	unsigned char differences;
	address = Array2;
	differences=0;
	i=0;
	while(1)
	{
		_delay_cycles(1);
		if(!(i%2))
		{
		oneN = (*address & 0xFF00)>>0x8;
		oneCN =  (*Array1 & 0xFF00)>>0x8;
        if(oneN == 0xFF)
        {
        	if(i < 4)
        	{
        	differences = 20;
        	}
        	break;
        }


		if((((oneN - oneCN) > (int)amount)) || oneN == 0)
		{
			differences++;
		}
		else if((oneCN -oneN) > (int)amount)
		{
			differences++;
		}
		_delay_cycles(10);
		}
		else
		{
			twoN = *address & 0x00FF;
			twoCN = *Array1 & 0x00FF;

	        if(twoN == 0XFF)
	        {
	        	break;
	        }

			if((twoN - twoCN) > (int)amount)
			{
				differences++;
			}
			else if((twoCN -twoN) > (int)amount)
			{
				differences++;
			}
			_delay_cycles(10);
			address++;
			Array1++;
		}
i++;
		}
	return differences;
}

void ArrayCopy(unsigned char* Array1,int* Array2, unsigned char characters)
{
	int i, *Address,value;
	unsigned char one,two;
	Address = Array2;
	flash_erase(Address);
	for(i=0;i<(characters -1);i++ )
	{
		if(!(i%2))
		{
		one = Array1[i];
		}
		else
		{
			two = Array1[i];
			value= (int)one|((int)two<<8);
			flash_write(Address , value);
			Address++;
		}


	}
}

unsigned char readSwitchP1(unsigned char bit)
{
	int i =0;
	int count =0;

	for(i=10;i>0;i--)
	             {
	           	    if((P1IN & bit) != bit)
	           	    {
	           	    	count++;
	           	    }
	           	    _delay_cycles(10);
	             }
	if(count > 8)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void BlinkLED(unsigned char bit)
{
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
}


void flash_erase(int *addrR)
{
  __disable_interrupt();               // Disable interrupts. This is important, otherwise,
                                       // a flash operation in progress while interrupt may
                                       // crash the system.
  while(BUSY & FCTL3);                 // Check if Flash being used
  FCTL2 = FWKEY + FSSEL_1 + FN3;       // Clk = SMCLK/4
  FCTL1 = FWKEY + ERASE;               // Set Erase bit
  FCTL3 = FWKEY;                       // Clear Lock bit
  *addrR = 0;                           // Dummy write to erase Flash segment
  while(BUSY & FCTL3);                 // Check if Flash being used
  FCTL1 = FWKEY;                       // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                // Set LOCK bit
  __enable_interrupt();
}
void flash_write(int *addrR , int value)
{
  __disable_interrupt();

    int *Flash_ptr;                          // Flash pointer

    Flash_ptr = addrR;              // Initialize Flash pointer
    FCTL1 = FWKEY + ERASE;                    // Set Erase bit
    FCTL3 = FWKEY;                            // Clear Lock bit
   // *Flash_ptr = 0;                           // Dummy write to erase Flash segment

    FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

      *Flash_ptr++ = value;                   // Write value to flash

    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                     // Set LOCK bit

  __enable_interrupt();
}

void Blink2LED(unsigned char bit, unsigned char bit2 )
{
	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);
	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);

	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);
	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);

	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);
}
