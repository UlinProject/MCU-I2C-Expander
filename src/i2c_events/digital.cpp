
#include "i2c/i2c.h"
#include "sys/uyield.h"
#include <Arduino.h>
#include "i2c/interrupt.h"
#include "i2c/buff.h"

namespace i2c_events {
	namespace digital {
		const void digital_write_high() {
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			digitalWrite(pin, HIGH);
			
			i2c::interrupt::en();
		}

		const void digital_write_low() {
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			digitalWrite(pin, LOW);
			
			i2c::interrupt::en();
		}
	}
}