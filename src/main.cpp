#include <Arduino.h>

#define ALWAYS_INLINE __inline__ __attribute__ ((__always_inline__))

// 9 PIN DISPLAY BL
// 3 PIN SLEEP <--IG
// 4 PIN RESET DISPLAY

#include <Arduino.h>
#include "sys/uyield.h"
#include "dallas.h"
#include "buttons.h"
#include "sys/sleep.h"
#include "sys/wdt.h"

#include "i2c/buff.h"
#include "i2c/i2c.h"
#include "i2c/interrupt.h"
#include "i2c_events/i2c_events.h"
#include <avr/sleep.h>

using namespace buttons;
using namespace dallas;


void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	i2c::init();
	i2c_events::led_builtin::init();
	pinMode(5, OUTPUT); // TODO WDT PIN
	digitalWrite(5, LOW);

	digitalWrite(DIG_PIN, HIGH);
	i2c::interrupt::dis();
	sys::wdt::enable();
	sys::sleep::init();
	sys::sleep::start_and_detect_sleep();
	sys::uyield::next();

	// INIT_BUFF
	i2c::buff::init();
	// SYSMEMORY
	i2c_events::sys_mem::init();
	i2c_events::wdt::init();
	// INIT EVENTS
	i2c_events::init();

	// INIT ALLBUTTONS
	buttons::init();
	// INIT DALLAS
	dallas::init();

	digitalWrite(DIG_PIN, LOW); // register event
	i2c::interrupt::dis();
	
	delay(2000);
	digitalWrite(5, HIGH);
}

void loop() {
	while (true) {
		sys::sleep::loop_event_detect_sleep();
		sys::uyield::next();
		if (i2c_events::wdt::check_and_trigg()) {
			sys::uyield::next();
		}
		
	}
}
