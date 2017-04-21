#include<LPC17xx.h>
#include<stdlib.h>

unsigned char i = 0x00000000;

void init_timer0(void) {
	LPC_SC->PCONP |= (1 << 1); // power on
	LPC_PINCON->PINSEL3 |= (3 << 4) | (3 << 18); // CAP1.0(P1.18) and MAT1.1(P1.25)
	LPC_TIM1->TCR = 0x02; // timer1 reset
	LPC_TIM1->CTCR = 0x02; // counter -ve edge
	LPC_TIM1->MR1 = 3; // match count with 3
	LPC_TIM1->EMR = 0xC0; // toggle EM1 upon match
	LPC_TIM1->MCR = 0x10; // reset MR1 upon match
	LPC_TIM1->TCR = 0x01; // enable timer1
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	init_timer0();
	while(1);
}
