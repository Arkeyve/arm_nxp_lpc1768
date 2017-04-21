#include <LPC17xx.h>

void pwm_init(void);
void PWM1_IRQHandler(void);

unsigned long int i;
unsigned char flag, flag1;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();

	pwm_init();

	while(1) {
		for(i=0; i<1000; i++); // delay
	}
}

void pwm_init(void) {
	LPC_SC->PCONP |= (1 << 6); // PWM1 power
	LPC_PINCON->PINSEL3 &= ~(0x0000c000); // clear any flag
	LPC_PINCON->PINSEL3 |= 0x00008000; // PWM1.4 selected for P1.23

	LPC_PWM1->PCR = 0x1000; // select pwm1 single edge
	LPC_PWM1->MCR = 0x03; // reset and interrupt on PWM MR0
	LPC_PWM1->MR0 = 30000; // register match register 0 count
	LPC_PWM1->MR4 = 0x0100; // setup match register MR4
	LPC_PWM1->LER = 0xff; // enable shadow copy register
	LPC_PWM1->TCR = 0x02; // reset counter and prescaler
	LPC_PWM1->TCR = 0x09; // enable pwm and counter

	NVIC_EnableIRQ(PWM1_IRQn);
	return;
}

void PWM1_IRQHandler(void) {
	LPC_PWM1->IR = 0xFF; // clear interrupts

	if(flag == 0x00) {
		LPC_PWM1->MR4 += 100;
		LPC_PWM1->LER = 0xff; // enable shadow copy

		if(LPC_PWM1->MR4 >= 27000) {
			flag = 0xff;
			LPC_PWM1->LER = 0xff;
		}
	} else if(flag == 0xff) {
		LPC_PWM1->MR4 -= 100;
		LPC_PWM1->LER = 0xff;
		
		if(LPC_PWM1->MR4 <= 300) {
			flag = 0x00;
			LPC_PWM1->LER = 0xff;
		}
	}
	
	return;
}