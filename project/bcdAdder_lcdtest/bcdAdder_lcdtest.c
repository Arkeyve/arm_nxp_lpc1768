#include <LPC17xx.h>
#include <stdlib.h>

#define RS_CTRL 0x08000000 // P0.27 [register select bit (0 --- command register; 1 --- data register]
#define EN_CTRL 0x10000000 // P0.28	[enable bit]
#define DT_CTRL 0x07800000 // P0.23 - P0.26	[data bits]
#define mask_lcd 0xff807fff // P0.15 - P0.22 [keyboard (rows)]
#define mask_keys 0xe07fffff // P0.23 - P0.28 [reg_sel || enable || data]

void lcd_init(void); //intitalize
void clr_disp(void); //clear display
void lcd_com(unsigned int); //send command
void wr_cn(unsigned int); //divide 8 bit commands to two 4 bit and write to pins
void lcd_data(unsigned int); //send data
void wr_dn(unsigned int); //divide 8 bit data to two 4 bit and write to pins
void delay_lcd(unsigned int); //call some delay
void clear_ports(void); //clear all pins
void lcd_puts(unsigned char *); // write the message

void EINT3_IRQHandler(void); // interrupt handler

unsigned int row, col; // for keyboard
unsigned char c[4], sum[8];
unsigned char log0msg[] = {"Number of nos.?"};
unsigned char log1msg[] = {"Enter numbers:"};
unsigned char log2msg[] = {"BCD sum is: "};
unsigned char spaces[] = {"                "}; // 16 spaces. to clear line 2
unsigned int logcount = 0, top, integer_sum = 0, number_of_numbers, i = 0, j;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();

	//initialize c
	top = 0;
	c[top] = '\0';
	
	// P0.15 - P0.22 as GPIO
	LPC_PINCON->PINSEL0 &= 0x3fffffff;
	LPC_PINCON->PINSEL1 &= 0xffffc000;
	
	LPC_GPIO0->FIODIR |= (0xf << 19); // P0.19 - P0.22 output [rows]
	LPC_GPIO0->FIODIR &= ~(0xf << 15); // P0.15 - P0.18 input [columns]
	LPC_GPIOINT->IO0IntEnR |= (0xf << 15); // P0.15 - P0.18 GPIO Interrupt [columns]
	
	LPC_GPIO0->FIOMASK = mask_keys;
	lcd_init(); // initialize lcd
	delay_lcd(3200);
	
	LPC_GPIO0->FIOMASK = mask_lcd;
	LPC_GPIO0->FIOSET = (0xf << 19); // P0.19 - P0.22 set 1 [rows]
	
	NVIC_EnableIRQ(EINT3_IRQn); // enable interrupts
	
	while(1) {
		if(logcount == 0) {
			LPC_GPIO0->FIOMASK = mask_keys;
			clr_disp();
			
			lcd_com(0x80); // row 0, col 0
			delay_lcd(800);
			lcd_puts(log0msg); // write "number of numbers"
			
			while(logcount == 0); // wait for interrupt and keep displaying numbers
			
		} else if(logcount == 1) {
			LPC_GPIO0->FIOMASK = mask_keys;
			clr_disp();
			
			lcd_com(0x80); // select row 1 on lcd
			delay_lcd(800);
			lcd_puts(log1msg); // write "enter numbers"
			
			while(i < number_of_numbers); // wait for interrupt and keep displaying numbers
			
			logcount++;
		} else if(logcount == 2) {
			LPC_GPIO0->FIOMASK = mask_keys;
			clr_disp();
			
			lcd_com(0x80); // select row 1 on lcd
			delay_lcd(800);
			lcd_puts(log2msg); // write "BCD sum is:"
			
			sprintf(sum, "%d", integer_sum); // convert sum from int to string
			
			lcd_com(0xc0); // go to beginning of row 2
			delay_lcd(800);
			lcd_puts(sum);

			while(1); // end in infinite loop
		}
	}	
	return 0;
}

void EINT3_IRQHandler(void) {
	unsigned int x = LPC_GPIOINT->IO0IntStatR;	// get status of interrupt register
	x &= 0x00078000; // extract bits 15 to 18
	
	LPC_GPIO0->FIOMASK = mask_lcd;
	
	for(j=0; j<100; j++); // some delay for key debounce period error

	if(x == 0x00008000) col = 0; // bit 15
	else if(x == 0x00010000) col = 1; // bit 16
	else if(x == 0x00020000) col = 2; // bit 17
	else if(x == 0x00040000) col = 3; //bit 18
	else col = -1;

	LPC_GPIO0->FIOCLR = (0xF << 19); // clear rows

	for(row = 0; row < 4; row++) {
		if(row == 0) {
			LPC_GPIO0->FIOSET = (1 << 19); // bit 19
			if((LPC_GPIO0->FIOPIN & 0x00078000) == x) break;
		}
		else if(row == 1) {
			LPC_GPIO0->FIOSET = (1 << 20); // bit 20
			if((LPC_GPIO0->FIOPIN & 0x00078000) == x) break;
		}
		else if(row == 2) {
			LPC_GPIO0->FIOSET = (1 << 21); // bit 21
			if((LPC_GPIO0->FIOPIN & 0x00078000) == x) break;
		}
		else if(row == 3) {
			LPC_GPIO0->FIOSET = (1 << 22); // bit 22
			if((LPC_GPIO0->FIOPIN & 0x00078000) == x) break;
		}
	}

	if(col != -1) {
		//perform function that corresponds to the input
		if(row == 0 && col == 0) {
			c[top++] = '1';
			c[top] = '\0';
		} else if(row == 0 && col == 1) {
			c[top++] = '2';
			c[top] = '\0';
		} else if(row == 0 && col == 2) {
			c[top++] = '3';
			c[top] = '\0';
		} else if(row == 0 && col == 3) {
			c[--top] = '\0';
		} else if(row == 1 && col == 0) {
			c[top++] = '4';
			c[top] = '\0';
		} else if(row == 1 && col == 1) {
			c[top++] = '5';
			c[top] = '\0';
		} else if(row == 1 && col == 2) {
			c[top++] = '6';
			c[top] = '\0';
		} else if(row == 2 && col == 0) {
			c[top++] = '7';
			c[top] = '\0';
		} else if(row == 2 && col == 1) {
			c[top++] = '8';
			c[top] = '\0';
		} else if(row == 2 && col == 2) {
			c[top++] = '9';
			c[top] = '\0';
		} else if(row == 3 && col == 1) {
			c[top++] = '0';
			c[top] = '\0';
		} else if(row == 3 && col == 3) {
			if(logcount == 0) {
				logcount++;
				number_of_numbers = atoi(c);
				top = 0; // reinitialize char stack
				c[top] = '\0';
			} else if(logcount == 1) {
				i++;
				integer_sum += atoi(c);
				top = 0; // reinitialize char stack
				c[top] = '\0';
			}
		}
	}

	LPC_GPIOINT->IO0IntClr |= (0xF << 15); // clear interrupt bits 15 - 18
	LPC_GPIO0->FIOSET = (0xf << 19); // P0.19 - P0.22 set 1 [rows]
	
	LPC_GPIO0->FIOMASK = mask_keys; // unmask lcd
	
	//display entered number
	lcd_com(0xc0);  // go to beginning of row 2
	delay_lcd(800);
	lcd_puts(spaces);

	lcd_com(0xc0);  // go to beginning of row 2
	delay_lcd(800);
	lcd_puts(c);
}

void lcd_init(void) {
	LPC_PINCON->PINSEL1 &= 0xfc003fff; //P0.23 - P0.26
	
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
