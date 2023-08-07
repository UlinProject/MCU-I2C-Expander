
#include <avr/wdt.h>
#include "sys/uyield.h"
#include "sys/wdt.h"

namespace sys {
	namespace wdt {
		const void disable() {
			wdt_disable();
		}

		const void enable() {
			wdt_enable(WDT_RESET_TIME);
			sys::uyield::next();
		}

		const void next() {
			wdt_reset();
		}
	}
}