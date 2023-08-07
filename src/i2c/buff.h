#pragma once

#include <Arduino.h>

namespace i2c {
	namespace buff {
		#if defined (__AVR_ATtiny88__)
			extern uint8_t i2c_send_buff[56];
		#else
			extern uint8_t i2c_send_buff[256];
		#endif
		extern uint8_t a_send_buff;
		
		const bool wait_count_bytes(const uint16_t cwait);
		
		union I2CBuffU16 {
			uint16_t u16;
			uint8_t rawbytes[2];
			
			const void inline read_wire();
		};
		const uint16_t get_buff_u16_length();
		
		union I2CBuffU32 {
			uint32_t u32;
			uint8_t rawbytes[4];
			
			const void inline read_wire();
		};
		const uint16_t get_buff_u32_length();
		
		union I2CBuffFloat {
			float nfloat;
			uint8_t rawbytes[4];
			
			const void inline read_wire();
		};
		const uint16_t get_buff_float_length();
		
		const void init();
		
		const void write_float(const float value);
		const void write_u32(const uint32_t value);
		const void write_u16(const uint16_t value);
		const void write_twou8(const uint8_t value1, const uint8_t value2);
		const void write_u8(const uint8_t value);
		
		const void clear();
		
		const float read_float();
		const uint32_t read_u32();
		const uint16_t read_u16();
		const uint8_t read_u8();
	}
}