#include<LPC17xx.h> //Contains all functions related to LPC17xx

int main(){

	SystemInit(); //Initialize system
	SystemCoreClockUpdate(); //Start core clock

	/****************************
	 * PINSEL0 -> P0.00 - P0.15 *
	 * PINSEL1 -> P0.16 - P0.31 *
	 * PINSEL2 -> P1.00 - P1.15 *
	 * PINSEL3 -> P1.16 - P1.31 *
	 * PINSEL4 -> P2.00 - P2.15 *
	 * PINSEL5 -> P2.16 - P2.31 *
	 * PINSEL6 -> P3.00 - P3.15 *
	 * PINSEL7 -> P3.16 - P3.31 *
	 ****************************/
	LPC_PINCON->PINSELx = (Value); // x = PINSEL Number

	/*******************
	 * x = PORT Number *
	 *******************/
	LPC_GPIOx->FIODIR = (Value); // 0: Input; 1: Output

	LPC_GPIOx->FIOSET = (Value); // 1: 1
	LPC_GPIOx->FIOCLR = (Value); // 1: 0
	LPC_GPIOx->FIOPIN = (Value); // 1: 1; 0: 0

	LPC_GPIOx->FIOMASK = (Value); // 1: No Further Change; 0: Further Changes Possible
}