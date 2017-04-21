#include<LPC17xx.h>
#include<stdlib.h>

#define RS_CTRL 0x08000000 // P0.27 [register select bit (0 --- command register; 1 --- data register]
#define EN_CTRL 0x10000000 // P0.28	[enable bit]
#define DT_CTRL 0x07800000 // P0.23 - P0.26	[data bits]
#define mask_lcd 0xff807fff // P0.15 - P0.22 [keyboard (rows)]
#define mask_keys 0xe07fffff // P0.23 - P0.28 [reg_sel || enable || data]

void lcd_init(void); // initialize lcd
void clr_disp(void); // clear display
void delay_lcd(unsigned int); // delay lcd
void lcd_com(void); // divide 8bit commands to 2 4bit commands and send to command register
void wr_cn(void); // write to command register
void lcd_data(void); // divide 8bit data to 2 4bit data and send to data register 
void wr_dn(void); // write to data register
void clear_ports(void); // clear all lines
void lcd_puts(unsigned char *); //write string to lcd

void EINT3_IRQHandler(void); // interrupt handler

unsigned int temp1 = 0, temp2 = 0; // for lcd
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
	
	// P0.15 - P0.28 as GPIO
	LPC_PINCON->PINSEL0 &= 0x3fffffff;
	LPC_PINCON->PINSEL1 &= 0xfc000000;
	
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
			
			temp1 = 0x80; // select row 1 on lcd
			lcd_com(); // run temp1 command
			delay_lcd(800);
			lcd_puts(log0msg); // write "number of numbers"
			
			while(logcount == 0) {
				temp1 = 0xc0; // go to beginning of row 2
				lcd_com();
				delay_lcd(800);
				lcd_puts(spaces);

				temp1 = 0xc0; // go to beginning of row 2
				lcd_com();
				delay_lcd(800);
				lcd_puts(c);
			}
		} else if(logcount == 1) {
			LPC_GPIO0->FIOMASK = mask_keys;
			clr_disp();
			
			temp1 = 0x80; // select row 1 on lcd
			lcd_com(); // run temp1 command
			delay_lcd(800);
			lcd_puts(log1msg); // write "enter numbers"
			
			while(i < number_of_numbers) {
				temp1 = 0xc0; // go to beginning of row 2
				lcd_com();
				delay_lcd(800);
				lcd_puts(spaces);

				temp1 = 0xc0; // go to beginning of row 2
				lcd_com();
				delay_lcd(800);
				lcd_puts(c);
			}
			logcount++;
		} else if(logcount == 2) {
			LPC_GPIO0->FIOMASK = mask_keys;
			clr_disp();
			
			temp1 = 0x80; // select row 1 on lcd
			lcd_com(); // run temp1 command
			delay_lcd(800);
			lcd_puts(log2msg); // write "BCD sum is:"
			
			sprintf(sum, "%d", integer_sum); // convert sum from int to string
			
			temp1 = 0xc0; // go to beginning of row 2
			lcd_com();
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
	LPC_GPIO0->FIOMASK = mask_keys;
}

void lcd_init() {
	
	LPC_PINCON->PINSEL1 &= 0xFC003FFF; // P0.23 - P0.28 as GPIO

	// all outputs
	LPC_GPIO0->FIODIR |= DT_CTRL;
	LPC_GPIO0->FIODIR |= RS_CTRL;
	LPC_GPIO0->FIODIR |= EN_CTRL;

	clear_ports();
	delay_lcd(3200);

	temp2 = (0x30 << 19); //8bit mode
	wr_cn();	   
	delay_lcd(30000); 
		
	temp2 = (0x30 << 19); //8bit mode
	wr_cn();
	delay_lcd(30000);	 
		
	temp2 = (0x30 << 19); //8bit mode
	wr_cn();
	delay_lcd(30000);

	temp2 = (0x20 << 19); //4bit mode
	wr_cn();
	delay_lcd(30000);

	temp1 = 0x28; //4bit 2 lines
	lcd_com();
	delay_lcd(30000);
		
	temp1 = 0x0c; // display on; cursor off
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x06; //entry mode, increment
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x01; //clear display and return to home
	lcd_com();
	delay_lcd(10000);
	
	temp1 = 0x80; // row 0 col 0
	lcd_com();
	delay_lcd(800);
	return;
}

void lcd_com(void) {
	temp2 = temp1 & 0xf0;	//move data (26-8+1) times : 26 - HN place, 4 - Bits
	temp2 = temp2 << 19;	//data lines from 23 to 26
	wr_cn();
	temp2 = temp1 & 0x0f; 	//26-4+1
	temp2 = temp2 << 23; 
	wr_cn();
	delay_lcd(1000);
	return;
}

// command nibble o/p routine
void wr_cn(void) {					//write command reg	 
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;		// Assign the value to the data lines    
	LPC_GPIO0->FIOCLR = RS_CTRL;		// clear bit RS
	LPC_GPIO0->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;		 		// EN =0
	return;
}

// data o/p routine which also outputs high nibble first
// and lower nibble next
void lcd_data(void) {	 
	temp2 = temp1 & 0xf0;
	temp2 = temp2 << 19;
	wr_dn();
	temp2= temp1 & 0x0f;	
	temp2= temp2 << 23;
	wr_dn();
	delay_lcd(1000);	
	return;
}

 // data nibble o/p routine
void wr_dn(void) {  	  
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;		// Assign the value to the data lines    
	LPC_GPIO0->FIOSET = RS_CTRL;	// set bit  RS
	LPC_GPIO0->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;	// EN =0
	return;
}

void delay_lcd(unsigned int r1) {
	unsigned int r;
	for(r=0;r<r1;r++);
	return;
}

void clr_disp(void) {
	temp1 = 0x01;
	lcd_com();
	delay_lcd(10000);
	return;
}

void clear_ports(void) {
	/* Clearing the lines at power on */
	LPC_GPIO0->FIOCLR = DT_CTRL; //Clearing data lines
	LPC_GPIO0->FIOCLR = RS_CTRL;  //Clearing RS line
	LPC_GPIO0->FIOCLR = EN_CTRL; //Clearing Enable line

	return;
}

void lcd_puts(unsigned char *buf1) {
	unsigned int i=0;

	while(buf1[i]!='\0') {
		temp1 = buf1[i];
		lcd_data();
		i++;
		if(i==16) {
			temp1 = 0xc0;
			lcd_com();
		}
	}
	return;
}
