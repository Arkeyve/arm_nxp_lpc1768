#include<LPC17xx.h>

unsigned int button;
unsigned int i;
unsigned long LED;

int main( void ) {
	
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_GPIO0->FIODIR |= 0x00000FF0;
	LPC_GPIO2->FIODIR &= 0x0;
	
	while(1) {
		
		button = LPC_GPIO2 -> FIOPIN;
		button &= (1 << 12);
		
		while ( !button && LED != 0x00000FF0 ) {
			LPC_GPIO0 -> FIOPIN = LED;
			for ( i = 0; i < 50000; i++ );
			LED += 0x00000010;
		}
		
		while ( button && LED != 0x00000010 ) {
			LPC_GPIO0 -> FIOPIN = LED;
			for ( i = 0; i < 50000; i++ );
			LED -= 0x00000010;
		}
	}
}