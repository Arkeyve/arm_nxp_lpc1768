#include <LPC17xx.h>
#include <stdlib.h>
//#include <lcd_interfacing.h>

#define ref_vtg 3.300
#define full_scale 0xfff // 12bit adc

int main(void) {
	unsigned long adc_temp;
	unsigned int i;
	float in_vtg;
	unsigned char vtg[7], dval[7];
	//unsigned char msg0[] = {"ANALOG IP: "};
	//unsigned char msg1[] = {"ADC OUTPUT: "};

	SystemInit();
	SystemCoreClockUpdate();

	//lcd_init();

	LPC_PINCON->PINSEL3 |= 0xc0000000; //P1.31 as AD0.5
	LPC_SC->PCONP |= (1 << 12); // ADC power on

	/*lcd_com(0x80); //row 0; col 0
	delay_lcd(800);
	lcd_puts(msg0);

	lcd_com(0xc0); //row2; col 0
	delay_lcd(800);
	lcd_puts(msg1);*/

	while(1) {
		LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24); // AD0.5, start conversion, operational
		while((adc_temp = LPC_ADC->ADGDR) == 0x80000000); // wait till done bit is 1
		adc_temp = LPC_ADC->ADGDR;
		adc_temp >>= 4;
		adc_temp &= 0x00000fff; // 12 bit adc
		in_vtg = (((float)adc_temp * (float)ref_vtg)) / ((float)full_scale);
		//sprintf(vtg, "%3.2f", in_vtg);
		for(i=0; i<2000; i++);
	}
}