#include<LPC17xx.h>

void delay(int ms) {
	LPC_SC->PCONP |= (1<<1);
	LPC_TIM0->TCR = 0x02; // reset timer
	LPC_TIM0->EMR = 0x30; // toggle EM0 upon match
	LPC_TIM0->PR = 0; 
	LPC_TIM0->MR0 = ms - 1; // ms milliseconds
	LPC_TIM0->MCR = 0x02; // stop PC, TC upon match
	LPC_TIM0->TCR = 0x01; //enable timer
	while(!(LPC_TIM0->EMR & 0x01)); // wait till EM0 sets
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL3 |= (3 << 24);
	delay(10);
	while(1);
}
