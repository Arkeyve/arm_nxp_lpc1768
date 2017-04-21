#include <lpc17xx.h>
#define RS_CTRL  0x08000000  //P0.27
#define EN_CTRL  0x10000000  //P0.28
#define DT_CTRL  0x07800000  //P0.23 to P0.26 data lines
void lcd_init(void); 
void wr_cn(void); 
void clr_disp(void); 
void delay_lcd(unsigned int); 
void lcd_com(void);	
void wr_dn(void); 
void lcd_data(void); 
void clear_ports(void); 
void lcd_puts(unsigned char *);
unsigned long int temp1=0, temp2=0 , row, col ;
unsigned char Msg1[14] = {"DEPT OF 1234"};
unsigned char Msg2[15] = {"MIT, MANIPAL12"};
unsigned char Msg3[15] = {"              "};
unsigned int value[4][4] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

unsigned int mask_display = 0xFFFFF0FF, i=0; // P0.8 to P0.11 unmasked (rows)
unsigned int mask_keyboard = 0xE07FFFFF; //P0.23 to P0.28 unmasked (keyboard)

int main(void) {
   
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_GPIO0->FIOMASK=mask_keyboard;
	lcd_init();				//initialise LCD
	delay_lcd(3200);
	LPC_GPIO0->FIOMASK=mask_display;
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF; //po.4-po.11 as GPIO
	
	LPC_GPIO0-> FIODIR |= 0x00000F00;  //PO.8-PO.11 OUTPUT
	
	LPC_GPIOINT -> IO0IntEnR = 0xF0; // P0.4 - P0.7 for interrupt (columns)
	
	LPC_GPIO0->FIOMASK=mask_display;

	LPC_GPIO0->FIOSET=0xF00; //PO.8-PO.11 HIGH (rows)
	
	LPC_GPIO0->FIOMASK=mask_keyboard; //unmask display
	

	temp1 = 0x80;					  //1st message on LCD 1st line
	lcd_com();
	delay_lcd(800);
	lcd_puts(Msg1);
	
	LPC_GPIO0->FIOMASK = mask_display;

	NVIC_EnableIRQ(EINT3_IRQn);
   
	while(1);
 }
 
 
 
 void EINT3_IRQHandler(void)
 { 
	
	LPC_GPIO0->FIOMASK = mask_keyboard;

	   temp1 = 0xC0;			//LCD 2nd line
	   lcd_com();
	   delay_lcd(800);
	   lcd_puts(Msg2);
	
	LPC_GPIO0->FIOMASK=mask_display;
	
	LPC_GPIOINT->IO0IntClr =0xFFFFFFFF;
	
	for(i=0; i<500000; i++);
	
	
		LPC_GPIO0->FIOMASK=mask_keyboard;

	   temp1 = 0xC0;			//LCD 2nd line
	   lcd_com();
	   delay_lcd(800);
	   lcd_puts(Msg3);
	
	LPC_GPIO0->FIOMASK=mask_display;
	 

	 // row= LPC_GPIOINT->IO2IntStatR;
	  // LPC_GPIO1 -> FIOCLR = 0x07800000;
	   
/*	
   col= LPC_GPIOINT->IO2IntStatR;
   LPC_GPIO1->FIOCLR = 0x07800000;
   for(row=0;row<4;row++)
   {
    LPC_GPIO1->FIOPIN = 1 << (23 + row);
	if(LPC_GPIO2->FIOPIN & col)
	{
	  if(col & 1<<10)
		col=0;
	   else if(col <<11)
	   col=1;
	   else if(col <<12)
	   col=2;
	   else if(col <<13)
	   col=3;
	   
	   temp1 = 0x80;			//1st message on LCD 1st line
	   lcd_com();
	   delay_lcd(800);
	   lcd_puts(Msg1);
	 break;
	}
	}
	LPC_GPIOINT->IO2IntClr =0xFFFFFFFF; */
 }
 
 
 
 //lcd initialization
 void lcd_init()
 {
	/* Ports initialized as GPIO */
    LPC_PINCON->PINSEL1 &= 0xFC003FFF;  //P0.23 to P0.28

	/* Setting the directions as output */
    LPC_GPIO0->FIODIR |= DT_CTRL;	
	LPC_GPIO0->FIODIR |= RS_CTRL;	
	LPC_GPIO0->FIODIR |= EN_CTRL;	
        
    clear_ports();
	delay_lcd(3200);

	temp2 = (0x30<<19);
	// 000[0 0|000 0]000 0000 0000 0000 0011 0000 Rsh 0
	// 000[0 0|000 0]000 0000 0000 0011 0000 0000 Rsh 4
	// 000[0 0|000 0]000 0000 0011 0000 0000 0000 Rsh 8
	// 000[0 0|000 0]000 0011 0000 0000 0000 0000 Rsh 12
	// 000[0 0|000 0]011 0000 0000 0000 0000 0000 Rsh 16
	// 000[0 0|000 0]110 0000 0000 0000 0000 0000 Rsh 17
	// 000[0 0|000 1]100 0000 0000 0000 0000 0000 Rsh 18
	// 000[0 0|001 1]000 0000 0000 0000 0000 0000 Rsh 19
	wr_cn();	   
	delay_lcd(30000); 

	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);	 
		
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);

	temp2 = (0x20<<19);
	wr_cn();
	delay_lcd(30000);

	temp1 = 0x28;
	lcd_com();
	delay_lcd(30000);
		
	temp1 = 0x0c;		
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x06;
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
	
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);
    return;
 }

 void lcd_com(void)
 {
	temp2 = temp1 & 0xf0;//move data (26-8+1) times : 26 - HN place, 4 - Bits
	temp2 = temp2 << 19;//data lines from 23 to 26
	wr_cn();
	temp2 = temp1 & 0x0f; //26-4+1
	temp2 = temp2 << 23; 
	wr_cn();
	delay_lcd(1000);
    return;
 }

 // command nibble o/p routine
 void wr_cn(void)                        //write command reg
 { 	 
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;	// Assign the value to the data lines    
    LPC_GPIO0->FIOCLR = RS_CTRL;		// clear bit RS
	LPC_GPIO0->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;		 		// EN =0
    return;
    
 }

 // data o/p routine which also outputs high nibble first
 // and lower nibble next
 void lcd_data(void)
 {             
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
 void wr_dn(void)
 {  	  
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;		// Assign the value to the data lines    
	LPC_GPIO0->FIOSET = RS_CTRL;	// set bit  RS
	LPC_GPIO0->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;	// EN =0
    return;
 }

 void delay_lcd(unsigned int r1)
 {
  	unsigned int r;
  	for(r=0;r<r1;r++);
    return;
 }

 void clr_disp(void)
 {
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
    return;
 }
 void clear_ports(void)
 {
    /* Clearing the lines at power on */
	LPC_GPIO0->FIOCLR = DT_CTRL; //Clearing data lines
	LPC_GPIO0->FIOCLR = RS_CTRL;  //Clearing RS line
	LPC_GPIO0->FIOCLR = EN_CTRL; //Clearing Enable line
        
    return;
 }

 void lcd_puts(unsigned char *buf1)
 {
    unsigned int i=0;

    while(buf1[i]!='\0')
    {
        temp1 = buf1[i];
     	lcd_data();
		i++;
        if(i==16)
		{
           	temp1 = 0xc0;
			lcd_com();
		}
         
       }
    return;
 }
