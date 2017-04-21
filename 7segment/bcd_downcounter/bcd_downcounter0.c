#include <LPC17xx.h>

void TIMER0_IRQHandler(void);
void timer0_init(void);
void display(void);

//define segments --- P1.23 - P1.26
#define seg0 0xF87FFFFF // x000 0xxx
#define seg1 0xF8FFFFFF // x000 1xxx
#define seg2 0xF97FFFFF // x001 0xxx
#define seg3 0xF9FFFFFF // x001 1xxx
unsigned int dig0 = 0, dig1 = 0, dig2 = 0, dig3 = 0; // sent through pins P0.4 - P0.11
unsigned char array_dec[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned int dig_count = 0, temp;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF;
	
	LPC_GPIO0->FIODIR |= (0xFF << 4); // P0.4 - P0.11 as output
	LPC_GPIO1->FIODIR |= (0xF << 23); // P0.23 - P0.26 as output

	timer0_init();
	
	while(1) {
		display();
	}
}

void timer0_init(void) {
	LPC_SC->PCONP |= (0x01); // turn on timer
	LPC_TIM0->TCR = 0x02; // reset timer
	LPC_TIM0->PR = 2999; // 3000 * 1000
	LPC_TIM0->MR0 = 100; // 3000 * 1000
	LPC_TIM0->MCR = 0x03; // generate interrupt and reset on MR0
	
	NVIC_EnableIRQ(TIMER0_IRQn); // enable interrupts
	LPC_TIM0->TCR = 0x01; // start timer
}

void TIMER0_IRQHandler(void) {
	dig0++;
	if(dig0 == 10) {
		dig1++;
		dig0 = 0;
		if(dig1 == 10) {
			dig2++;
			dig1 = 0;
			if(dig2 == 10) {
				dig3++;
				dig2 = 0;
				if(dig3 == 10) {
					dig3 = 0;
				}
			}
		}
	}
	LPC_TIM0->IR |= 0xffffffff;
}

void display(void) {
	if(dig_count == 0) {
		temp = (array_dec[dig0] << 4);
		LPC_GPIO1->FIOPIN = seg0;
	} else if(dig_count == 1) {
		temp = (array_dec[dig1] << 4);
		LPC_GPIO1->FIOPIN = seg1;
	} else if(dig_count == 2) {
		temp = (array_dec[dig2] << 4);
		LPC_GPIO1->FIOPIN = seg2;
	} else if(dig_count == 3) {
		temp = (array_dec[dig3] << 4);
		LPC_GPIO1->FIOPIN = seg3;
	} else {
		dig_count = -1;
	}
	LPC_GPIO0->FIOPIN = (temp & 0x0FF0);
	dig_count++;
}
