/***********************************************************************
 * LED Test
 * Developed by
 * Advanced Electronics Systems, Bengaluru.
 *----------------------------------------------------------------------
 * There are 8 GP leds on the board. This board serially toggles them.
 * Connect a 10 pin FRC cable from CNA to CNA1.
 * Port lines: P0.4 to P0.11
 ***********************************************************************/ 
 #include <LPC17xx.h>
 
 unsigned int i,j,X,Y;
 unsigned long LED = 0x00000010;

 int main(void)
 {
	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL1 &= 0x0;
	 LPC_GPIO2->FIODIR =0X0;
	LPC_GPIO0->FIODIR |= 0x00fff000;

	while(1)
	{
		X=LPC_GPIO2->FIOPIN;
		X= X & (1<<12);
		
			if  (!X )
	{
		LED = 0xFFFFFFFF;
		LPC_GPIO0->FIOPIN = LED;
			
	}
else
{	
	
		LED = 0x0;

				LPC_GPIO0->FIOPIN = LED;

		}

	}
 }
