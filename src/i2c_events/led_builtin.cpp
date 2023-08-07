
#include <Arduino.h>
#include "i2c/interrupt.h"
#include "led_builtin.h"

namespace i2c_events {
	namespace led_builtin {
		const void init() {
			pinMode(DIG_PIN, OUTPUT);
		}
		
		const void led_builtin_on() {
			digitalWrite(DIG_PIN, HIGH);
					
			i2c::interrupt::en();
		}

		const void led_builtin_off() {
			digitalWrite(DIG_PIN, LOW);
					
			i2c::interrupt::en();
		}
	}

}