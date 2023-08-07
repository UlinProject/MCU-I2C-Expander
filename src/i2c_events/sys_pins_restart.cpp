#include <Arduino.h>
#include "led_builtin.h"
#include "i2c/interrupt.h"
#include "sys/sleep.h"

namespace i2c_events {
	namespace sys_pins_restart {
		const void sys_pins_restart() {
			uint8_t i = 0;

			while (i != 26) {
				if (
					i != 23 && i != 24 
					&& i != LED_BUILTIN
					&& i != INTERRUPT_PIN 
					&& i != DIG_PIN 
					&& i != SLEEP_PIN
				) {
					pinMode(i, INPUT);
				}
				i += 1;
			}
			
			i2c::interrupt::en();
		}

	}
}