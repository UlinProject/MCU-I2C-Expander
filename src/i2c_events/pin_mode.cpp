
#include "i2c/i2c.h"
#include "sys/uyield.h"
#include <Arduino.h>
#include "i2c/interrupt.h"
#include "i2c/buff.h"

namespace i2c_events {
	namespace pin_mode {
		const void pin_mode_output() {
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			pinMode(pin, OUTPUT);
			
			i2c::interrupt::en();
		}

		const void pin_mode_input() {
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			pinMode(pin, INPUT);
			
			i2c::interrupt::en();
		}

		const void pin_mode_input_pullup() {
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t pin = i2c::buff::read_u8();
			pinMode(pin, INPUT_PULLUP);
			
			i2c::interrupt::en();
		}

		const void pin_timer_setfreq() {
			if (!i2c::buff::wait_count_bytes(2)) {
				return;
			}
			const uint8_t num_timer = i2c::buff::read_u8();
			const uint8_t freq_timer = i2c::buff::read_u8();

			if (num_timer == 0) { // Пины D5 и D6 (Timer 0) - 8 бит
				if (freq_timer == 0) {
					// Пины D5 и D6 - 62.5 кГц
					TCCR0B = 0b00000001;	// x1
					TCCR0A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 1) {
					// Пины D5 и D6 - 31.4 кГц
					TCCR0B = 0b00000001;	// x1
					TCCR0A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 2) {
					// Пины D5 и D6 - 7.8 кГц
					TCCR0B = 0b00000010;	// x8
					TCCR0A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 3) {
					// Пины D5 и D6 - 4 кГц
					TCCR0B = 0b00000010;	// x8
					TCCR0A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 4) {
					// Пины D5 и D6 - 976 Гц - по умолчанию
					TCCR0B = 0b00000011;	// x64
					TCCR0A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 5) {
					// Пины D5 и D6 - 490 Гц
					TCCR0B = 0b00000011;	// x64
					TCCR0A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 6) {
					// Пины D5 и D6 - 244 Гц
					TCCR0B = 0b00000100;	// x256
					TCCR0A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 7) {
					// Пины D5 и D6 - 122 Гц
					TCCR0B = 0b00000100;	// x256
					TCCR0A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 8) {
					// Пины D5 и D6 - 61 Гц
					TCCR0B = 0b00000101;	// x1024
					TCCR0A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 9) {
					// Пины D5 и D6 - 30 Гц
					TCCR0B = 0b00000101;	// x1024
					TCCR0A = 0b00000001;	// phase correct
				}
			}else

			if (num_timer == 1) { // Пины D9 и D10 (Timer 1) - 8 бит
				if (freq_timer == 0) {
					// Пины D9 и D10 - 62.5 кГц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00001001;	// x1 fast pwm
				}else
				if (freq_timer == 1) {
					// Пины D9 и D10 - 31.4 кГц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00000001;	// x1 phase correct
				}else
				if (freq_timer == 2) {
					// Пины D9 и D10 - 7.8 кГц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00001010;	// x8 fast pwm
				}else
				if (freq_timer == 3) {
					// Пины D9 и D10 - 4 кГц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00000010;	// x8 phase correct
				}else
				if (freq_timer == 4) {
					// Пины D9 и D10 - 976 Гц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00001011;	// x64 fast pwm
				}else
				if (freq_timer == 5) {
					// Пины D9 и D10 - 490 Гц - по умолчанию
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00000011;	// x64 phase correct
				}else
				if (freq_timer == 6) {
					// Пины D9 и D10 - 244 Гц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00001100;	// x256 fast pwm
				}else
				if (freq_timer == 7) {
					// Пины D9 и D10 - 122 Гц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00000100;	// x256 phase correct
				}else
				if (freq_timer == 8) {
					// Пины D9 и D10 - 61 Гц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00001101;	// x1024 fast pwm
				}else
				if (freq_timer == 9) {
					// Пины D9 и D10 - 30 Гц
					TCCR1A = 0b00000001;	// 8bit
					TCCR1B = 0b00000101;	// x1024 phase correct
				}
			}else

			if (num_timer == 2) { // Пины D9 и D10 (Timer 1) - 10 бит
				if (freq_timer == 0) {
					// Пины D9 и D10 - 15.6 кГц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00001001;	// x1 fast pwm
				}else
				if (freq_timer == 1) {
					// Пины D9 и D10 - 7.8 кГц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00000001;	// x1 phase correct
				}else
				if (freq_timer == 2) {
					// Пины D9 и D10 - 2 кГц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00001010;	// x8 fast pwm
				}else
				if (freq_timer == 3) {
					// Пины D9 и D10 - 977 Гц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00000010;	// x8 phase correct
				}else
				if (freq_timer == 4) {
					// Пины D9 и D10 - 244 Гц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00001011;	// x64 fast pwm
				}else
				if (freq_timer == 5) {
					// Пины D9 и D10 - 122 Гц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00000011;	// x64 phase correct
				}else
				if (freq_timer == 6) {
					// Пины D9 и D10 - 61 Гц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00001100;	// x256 fast pwm
				}else
				if (freq_timer == 7) {
					// Пины D9 и D10 - 30 Гц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00000100;	// x256 phase correct
				}else
				if (freq_timer == 8) {
					// Пины D9 и D10 - 15 Гц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00001101;	// x1024 fast pwm
				}else
				if (freq_timer == 9) {
					// Пины D9 и D10 - 7.5 Гц 10bit
					TCCR1A = 0b00000011;	// 10bit
					TCCR1B = 0b00000101;	// x1024 phase correct
				}
				
			}else

			if (num_timer == 3) { // Пины D3 и D11 (Timer 2) - 8 бит
				if (freq_timer == 0) {
					// Пины D3 и D11 - 62.5 кГц
					TCCR2B = 0b00000001;	// x1
					TCCR2A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 1) {
					// Пины D3 и D11 - 31.4 кГц
					TCCR2B = 0b00000001;	// x1
					TCCR2A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 2) {
					// Пины D3 и D11 - 8 кГц
					TCCR2B = 0b00000010;	// x8
					TCCR2A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 3) {
					// Пины D3 и D11 - 4 кГц
					TCCR2B = 0b00000010;	// x8
					TCCR2A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 4) {
					// Пины D3 и D11 - 2 кГц
					TCCR2B = 0b00000011;	// x32
					TCCR2A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 5) {
					// Пины D3 и D11 - 980 Гц
					TCCR2B = 0b00000011;	// x32
					TCCR2A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 6) {
					// Пины D3 и D11 - 980 Гц
					TCCR2B = 0b00000100;	// x64
					TCCR2A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 7) {
					// Пины D3 и D11 - 490 Гц - по умолчанию
					TCCR2B = 0b00000100;	// x64
					TCCR2A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 8) {
					// Пины D3 и D11 - 490 Гц
					TCCR2B = 0b00000101;	// x128
					TCCR2A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 9) {
					// Пины D3 и D11 - 245 Гц
					TCCR2B = 0b00000101;	// x128
					TCCR2A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 10) {
					// Пины D3 и D11 - 245 Гц
					TCCR2B = 0b00000110;	// x256
					TCCR2A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 11) {
					// Пины D3 и D11 - 122 Гц
					TCCR2B = 0b00000110;	// x256
					TCCR2A = 0b00000001;	// phase correct
				}else
				if (freq_timer == 12) {
					// Пины D3 и D11 - 60 Гц
					TCCR2B = 0b00000111;	// x1024
					TCCR2A = 0b00000011;	// fast pwm
				}else
				if (freq_timer == 13) {
					// Пины D3 и D11 - 30 Гц
					TCCR2B = 0b00000111;	// x1024
					TCCR2A = 0b00000001;	// phase correct
				}
			}
			
			i2c::interrupt::en();
		}
	}
}