#include<LPC17xx.h>
#include<stdlib.h>

unsigned char ival, fval;
unsigned char freq = 0, count = 1; 

void init_counter(void) {
	LPC_TIM1->TCR = 0x02; // reset timer 1
	LPC_PINCON->PINSEL3 |= (3 << 4); // P1.18 function 3
	LPC_TIM1->CTCR = 0x02; // count on negative edge
	LPC_TIM1->TCR = 0x01; // enable counter
}

void timer_delay(int time) {
	LPC_TIM0->TCR = 0x02; // reset timer 0
	LPC_TIM0->CTCR = 0x00; // timer operation
	LPC_TIM0->CCR = 0x00; // no capture as timer0 used as timer
	LPC_TIM0->EMR = 0x20; // set on match
	LPC_TIM0->PR = 0;
	LPC_TIM0->MR0 = time - 1; // match this
	LPC_TIM0->MCR = 0x04; // stop TC upon match
	LPC_TIM0->TCR = 0x01; //enable TC
	while(!(LPC_TIM0->EMR & 0x01)); //extracting bit 0 for EMR0
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF; // P0.4 to P0.11 for LED
	LPC_GPIO0->FIODIR |= 0x00000FF0; // P0.4 to P0.11 for output
	init_counter();
	while(1) {
		ival = LPC_TIM1->TC;
		timer_delay(1000);
		fval = LPC_TIM1->TC;
		if((fval - ival) > freq) freq = fval - ival;
		LPC_GPIO0->FIOPIN = (freq << 4);
	}
	
}
