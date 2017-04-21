	area reset, data, readonly
	export __Vectors
	
__Vectors
	dcd 0x40001000
	dcd Reset_Handler
	
	area stack_test, code, readonly
	export Reset_Handler
	entry
	
Reset_Handler
	ldr r1, = 0x11223344
	ldr r2, = 0x55667788
	ldr r3, = 0x9900aabb
	ldr r4, = 0xccddeeff
	ldr r5, = 0x40001000
	push {r4, r1, r2, r3}
	
stop
	b stop
	
	end