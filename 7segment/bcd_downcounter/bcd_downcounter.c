/***************************************************************************
 * Seven Segment Display
 * Developed by
 * Advanced Electronics Systems, Bengaluru
 *--------------------------------------------------------------------------
 * Seven segment display counter from 0000 to 9999 using a timer0.
 * Common cathode displays are used. By default all enable lines are pulled
 * up in the hardware. Active high data inputs and low at enable line displays
 * the data on displays. Display update wil take place once in a 2msec.
 * Value will change once in a second
 * Connection : CNA to CNA2 and CNB to CNB2
 * Data lines A to H - P0.4 to P0.11 and En1 to En4 - P1.23 to P1.26

 * Lookup Table for displaying 0,1,2,3 to 9
 * at common cathode displays
 *	      A
 *	      __
 *    F	 |  | B
 *	     -g-
 *    E	 |  |  C
 *	      -- . H
 *	      D
 *
 * array_dec[10]:->
 * value= h g f e d c b a	On 7-SEG U8,U9,U10 & U11.
 * 0x3F = 0 0 1 1 1 1 1 1   -> Displaying '0'
 * 0x06 = 0 0 0 0 0 1 1 0   -> Displaying '1'
 * 0x5B = 0 1 0 1 1 0 1 1   -> Displaying '2'
 * 0x4F = 0 1 0 0 1 1 1 1   -> Displaying '3'
 * 0x66 = 0 1 1 0 0 1 1 0   -> Displaying '4'
 * 0x6D = 0 1 1 0 1 1 0 1   -> Displaying '5'
 * 0x7D = 0 1 1 1 1 1 0 1   -> Displaying '6'
 * 0x07 = 0 0 0 0 0 1 1 1   -> Displaying '7'
 * 0x7F = 0 1 1 1 1 1 1 1   -> Displaying '8'
 * 0x6F = 0 1 1 0 1 1 1 1   -> Displaying '9'
 ******************************************************************************/
 #include <LPC17xx.h>
 #include <stdio.h>

 #define	FIRST_SEG	0xF87FFFFF // 1111 1000 0111 1111 1111 1111 1111 1111
 #define	SECOND_SEG	0xF8FFFFFF // 1111 1000 1111 1111 1111 1111 1111 1111
 #define	THIRD_SEG	0xF97FFFFF // 1111 1001 0111 1111 1111 1111 1111 1111
 #define	FOURTH_SEG	0xF9FFFFFF // 1111 1001 1111 1111 1111 1111 1111 1111
 #define	DISABLE_ALL 	0xFA7FFFFF // 1111 1010 0111 1111 1111 1111 1111 1111

 unsigned int dig1=0x09,dig2=0x09,dig3=0x09,dig4=0x09;
 unsigned int twenty_count = 0x00,dig_count=0x04,temp1=0x00;
 unsigned char array_dec[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
 unsigned char tmr0_flg = 0x00,one_sec_flg = 0x00;
 unsigned long int temp2 = 0x00000000, i=0;
 unsigned int temp3=0x00;

 void TIMER0_Init(void);
 void TIMER0_IRQHandler(void);
 void Display(void);

 int main(void) {
	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 &= 0xFF0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF;	 //P1.23 to P1.26 GPIO enable lines

	LPC_GPIO0->FIODIR |= 0x00000FF0;	//P0.4 to P0.11 output
	LPC_GPIO1->FIODIR |= 0x07800000;	//P1.23 to P1.26 output

	TIMER0_Init();

	while(1) {
		while(tmr0_flg == 0x00);
		tmr0_flg = 0x00;

		dig_count -=1;
		if(dig_count == 0x00)
            dig_count = 0x04;

		if(one_sec_flg == 0xFF) {
 			one_sec_flg = 0x00;
			dig1 -=1;

			if(dig1 == 0xFFFFFFFF) {
				dig1 = 0x09;
				dig2 -=1;

				if(dig2 == 0xFFFFFFFF) {
					dig2 = 0x09;
					dig3-=1;

					if(dig3 == 0xFFFFFFFF) {
						dig3 = 0x09;
						dig4 -= 1;

						if(dig4 == 0xFFFFFFFF) {
							dig4 = 0x09;
						} //end of dig4

					} //end of dig3

				} //end of dig2

			}  //end of dig1

		}   //end of one_sec if

		Display();

	} //end of while(1)

 }//end of main

 void TIMER0_Init(void) {
	LPC_SC->PCONP |= (1<<1);    //powers the T0
	LPC_TIM0->TCR = 0x00000020;     // Timer0 Reset
	LPC_TIM0->MR0 = 3000 * 1;	//2 msec
	LPC_TIM0->EMR = 0X30;
	LPC_TIM0->MCR = 0x00000003;     // Interrupt and Reset on MR0

	NVIC_EnableIRQ(TIMER0_IRQn);

	LPC_TIM0->TCR = 0x00000001;     // Timer0 Enable

	return;
 }

 void TIMER0_IRQHandler(void)
 {
	tmr0_flg = 0xFF;
	if(twenty_count == 1000) {    //multiplied by 500x2msec for 1 Sec
		one_sec_flg = 0xFF;
		twenty_count = 0x00;
	} else {
		twenty_count += 1;
	}
	LPC_TIM0->IR = 1;
}


 void Display(void) {     //To Display on 7-segments
	if(dig_count == 0x01) {		// For Segment U8
		temp1 = dig1;
		LPC_GPIO1->FIOPIN = FIRST_SEG; 
	} else if(dig_count == 0x02) {	// For Segment U9
		temp1 = dig2;
		LPC_GPIO1->FIOPIN = SECOND_SEG; 
	} else if(dig_count == 0x03) {	// For Segment U10
		temp1 = dig3;
		LPC_GPIO1->FIOPIN = THIRD_SEG;
	} else if(dig_count == 0x04) {	// For Segment U11
		temp1 = dig4;
		LPC_GPIO1->FIOPIN = FOURTH_SEG; 
	}
	temp1 &= 0x0F;
	temp2 = array_dec[temp1];	// Decoding to 7-segment
	temp2 = temp2 << 4;
	LPC_GPIO0->FIOPIN = temp2;		// Taking Data Lines for 7-Seg
	for(i=0;i<500;i++);
	LPC_GPIO0->FIOCLR = 0x00000FF0; //	LPC_GPIO1->FIOPIN = DISABLE_ALL; //disable all the segments
 }
