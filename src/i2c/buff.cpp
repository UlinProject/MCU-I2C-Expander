
#include "i2c/buff.h"
#include "i2c/i2c.h"
#include "sys/uyield.h"

namespace i2c {
	namespace buff {
		#if defined (__AVR_ATtiny88__)
			uint8_t i2c_send_buff[56];
		#else
			uint8_t i2c_send_buff[256];
		#endif
		uint8_t a_send_buff;
		
		const void init() {
			a_send_buff = 255;

			// INIT_BUFF
			for (uint8_t& a: i2c_send_buff) {
				a = '~';
			}
			
			
			uint16_t i = 0;
			const uint16_t max = sizeof(i2c_send_buff) / sizeof(uint8_t);
			while (i < max) {
				i2c_send_buff[i] = '~';
				sys::uyield::next();
				i += 1;
			}
			sys::uyield::next();
		}
		
		const void clear() {
			a_send_buff = 0;
			high_write_len = 0;
		}
		
		const bool wait_count_bytes(const uint16_t cwait) {
			uint16_t count_delay = 0;
			while ((uint16_t)i2c::wire_current.available() < cwait) {
				sys::uyield::next();
				
				if (count_delay > 2000) {
					return false;
				}
				count_delay += 1;
			}
			return true;
		}
		
		const uint16_t get_buff_u16_length() {
			return 2;
		}
		const uint16_t get_buff_u32_length() {
			return 4;
		}
		const uint16_t get_buff_float_length() {
			return 4;
		}
		
		const void inline I2CBuffU16::read_wire() {
			this->rawbytes[0] = read_u8();
			this->rawbytes[1] = read_u8();
		}
		
		const void inline I2CBuffU32::read_wire() {
			this->rawbytes[0] = read_u8();
			this->rawbytes[1] = read_u8();
			this->rawbytes[2] = read_u8();
			this->rawbytes[3] = read_u8();
		}
		
		const void inline I2CBuffFloat::read_wire() {
			this->rawbytes[0] = read_u8();
			this->rawbytes[1] = read_u8();
			this->rawbytes[2] = read_u8();
			this->rawbytes[3] = read_u8();
		}
		
		const void write_float(const float value) {
			I2CBuffFloat rdata;
			rdata.nfloat = value;
			
			i2c_send_buff[a_send_buff] = '~';
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[0];
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[1];
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[2];
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[3];
			a_send_buff += 1;
		}
		
		const void write_u32(const uint32_t value) {
			I2CBuffU32 rdata;
			rdata.u32 = value;
			
			i2c_send_buff[a_send_buff] = '~';
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[0];
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[1];
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[2];
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[3];
			a_send_buff += 1;
		}
		
		const void write_u16(const uint16_t value) {
			I2CBuffU16 rdata;
			rdata.u16 = value;
			
			i2c_send_buff[a_send_buff] = '~';
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[0];
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = rdata.rawbytes[1];
			a_send_buff += 1;
		}
		
		const void write_twou8(const uint8_t value1, const uint8_t value2) {
			i2c_send_buff[a_send_buff] = '~';
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = value1;
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = value2;
			a_send_buff += 1;
		}
		
		const void write_u8(const uint8_t value) {
			i2c_send_buff[a_send_buff] = '~';
			a_send_buff += 1;
			i2c_send_buff[a_send_buff] = value;
			a_send_buff += 1;
		}
		
		
		const float read_float() {
			I2CBuffFloat result;
			result.read_wire();
			
			return result.nfloat;
		}
		const uint32_t read_u32() {
			I2CBuffU32 result;
			result.read_wire();
			
			return result.u32;
		}
		const uint16_t read_u16() {
			I2CBuffU16 result;
			result.read_wire();
			
			return result.u16;
		}
		
		const uint8_t read_u8() {
			return (uint8_t)(char)i2c::wire_current.read();
		}
	}
}