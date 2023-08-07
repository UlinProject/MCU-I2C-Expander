#pragma once

#define SLEEP_PIN 3

namespace sys {
	namespace sleep {
		//static uint8_t a_cpu_sleep_mode;
		extern bool is_cpusleep;
		extern bool is_adcsleep;

		const void init();
		
		const void set_adc(const bool is_sleep = true);
		const void set_cpu(const bool is_sleep = false);
		void pin_interrupt();
		
		const void loop_event_detect_sleep();
		const void start_and_detect_sleep();
	}
}