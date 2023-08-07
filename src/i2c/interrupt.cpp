
#include "sys/uyield.h"
#include "i2c/interrupt.h"
#include <Arduino.h>

namespace i2c {
	namespace interrupt {
		const void init() {
			pinMode(INTERRUPT_PIN, OUTPUT);
		}
		
		const void en() {
			digitalWrite(INTERRUPT_PIN, LOW);
			sys::uyield::next();
		}
		
		const void dis() {
			digitalWrite(INTERRUPT_PIN, HIGH);
			sys::uyield::next();
		}
	}
}
