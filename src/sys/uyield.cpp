#include <Arduino.h>
#include "sys/uyield.h"
#include "sys/wdt.h"

namespace sys {
	namespace uyield {
		const void next() {
			sys::wdt::next();
			yield();
		}
		const void next_ignorewdt() {
			yield();
		}
	}
}
