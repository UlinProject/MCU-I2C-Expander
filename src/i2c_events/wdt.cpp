#include "i2c_events/wdt.h"
#include "i2c/buff.h"
#include "sys/uyield.h"

namespace i2c_events {
	namespace wdt {
		bool is_en;
		bool is_old_trig;
		bool is_onemode_reboot;
		uint32_t wdt_time;
		uint16_t exp_time;
		void(*trigger)();
		uint8_t pin_trigg;
		
		const void init() {
			is_en = false;
			wdt_time = 0;
			exp_time = 0;
			pin_trigg = 0;
			trigger = NULL;
			is_old_trig = false;
			is_onemode_reboot = true;
		}
		
		const void disable() {
			is_en = false;
		}
		
		const void set_is_onemode_reboot(const bool is) {
			is_onemode_reboot = is;
		}
		
		const void en_set_trig_pin(const uint8_t pin, const uint16_t time) {
			pin_trigg = pin;
			exp_time = time;
			is_en = true;
			is_onemode_reboot = true;
			is_old_trig = false;
			
			wdt_time = millis() + (uint32_t)exp_time;
		}
		
		const void reset() {
			if (!is_en) {
				return;
			}
			wdt_time = millis() + (uint32_t)exp_time;
		}
		
		const void en_set_trigger(void(*trig_fn)()) {
			trigger = trig_fn;
			is_en = true;
		}
		
		const bool is_old_trig_and_clear() {
			if (is_old_trig) {
				is_old_trig = false;
				
				return true;
			}
			return false;
		}
		
		const bool check_and_trigg() {
			if (!is_en || (is_onemode_reboot && is_old_trig)) {
				return false;
			}
			const uint32_t cmillis = millis();
			if (cmillis > wdt_time) {
				wdt_trig(cmillis);
				
				return true;
			}
			return false;
		}
		
		const void wdt_trig(const uint32_t cmillis) {
			wdt_time = cmillis + (uint32_t)exp_time;
			is_old_trig = true;
			i2c::buff::clear();
			sys::uyield::next();
			
			if (trigger != NULL) {
				(trigger)();
			}
			if (pin_trigg != 0) {
				digitalWrite(LED_BUILTIN, HIGH);
				
				digitalWrite(pin_trigg, LOW);
				sys::uyield::next();
				delay(5000);
				digitalWrite(pin_trigg, HIGH);
				sys::uyield::next();
				delay(2000);
				digitalWrite(pin_trigg, LOW);
				sys::uyield::next();
				delay(5000);
				sys::uyield::next();
				digitalWrite(pin_trigg, HIGH);
				
				digitalWrite(LED_BUILTIN, LOW);
			}
		}
	}
}
