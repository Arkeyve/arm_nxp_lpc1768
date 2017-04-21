#include <LPC17xx.h>
#include <stdlib.h>

#define DT_CTRL 0x07800000 //P0.23 - P0.26
#define EN_CTRL 0x10000000 //P0.28
#define RS_CTRL 0x08000000 //P0.27

void lcd_init(void);
void clr_disp(void);
void lcd_com(unsigned int);
void wr_cn(unsigned int);
void lcd_data(unsigned int);
void wr_dn(unsigned int);
void delay_lcd(unsigned int);
void clear_ports(void);
void lcd_puts(unsigned char *);

void EINT3_IRQHandler(void);

unsigned char msg[1];
int random_number;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL4 &= 0xFFFFFFFC; // P2.0 --- GPIO
	LPC_GPIOINT->IO2IntEnR |= 0x01; // P2.0 --- GPIOINT
	
	lcd_init();
	delay_lcd(3600);
	
	NVIC_EnableIRQ(EINT3_IRQn);
	
	while(1);
	
	return 0;
}

void EINT3_IRQHandler(void) {
	random_number = (rand() % 7);
	sprintf(msg, "%d", random_number);
	
	lcd_com(0x80); // row 1 lcd
	delay_lcd(800);
	
	lcd_puts(msg);
	delay_lcd(800);
	
	LPC_GPIOINT->IO2IntClr |= 0x01;
	
	return;
}

void lcd_init(void) {
	LPC_PINCON->PINSEL1 &= 0xfc003fff; //P0.23 
	
	//all outputs
	LPC_GPIO0->FIODIR |= EN_CTRL;
	LPC_GPIO0->FIODIR |= RS_CTRL;
	LPC_GPIO0->FIODIR |= DT_CTRL;
	
	clear_ports();
	delay_lcd(3200);
	
	lcd_com(0x30); //8bit
	delay_lcd(3000);
	
	lcd_com(0x30); //8bit mode
	delay_lcd(30000);
	
	lcd_com(0x30); //8bit mode
	delay_lcd(30000);
	
	lcd_com(0x20); //4bit mode
	delay_lcd(30000);
	
	lcd_com(0x28); //4bit, 2 lines
	delay_lcd(30000);
	
	lcd_com(0x0c); //display on; cursor off
	delay_lcd(800);
	
	lcd_com(0x06); //entry mode --- increment
	delay_lcd(800);
	
	lcd_com(0x01); //clear display and return to home
	delay_lcd(10000);
	
	lcd_com(0x80); //row 0, col 0
	delay_lcd(800);
	
	return;
}

void lcd_com(unsigned int cmd) {
	unsigned int cmd_nibble;
	
	cmd_nibble = (cmd & 0xf0) << 19;
	wr_cn(cmd_nibble);
	
	cmd_nibble = (cmd & 0x0f) << 23;
	wr_cn(cmd_nibble);
	
	delay_lcd(1000);
	
	return;
}

void wr_cn(unsigned int cnibble) {
	clear_ports();
	LPC_GPIO0->FIOPIN = cnibble; // assign value to pin
	LPC_GPIO0->FIOCLR = RS_CTRL; // command mode
	LPC_GPIO0->FIOSET = EN_CTRL; // enable
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL; //disable
	return;
}

void lcd_data(unsigned int data) {
	unsigned int data_nibble;
	
	data_nibble = (data & 0xf0) << 19;
	wr_dn(data_nibble);
	
	data_nibble = (data & 0x0f) << 23;
	wr_dn(data_nibble);
	
	delay_lcd(1000);
	
	return;
}

void wr_dn(unsigned int dnibble) {
	clear_ports();
	LPC_GPIO0->FIOPIN = dnibble; // assign value to pin
	LPC_GPIO0->FIOSET = RS_CTRL; // data mode
	LPC_GPIO0->FIOSET = EN_CTRL; // enable
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL; //disable
	return;
}

void delay_lcd(unsigned int delay) {
	unsigned int i;
	for(i=0; i<delay; i++);
	return;
}

void clr_disp(void) {
	lcd_com(0x01); // clear display
	delay_lcd(10000);
	return;
}

void clear_ports(void) {
	LPC_GPIO0->FIOCLR = (DT_CTRL | RS_CTRL | EN_CTRL); //clear all
	return;
}

void lcd_puts(unsigned char *msg) {
	unsigned int i = 0;
	while(msg[i] != '\0') {
		lcd_data(msg[i]);
		i++;
		if(i == 16) {
			lcd_com(0xc0); // row 1, col 0
		}
	}
	return;
}