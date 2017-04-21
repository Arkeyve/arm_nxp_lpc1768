#include <LPC17xx.h>

unsigned long adc_temp;
unsigned int i;
void pwm_init(void);

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL3 = 0xc0008000; // P1.31 adc, P1.23, pwm
	LPC_SC->PCONP |= (1 << 6) | (1 << 12); // pwm and adc turn on
	
	pwm_init();
	
	while(1) {
		LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24); // AD0.5, start conversion, operational
		for(i=0; i<200; i++); // delay to convert
		while((adc_temp = LPC_ADC->ADGDR) == 0x80000000); // wait till done bit is 1
		adc_temp = LPC_ADC->ADGDR;
		adc_temp >>= 4;
		adc_temp &= 0x00000fff; // 12 bit adc
		LPC_PWM1->MR4 = adc_temp;
		LPC_PWM1->LER = 0xff;
		adc_temp = 0;
	}
}

void pwm_init(void) {
	LPC_PWM1->MCR = 0x02; // reset on match
	LPC_PWM1->MR0 = 0x0fff;
	LPC_PWM1->MR4 = 0x001;
	LPC_PWM1->LER = 0xff;
	LPC_PWM1->TCR = 0x02; // reset pwm and prescaler
	LPC_PWM1->TCR = 0x09; // start pwm and counter
}
