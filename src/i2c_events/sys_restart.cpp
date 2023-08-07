#include "sys/uyield.h"
#include "sys/wdt.h"
#include "i2c/interrupt.h"

namespace i2c_events {
	namespace sys_restart {
		const void sys_restart() {
			void(*ptrToFunction)() = 0; // pointer to a function

			sys::uyield::next();
			sys::wdt::disable();
			i2c::interrupt::dis();
			(*ptrToFunction)(); // reset!
		}
	}
}