	/*******************************************************************************
	************************************************************
	 * TIMER
	 * -----
	 *
	 * Registers:
	 * IR: Interrupt Register
	 * TCR: Timer Control Register
	 * TC: Timer Counter
	 * PC: Prescale Counter
	 * PR: Prescale Register
	 *
	 * Capture and Match Registers:
	 * CTCR: Counter Control Register
	 * CCR: Capture Control Register
	 * EMR: External Match Register
	 * MCR: Match Control Register
	 *
	 * IR: [Reserved][CR1][CR0][MR3][MR2][MR1][MR0]
	 * 	# Corresponding bits turn to 1 when interrupt is to be generated
	 * 	# Once set, 1 sent again to clear
	 *		# Cannot be set manually, thus sending 1 when 0 has no effect
	 *
	 * TCR: [Reserved][Reset][Disable/Enable(TC & PC)]
	 *		# Bit 0{0: disable; 1: enable}
	 *		# Bit 1{0: no reset; 1: reset}
	 *
	 * TC, PR, PC:
	 *		# PC counts 0x00000000 to PR
	 *		# TC counts from 0x00000000 to 0xFFFFFFFF (one count - one +ve edge after PC matches PR)
	 *
	 * CTCR: [Reserved][CAPn.0 or CAPn.1][Mode Bit 1][Mode Bit 0]
	 * 	# Mode {
	 * 		00: Timer;
	 * 		01: Counter +ve edge;
	 * 		10: Counter -ve edge;
	 * 		11: Counter any edge;
	 * 	  }
	 *
	 * CCR: [Reserved][CAPn.1 interrupt][CAPn.1 -ve edge][CAPn.1 +ve edge][CAPn.0 interrupt][CAPn.0 -ve edge][CAPn.0 +ve edge]
	 *
	 * EMR: [Reserved][EMC3][EMC3][EMC2][EMC2][EMC1][EMC1][EMC0][EMC0][EM3][EM2][EM1][EM0]
	 * 	# EMs are outputs depending on corresponding EMCs (External Match Control Bits)
	 * 	# EMC {
	 *			00: no change;
	 *			01: clear;
	 *			10: set;
	 *			11: toggle;
	 *	  }
	 *
	 * MCR: [Reserved][MR3s][MR3r][MR3i][MR2s][MR2r][MR2i][MR1s][MR1r][MR1i][MR0s][MR0r][MR0i]
	 * 	# Defines what to do upon match
	 * 	# MRxs: STOP TC and PC
	 * 	# MRxr: RESET TC
	 * 	# MRxi: Interrupt
	 *******************************************************************************************************************************************/

	LPC_SC->PCONP |= (1<<1); // switches timer on (not necessary as timer on by default. but jic)
	LPC_TIM0->TCR = 0x02; // reset timer
	LPC_TIM0->EMR = 0x20; // EMC0->10; SET EM0 upon match
	LPC_TIM0->PR = 2999; // 3000 - 1
	LPC_TIM0->MR0 = 999; // 1000 - 1; (PR + 1) * (MR0 + 1) = 3e6
	LPC_TIM0->MCR = 0x04; // stop PC and TC upon match
	LPC_TIM0->TCR = 0x01; // enable timer0
	while(!(LPC_TIM0 -> EMR & 0x01)); // wait until match (extracting only last bit)