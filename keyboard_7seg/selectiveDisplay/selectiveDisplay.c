/*********************************************************************
 * Key pad Interface
 * Developed by
 * Advanced Electronics Systems, Bengaluru
 *--------------------------------------------------------------------
 * 4x4, matrix keyboard has total 16 keys. There are 4 rows and 4 cols
 * Identity of key pressed (0 to F) will be displayed on LCD.
 * Port lines used : P1.23 to P1.26 - col1 to col4, P2.10 to P2.13
 * row1 to row4. Connect 10 pin FRC cable from CNB to CNB3,
 * Short JP4 (1, 2)

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
 * array_hex[10]:->
 * value= h g f e d c b a	On 7-SEG U8,U9,U10 & U11.
 * 0x01 = 0 0 0 0 0 0 0 1 -> [0]
 * 0x22 = 0 0 1 0 0 0 1 0 -> [1]
 * 0x14 = 0 0 0 1 0 1 0 0 -> [2]
 * 0x08 = 0 0 0 0 1 0 0 0 -> [3]
 *********************************************************************/

 #include <LPC17xx.h>
 #define	FIRST_SEG      0xFFF87FFF // 1111 1111 1111 1000 0111 1111 1111 1111
 #define	SECOND_SEG     0xFFF8FFFF // 1111 1111 1111 1000 1111 1111 1111 1111
 #define	THIRD_SEG      0xFFF97FFF // 1111 1111 1111 1001 0111 1111 1111 1111
 #define	FOURTH_SEG     0xFFF9FFFF // 1111 1111 1111 1001 1111 1111 1111 1111
 #define	DISABLE_ALL    0xFFFA7FFF // 1111 1111 1111 1010 0111 1111 1111 1111
 void scan(void);

 unsigned char col,row,flag;
 unsigned long int i,var1,temp,temp3,temp2;
 //unsigned char SEVEN_CODE[4][4] = {{0x3F,0x06,0x5B,0x4F},{0x66,0x6D,0x7D,0x07},{0x7F,0x6F,0x77,0x7c},{0x39,0x5e,0x79,0x71}};
 //unsigned char SEVEN_CODE[4][4] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7c,0x58,0x5e,0x79,0x71};
 unsigned int segmentsel[4] = {FOURTH_SEG,THIRD_SEG,SECOND_SEG,FIRST_SEG};
 unsigned char codesel[4] = {0x01,0x22,0x14,0x08};

 int main(void)
 {
 	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_GPIO0->FIODIR |= 0x00000FF0;	//P0.4 to P0.11 output
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF; //P1.23 to P1.26 MADE GPIO
	LPC_PINCON->PINSEL3 &= 0xF00FFFFF; //P2.10 t P2.13 made GPIO
	LPC_GPIO2->FIODIR |= 0x00003C00; //made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIR &= 0xF87FFFFF; //made input P1.23 to P1.26 (cols)
	LPC_PINCON->PINSEL1 &= 0xFFFFFF00; //P0.15 to P0.18 for multiplexing
	LPC_GPIO0->FIODIR |= 0x00078000; //P0.15 to P0.18 output

	while(1)
	{
		for(row=0;row<4;row++)
			{
				if(row == 0)
				temp = 0x00000400;
				else if(row == 1)
				temp = 0x00000800;
				else if(row == 2)
				temp = 0x00001000;
				else if(row == 3)
				temp = 0x00002000;

				LPC_GPIO2->FIOPIN = temp;
				flag = 0;
				scan();
				if(flag == 1)
				{
				//displaycode here
					LPC_GPIO0->FIOMASK = 0xFFF00FFF;
					LPC_GPIO0->FIOPIN = segmentsel[col];
					LPC_GPIO0->FIOMASK = 0xFFFFF00F;
					LPC_GPIO0->FIOPIN = (codesel[row] << 4);
				}
			} //end for(row=1;row<5;row++)

		}//end while 1

 }//end main

 void scan(void)
 {
 	unsigned long temp3;

	temp3 = LPC_GPIO1->FIOPIN;
	temp3 &= 0x07800000;
	if(temp3 != 0x00000000)
	{
				flag = 1;
			if (temp3 ==0x00800000)
				col=0;
			else if (temp3==0x01000000)
				col=1;
			else if (temp3==0x02000000)
				col=2;
			else if (temp3==0x04000000)
				col=3;

	}//1st if(temp3 != 0x00000000)
 }//end scan
