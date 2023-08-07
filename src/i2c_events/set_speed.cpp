
#include "i2c/i2c.h"
#include "sys/uyield.h"
#include "i2c/buff.h"
#include "i2c/interrupt.h"

namespace i2c_events {
	namespace set_speed {
		const void i2c_set_speed() {
			if (!i2c::buff::wait_count_bytes(i2c::buff::get_buff_u32_length())) {
				return;
			}
			const uint32_t clock_speed = i2c::buff::read_u32();
			i2c::wire_current.setClock(clock_speed);

			i2c::interrupt::en();
		}
	}
}