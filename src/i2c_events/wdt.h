#pragma once
#include <Arduino.h>

namespace i2c_events {
	namespace wdt {
		extern bool is_en;
		extern bool is_old_trig;
		extern bool is_onemode_reboot;
		extern uint32_t wdt_time;
		extern uint16_t exp_time;
		extern void(*trigger)();
		extern uint8_t pin_trigg;
		
		const void disable();
		
		const void set_is_onemode_reboot(const bool is);
		const void en_set_trig_pin(const uint8_t pin, const uint16_t time);
		const void en_set_trigger(void* trig_fn);
		
		const void init();
		const void reset();
		const void wdt_trig(const uint32_t cmillis);
		
		const bool check_and_trigg();
		const bool is_old_trig_and_clear();
	}
}
