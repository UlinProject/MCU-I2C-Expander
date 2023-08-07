#pragma once


namespace i2c_events {
	namespace sys_mem {
		/*#if defined (__AVR_ATtiny88__)
			extern uint8_t i2c_sys_mem[100];
		#else*/
			extern uint8_t i2c_sys_mem[512];
		//#endif
		
		const void init();
		
		const void flush_mem();
		
		const void get_mem();
		const void get_mem_16bit();

		const void set_mem_16bit();
		const void set_mem();
	}
}
