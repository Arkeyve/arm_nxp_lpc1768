#include<LPC17xx.h>

unsigned int i;
unsigned long LED;

int main( void ) {

	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_GPIO0->FIODIR |= 0x00000FF0;

	while(1) {
		LED = 0x00000FF0;
		while(LED != 0x00000010) {
			LPC_GPIO0->FIOPIN = LED;
			for(i = 0; i < 50000; i++);
			LED -= 0x00000010;
		}
	}
}
