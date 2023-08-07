#include <Arduino.h>
#include <avr/sleep.h>
#include "sys/sleep.h"
#include "sys/wdt.h"
#include "sys/uyield.h"
#include "i2c_events/led_builtin.h"

namespace sys {
	namespace sleep {
		bool is_cpusleep;
		bool is_adcsleep;
		
		const void init() {
			is_cpusleep = true;
			is_adcsleep = true;
			//a_cpu_sleep_mode = 0b00000101;
			
			pinMode(SLEEP_PIN, INPUT_PULLUP);
			
			
			set_adc(false);
			set_cpu(false);
			attachInterrupt(
				digitalPinToInterrupt(SLEEP_PIN), 
				pin_interrupt, 
				CHANGE
			);
		}
		
		/*#if defined (__AVR_ATtiny88__)
			const void set_adc(const bool is_sleep = true) {
				if (!is_sleep) {
					ADCSRA |= (1 << ADEN);
					analogReference(DEFAULT);
				}else {
					ADCSRA &= ~(1 << ADEN);
				}
			}
		#else*/
			const void set_adc(const bool is_sleep = true) {
				if (is_adcsleep == is_sleep) {
					return;
				}
				is_cpusleep = !is_cpusleep;
				is_adcsleep = is_sleep;
				
				set_cpu(!is_cpusleep);
			}
		//#endif
		
		/*#if defined (__AVR_ATtiny88__)
			const void set_cpu(const bool is_sleep = false) {
				if (is_cpusleep == is_sleep) {
					return;
				}
				if (is_sleep) {
					SMCR = a_cpu_sleep_mode;
					//digitalWrite(LED_BUILTIN, LOW);
				}else {
					SMCR = 0b00000000;														 // 1,38мА/0,22мА Power-down Mode
					//digitalWrite(LED_BUILTIN, HIGH);
				}
				is_cpusleep = is_sleep;
			}
		#else*/
			const void set_cpu(const bool is_sleep = false) {
				if (is_cpusleep == is_sleep) {
					return;
				}
				if (is_sleep) { // LOWPIN
					set_sleep_mode(SLEEP_MODE_PWR_DOWN);
					//digitalWrite(LED_BUILTIN, LOW);
					
					/*attachInterrupt(
						digitalPinToInterrupt(SLEEP_PIN), 
						pin_interrupt, 
						CHANGE
					);*/
					
				}else {
					if (is_adcsleep) {
						set_sleep_mode(SLEEP_MODE_ADC);
					}else {
						set_sleep_mode(SLEEP_MODE_IDLE);
					}
					//digitalWrite(LED_BUILTIN, HIGH);
					
					//detachInterrupt(digitalPinToInterrupt(SLEEP_PIN));
				}
				is_cpusleep = is_sleep;
			}
		//#endif
		
		
		void pin_interrupt() {
			/*if (!is_cpusleep) {
				return;
			}*/
			set_cpu(digitalRead(SLEEP_PIN) == LOW);
		}
		
		const void loop_event_detect_sleep() {
			if (is_cpusleep) {
				sys::wdt::disable();
				/*#if defined (__AVR_ATtiny88__)
					__asm__ volatile ("sleep\n\t"); // Спящий режим CPU
				#else*/
					sleep_mode();
				//#endif
				sys::wdt::enable();
			}
		}
		
		const void start_and_detect_sleep() {
			while (digitalRead(SLEEP_PIN) == LOW) {
				sys::sleep::set_cpu(true);
				digitalWrite(DIG_PIN, LOW);

				sys::wdt::disable();
				/*#if defined (__AVR_ATtiny88__)
					__asm__ volatile ("sleep\n\t"); // Спящий режим CPU
				#else*/
					sleep_mode();
				//#endif
				sys::wdt::enable();
				
				digitalWrite(DIG_PIN, HIGH);
				sys::uyield::next();
			}
		}
	}
}
