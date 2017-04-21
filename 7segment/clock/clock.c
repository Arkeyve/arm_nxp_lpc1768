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
 * 0xBF = 1 0 1 1 1 1 1 1   -> Displaying '0' in PM
 * 0x86 = 1 0 0 0 0 1 1 0   -> Displaying '1' in PM
 * 0xDB = 1 1 0 1 1 0 1 1   -> Displaying '2' in PM
 * 0xCF = 1 1 0 0 1 1 1 1   -> Displaying '3' in PM
 * 0xE6 = 1 1 1 0 0 1 1 0   -> Displaying '4' in PM
 * 0xED = 1 1 1 0 1 1 0 1   -> Displaying '5' in PM
 * 0xFD = 1 1 1 1 1 1 0 1   -> Displaying '6' in PM
 * 0x87 = 1 0 0 0 0 1 1 1   -> Displaying '7' in PM
 * 0xFF = 1 1 1 1 1 1 1 1   -> Displaying '8' in PM
 * 0xEF = 1 1 1 0 1 1 1 1   -> Displaying '9' in PM
 ******************************************************************************/
 #include <LPC17xx.h>
 #include <stdio.h>

 #define	FIRST_SEG	0xF87FFFFF
 #define	SECOND_SEG	0xF8FFFFFF
 #define	THIRD_SEG	0xF97FFFFF
 #define	FOURTH_SEG	0xF9FFFFFF
 #define	DISABLE_ALL 0xFA7FFFFF

 unsigned int dig1=0x00,dig2=0x00,dig3=0x00,dig4=0x00, pmflag=0;
 unsigned int twenty_count = 0x00,dig_count=0x00,temp1=0x00;
 unsigned char array_dec[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
 unsigned char array_dec2[10]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF};
 unsigned char tmr0_flg = 0x00,one_sec_flg = 0x00;
 unsigned long int temp2 = 0x00000000,i=0;
 unsigned int temp3=0x00;

 void TIMER0_Init(void);
 void TIMER0_IRQHandler(void);
 void Display(void);

 int main(void)
 {    
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF;	 //P1.23 to P1.26 GPIO enable lines

	LPC_GPIO0->FIODIR |= 0x00000FF0;	//P0.4 to P0.11 output
	LPC_GPIO1->FIODIR |= 0x07800000;	//P1.23 to P1.26 output


	while(1)
	{
		TIMER0_Init();
		while(tmr0_flg == 0x00);
		tmr0_flg = 0x00;
		
		
		dig_count +=1;  
		if(dig_count == 0x05)
            dig_count = 0x00;

		if(one_sec_flg == 0xFF)
		{
 			one_sec_flg = 0x00;
			dig1 +=1;
              
			if(dig1 == 0x0A)
			{ 
				dig1 = 0;
				dig2 +=1;

				if(dig2 == 0x06)
				{
					dig2 = 0;
					dig3+=1;

					if(dig3 == 0x0A)
					{
						dig3 = 0;
						dig4 += 1;
					
					} //end of dig3
				
				} //end of dig2

			}  //end of dig1
			if(dig4 == 0x01 && dig3==2)
			{
				dig4 = 0;
				dig3 = 0;
				dig2 = 0;
				dig1 = 1;
				if(pmflag==0)
					pmflag=1;
				else
					pmflag=0;
			} //end of dig4

		}   //end of one_sec if

		Display();	
		
	
	} //end of while(1)

 }//end of main

 void TIMER0_Init(void)
 {
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
	if(twenty_count ==1000)     //multiplied by 500x2msec for 1 Sec
	{ 
		one_sec_flg = 0xFF;
		twenty_count = 0x00;			 
	}
	else twenty_count += 1;
		LPC_TIM0->IR = 1; 

 }


 void Display(void)      //To Display on 7-segments
 {

        
	if(dig_count == 0x01)		// For Segment U8
	{
		temp1 = dig1; 
		LPC_GPIO1->FIOPIN = FIRST_SEG;    

	}

	else if(dig_count == 0x02)	// For Segment U9
	{
		temp1 = dig2;             
		LPC_GPIO1->FIOPIN = SECOND_SEG;    
   
	}

	else if(dig_count == 0x03)	// For Segment U10
	{
		temp1 = dig3;           
		LPC_GPIO1->FIOPIN = THIRD_SEG;    
	}
	else if(dig_count == 0x04)	// For Segment U11
	{ 
		temp1 = dig4;   
		LPC_GPIO1->FIOPIN = FOURTH_SEG;    

	}
	temp1 &= 0x0F;
	if(pmflag==0)
		temp2 = array_dec[temp1];	// Decoding to 7-segment
	else
		temp2 = array_dec2[temp1];
	temp2 = temp2 << 4;
	LPC_GPIO0->FIOPIN = temp2;		// Taking Data Lines for 7-Seg
	for(i=0;i<500;i++);
	LPC_GPIO0->FIOCLR = 0x00000FF0;//	LPC_GPIO1->FIOPIN = DISABLE_ALL; //disable all the segments
 }
 