/*SEVEN SEGMENT DISPLAY
---------------------

	   -a-
	f |   | b
	   -g-
	e |   | c
	   -d- .h

4 digit seven-segment display requires multiplexing

Requirements {
	8bits for sending data, 
	2 pins for selecting a display, 
	1 extra pin for disabling all displays
}

For a 4 digit number to be displayed,
the number must be separated into 4 digits, one for each segment. 

Suppose the number to be displayed is 1234

Then, have 4 variables each reaching to 1 display each,
Say,
	display0 = [][][][#], 
	display1 = [][][#][],
	display2 = [][#][][],
	display3 = [#][][][].

And,
	digit0 = 4,
	digit1 = 3,
	digit2 = 2,
	digit1 = 1.

Where, 
	digit0 -> display0
	digit1 -> display1
	digit2 -> display2
	digit3 -> display3

Program flow will be {
	while(1) {
		select display0,
		send digit0,
		wait for few milliseconds.

		select display1,
		send digit1,
		wait for few milliseconds.

		select display2,
		send digit2,
		wait for few milliseconds.

		select display3,
		send digit3,
		wait for few milliseconds.
	}
}
*/