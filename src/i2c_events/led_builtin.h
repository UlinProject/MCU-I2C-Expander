#pragma once

#define DIG_PIN LED_BUILTIN

namespace i2c_events {
	namespace led_builtin {
		const void init();
		const void led_builtin_on();
		const void led_builtin_off();
	}
}