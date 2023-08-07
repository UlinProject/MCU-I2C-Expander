
#include <Arduino.h>
#include "i2c/i2c.h"
#include "sys/uyield.h"
#include "sys/sleep.h"
#include "i2c/buff.h"
#include "i2c/interrupt.h"

namespace i2c_events {
	namespace analog {
		const void analog_read() {
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			
			const bool is_adcsleep = sys::sleep::is_adcsleep;
			if (is_adcsleep) {
				sys::sleep::set_adc(false);
			}
			const uint16_t value = (uint16_t)analogRead(pin);
			if (is_adcsleep) {
				sys::sleep::set_adc(true);
			}
			
			i2c::buff::write_u16(value);
			i2c::interrupt::en();
		}


		const void analog_write() {
			if (!i2c::buff::wait_count_bytes(2)) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			const uint8_t v = i2c::buff::read_u8();
			
			analogWrite(pin, (int)v);
			
			i2c::interrupt::en();
		}

		const void analog_write_16bit() {
			if (!i2c::buff::wait_count_bytes(1 + i2c::buff::get_buff_u16_length())) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			const uint16_t value = i2c::buff::read_u16();
			
			//analogWrite(pin, rdata.u16);
			if (pin == 9) {
				OCR1A = value;
			}else
			if (pin == 10) {
				OCR1B = value;
			}else {
				analogWrite(pin, value);
			}
			
			i2c::interrupt::en();
		}

	}
}