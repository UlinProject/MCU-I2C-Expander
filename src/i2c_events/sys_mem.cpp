
#include "i2c/i2c.h"
#include "sys/uyield.h"
#include "i2c/buff.h"
#include "i2c/interrupt.h"
#include "i2c_events/sys_mem.h"

namespace i2c_events {
	namespace sys_mem {
		/*#if defined (__AVR_ATtiny88__)
			uint8_t i2c_sys_mem[100];
		#else*/
			uint8_t i2c_sys_mem[512];
		//#endif
		
		const void init() {
			uint16_t i = 0;
			const uint16_t max = sizeof(i2c_sys_mem) / sizeof(uint8_t);
			i2c_events::sys_mem::i2c_sys_mem[i] = 'Y';
			i += 1;
			i2c_events::sys_mem::i2c_sys_mem[i] = '0';
			i += 1;
			
			while (i < max) {
				i2c_events::sys_mem::i2c_sys_mem[i] = 0;
				i += 1;
			}
		}
		
		const void flush_mem() {
			uint16_t i = 0;
			const uint16_t max = sizeof(i2c_sys_mem) / sizeof(uint8_t);
			while (i < max) {
				i2c_sys_mem[i] = 0;
		
				sys::uyield::next();
				i += 1;
			}
		
			i2c::interrupt::en();
		}
		
		const void get_mem() {
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t adress = i2c::buff::read_u8();
			
			const uint16_t max = sizeof(i2c_sys_mem) / sizeof(uint8_t);
			if (adress > max) {
				i2c::buff::write_u8(255);
			}else {
				i2c::buff::write_u8(i2c_sys_mem[adress]);
			}
			
			i2c::interrupt::en();
		}

		const void get_mem_16bit() {
			if (!i2c::buff::wait_count_bytes(i2c::buff::get_buff_u16_length())) {
				return;
			}
			
			const uint16_t adress = i2c::buff::read_u16();
			
			const uint16_t max = sizeof(i2c_sys_mem) / sizeof(uint8_t);
			if (adress > max) {
				i2c::buff::write_u8(255);
			}else {
				i2c::buff::write_u8(i2c_sys_mem[adress]);
			}
			
			i2c::interrupt::en();
		}

		const void set_mem_16bit() {
			if (!i2c::buff::wait_count_bytes(i2c::buff::get_buff_u16_length() + 1)) {
				return;
			}

			const uint16_t adress = i2c::buff::read_u16();
			const uint8_t value = i2c::buff::read_u8();
			
			const uint16_t max = sizeof(i2c_sys_mem) / sizeof(uint8_t);
			if (adress > max) {
				
			}else {
				i2c_sys_mem[adress] = value;
			}
			
			i2c::interrupt::en();
		}

		const void set_mem() {
			if (!i2c::buff::wait_count_bytes(2)) {
				return;
			}
			const uint8_t adress = i2c::buff::read_u8();
			const uint8_t value = i2c::buff::read_u8();
			
			const uint16_t max = sizeof(i2c_sys_mem) / sizeof(uint8_t);
			if (adress > max) {
				
			}else {
				i2c_sys_mem[adress] = value;
			}
			
			i2c::interrupt::en();
		}
	}
}