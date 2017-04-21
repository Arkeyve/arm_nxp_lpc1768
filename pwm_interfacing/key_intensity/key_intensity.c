#include <LPC17xx.h>

void pwm_init(void);
void EINT3_IRQHandler(void);

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 &= 0xff0000ff;
	LPC_GPIO0->FIODIR |= 0x00000100; //P0.8 output [row 0]
	LPC_GPIOINT->IO0IntEnR = 0x000000f0; //P0.4 - P0.7 interrupt [cols]
	LPC_GPIO0->FIOSET |= 0x00000100; //P0.8 high
	NVIC_EnableIRQ(EINT3_IRQn);

	pwm_init();

	while(1);
}

void pwm_init(void) {
	LPC_SC->PCONP |= (1 << 6);
	LPC_PINCON->PINSEL3 |= 0x00008000; //P0.23 --- PWM1.4

	LPC_PWM1->PCR = (1 << 12); //enable PWM4 single edge
	LPC_PWM1->MCR = 0x02; //reset on match
	LPC_PWM1->MR0 = 3000;
	LPC_PWM1->MR4 = 0; //starting with 0 intensity
	LPC_PWM1->LER = 0xff; //enable shadow copy
	LPC_PWM1->TCR = 0x02; //reset counter and prescaler
	LPC_PWM1->TCR = 0x09; //enable pwm and counter
}

void EINT3_IRQHandler(void) {
	if(LPC_GPIOINT->IO0IntStatR == 0x00000010) {
		LPC_PWM1->MR4 = 3000 * 0.10;
		LPC_PWM1->LER = 0xff;
	} else if(LPC_GPIOINT->IO0IntStatR == 0x00000020) {
		LPC_PWM1->MR4 = 3000 * 0.25;
		LPC_PWM1->LER = 0xff;
	} else if(LPC_GPIOINT->IO0IntStatR == 0x00000040) {
		LPC_PWM1->MR4 = 3000 * 0.50;
		LPC_PWM1->LER = 0xff;
	} else if(LPC_GPIOINT->IO0IntStatR == 0x00000080) {
		LPC_PWM1->MR4 = 3000 * 0.75;
		LPC_PWM1->LER = 0xff;
	}
	
	LPC_GPIOINT->IO0IntClr |= 0x000000f0;
}