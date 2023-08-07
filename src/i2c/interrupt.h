#pragma once

#if defined (__AVR_ATtiny88__)
	#define INTERRUPT_PIN 21
#else
	#define INTERRUPT_PIN 4
#endif

namespace i2c {
	namespace interrupt {
		const void init();
		
		const void en();
		const void dis();
	}
}
