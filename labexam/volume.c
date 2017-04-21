/*
     -a-
  f |	| b
     -g-
  e |	| c
     -d- .h

h g f e d c b a
0 0 0 0 0 0 0 0 --- level 00 06 12
0 0 0 1 0 0 0 0 --- level 01 07 13
0 0 1 1 0 0 0 0 --- level 02 08 14
0 1 1 1 1 0 0 1 --- level 03 09 15
0 0 1 1 0 1 0 0 --- level 04 10 16
0 0 1 1 0 1 1 0 --- level 05 11 17

Thus, total 6 levels per 7 segment display, totalling to 6 * 4 levels.

12 bits divided to 24 levels.

((24 * current_val) / 0x0fff) */

void digit_init() {
	dig0 = dig1 = dig2 = dig3 = 0;
}

while(1) {
	digit_init();
	current_val = ((LPC_ADC->ADGDR >> 4) & 0x0fff)
	dig0 = ((24 * current_val) / 0x0fff);
	if(dig0 > 6) {
		dig1 = (dig0 - 6);
		dig0 = 5;
		if(dig1 > 11) {
			dig2 = (dig1 - 6);
			dig1 = 11;
			if(dig2 > 17) {
				dig3 = (dig2 - 6);
				dig2 = 17;
			}
		}
	}

	display();
}