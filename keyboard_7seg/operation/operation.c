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
 * 0x73 = 0 1 1 1 0 0 1 1   -> Displaying 'P'
 * 0x40 = 0 1 0 0 0 0 0 0   -> Displaying '-'
 * 0x48 = 0 1 0 0 1 0 0 0   -> Displaying '='
 *********************************************************************/

 #include <LPC17xx.h>
 #define	FIRST_SEG		0xFFF87FFF // 1111 1111 1111 1000 0111 1111 1111 1111
 #define	SECOND_SEG	0xFFF8FFFF // 1111 1111 1111 1000 1111 1111 1111 1111
 #define	THIRD_SEG		0xFFF97FFF // 1111 1111 1111 1001 0111 1111 1111 1111
 #define	FOURTH_SEG	0xFFF9FFFF // 1111 1111 1111 1001 1111 1111 1111 1111
 #define	DISABLE_ALL 0xFFFA7FFF
 void scan(void);

 unsigned int dig_count = 0x00;
 unsigned char col, row, flag;
 unsigned int dig1 = 0x00, dig2 = 0x00, dig3 = 0x00, dig4 = 0x00;
 unsigned long int i, var1, temp, temp1, temp3, temp2;
 unsigned char SEVEN_CODE[4][4] = {{0x3F,0x06,0x5B,0x4F},{0x66,0x6D,0x7D,0x07},{0x7F,0x6F,0x73,0x40},{0x48,0x00,0x00,0x00}};
 unsigned int segmentsel[4] = {FOURTH_SEG,THIRD_SEG,SECOND_SEG,FIRST_SEG};

 void Display(void)      //To Display on 7-segments
 {
	LPC_GPIO0->FIOMASK = 0xFFF87FFF;
	if(dig_count == 0x01)		// For Segment U8
	{
		if(dig1 == 0x00 && flag == 1)
			dig1 = SEVEN_CODE[row][col];
		temp1 = dig1;
		LPC_GPIO0->FIOPIN = FIRST_SEG;
	}
	else if(dig_count == 0x02)	// For Segment U9
	{
		//if(dig2 != 0xFF)
		if(dig2 == 0x00 && flag == 1)
			dig2 = SEVEN_CODE[row][col];
		temp1 = dig2;
		LPC_GPIO0->FIOPIN = SECOND_SEG;
	}
	else if(dig_count == 0x03)	// For Segment U10
	{
		//if(dig3 != 0xFF)
		if(dig3 == 0x00 && flag == 1)
			dig3 = SEVEN_CODE[row][col];
		temp1 = dig3;
		LPC_GPIO0->FIOPIN = THIRD_SEG;
	}
	else if(dig_count == 0x04)	// For Segment U11
	{
		//if(dig4 != 0xFF)
		if(dig4 == 0x00 && flag == 1)
			dig4 = SEVEN_CODE[row][col];
		temp1 = dig4;
		LPC_GPIO0->FIOPIN = FOURTH_SEG;
	}
	//temp1 &= 0x0F;
	//temp2 = temp1;	// Decoding to 7-segment
	//temp2 = temp2 << 4;
	LPC_GPIO0->FIOMASK = 0xFFFFF00F;
	temp1 = temp1 << 4;
	LPC_GPIO0->FIOPIN = temp1;		// Taking Data Lines for 7-Seg
	for(i=0;i<50;i++);
	LPC_GPIO0->FIOCLR = 0x00000FF0;
  LPC_GPIO0->FIOMASK = 0xFFFFFFFF;
//	LPC_GPIO1->FIOPIN = DISABLE_ALL; //disable all the segments
 }


 int main(void)
 {
 	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 &= 0xFF0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_GPIO0->FIODIR |= 0x00000FF0;	//P0.4 to P0.11 output
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF; //P1.23 to P1.26 MADE GPIO
	LPC_PINCON->PINSEL4 &= 0xF00FFFFF; //P2.10 t P2.13 made GPIO
	LPC_GPIO2->FIODIR |= 0x00003C00; //made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIR &= 0xF87FFFFF; //made input P1.23 to P1.26 (cols)
	LPC_PINCON->PINSEL1 &= 0xFFFFFF00; //P0.15 to P0.18 for multiplexing
	LPC_GPIO0->FIODIR |= 0x00078000; //P0.15 to P0.18 output

	while(1) {
		dig_count += 1;
		if(dig_count == 0x05)
            dig_count = 0x00;

		for(row=0;row<4;row++) {
			if(row == 0)
			    temp = 0x00000400; //P0.10
			else if(row == 1)
			    temp = 0x00000800; //p0.11
			else if(row == 2)
			    temp = 0x00001000; //P0.12
			else if(row == 3)
			    temp = 0x00002000; //P0.13

			LPC_GPIO2->FIOPIN = temp;
			flag = 0;
			scan();
			/*if(flag == 1) {
                Display();
			}*/
            Display();
		} //end for(row=1;row<5;row++)
    }//end while 1

 }//end main

 void scan(void) {
 	unsigned long temp3;

	temp3 = LPC_GPIO1->FIOPIN;
	temp3 &= 0x07800000;
	if(temp3 != 0x00000000) {
		flag = 1;
		if (temp3 == 0x00800000)
			col=0;
		else if (temp3 == 0x01000000)
			col=1;
		else if (temp3 == 0x02000000)
			col=2;
		else if (temp3 == 0x04000000)
			col=3;
	}//1st if(temp3 != 0x00000000)
 }//end scan
