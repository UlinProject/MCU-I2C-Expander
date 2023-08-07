#pragma once

#define WDT_RESET_TIME WDTO_8S

namespace sys {
	namespace wdt {
		const void disable();
		const void enable();
		const void next();
	}
}