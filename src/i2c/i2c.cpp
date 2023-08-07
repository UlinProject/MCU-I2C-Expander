#include "i2c/i2c.h"
#include "i2c/buff.h"
#include "sys/uyield.h"
#include "sys/sleep.h"
#include "i2c/interrupt.h"
#include "buttons.h"
#include "dallas.h"
#include "i2c_events/i2c_events.h"
#include <EEPROM.h>

using namespace dallas;
using namespace buttons;

namespace i2c {
	TwoWire wire_current;
	uint8_t high_write_len;
	
	const void init() {
		high_write_len = 0;
		
		i2c::wire_current.begin(I2C_ADRESS);								// join i2c bus with address #8
		i2c::wire_current.setClock(100000L);
		//TWAR = (I2C_ADRESS << 1) | 1; 
		
		i2c::interrupt::init();
		
		i2c::wire_current.onReceive(receiveEvent);
		i2c::wire_current.onRequest(requestEvent); // register event
	}

	void requestEvent() {
		if (high_write_len != 0) {
			const uint8_t senda[2] = {'~', high_write_len};
			high_write_len = 0;
	
			i2c::wire_current.write(senda, 2);
			return;
		}
		if (i2c::buff::a_send_buff == 0) {
			i2c::wire_current.write(255);
			return;
		}
		i2c::buff::a_send_buff -= i2c::wire_current.write(
			i2c::buff::i2c_send_buff, 
			i2c::buff::a_send_buff
		);
	}
	
	void receiveEvent(int howMany) {
		//digitalWrite(DIG_PIN, !digitalRead(DIG_PIN));
		if (howMany >= 2) {
			//i2c_events::wdt::reset();
			sys::uyield::next();
			while (i2c::buff::read_u8() != (uint8_t)'~') {
				sys::uyield::next();
				if (i2c::wire_current.available() <= 0) {
					return;
				}
				
				sys::uyield::next();
				continue;
				//return;
			}
			
			if (!i2c::buff::wait_count_bytes(1)) {
				return;
			}
			const uint8_t readc = i2c::buff::read_u8();
			i2c_events::wdt::reset();
			
			if (readc == TRecv::I2CSetSpeed) {
				i2c_events::set_speed::i2c_set_speed();
				
				return;
			}else
			if (readc == TRecv::GetMillis) {
				i2c::buff::write_u32(millis());
				
				i2c::interrupt::en();
				return;
			}else
			if (readc == TRecv::SysPinsRestart) {
				i2c_events::sys_pins_restart::sys_pins_restart();
				return;
			}else
			if (readc == TRecv::SysRestart) { // Restart
				i2c_events::sys_restart::sys_restart();
				return;
			}else
	
			if (readc == TRecv::LED_BUILTIN_ON) { // LED_BUILTIN
				i2c_events::led_builtin::led_builtin_on();
				return;
			}else
	
			if (readc == TRecv::LED_BUILTIN_OFF) { // LED_BUILTIN
				i2c_events::led_builtin::led_builtin_off();
				return;
			}else
	
			/*#if defined (__AVR_ATtiny88__)
				if (readc == TRecv::Read_ADC_Temperature) { // READ TEMPERATURE
					if (is_adcsleep) {
						set_adc(false);
					}
					uyield();
					analogReference(INTERNAL);
					const uint16_t aread = (uint16_t)analogRead(ADC_TEMPERATURE);
					
					if (is_adcsleep) {
						set_adc(true);
					}else {
						set_adc(false);
					}
		
					write_u16(aread);
		
					interrupt_on();
					return;
				}else
			#endif*/
			
			if (readc == TRecv::AnalogRead) { // analogRead
				i2c_events::analog::analog_read();
				
				return;
			}else
			if (readc == TRecv::INTERRUPT_ON) {
				i2c::interrupt::en();
				
				return;
			}else
			if (readc == TRecv::INTERRUPT_OFF) {
				i2c::interrupt::dis();
				
				return;
			}else
			if (readc == TRecv::PinModeOutput) {
				i2c_events::pin_mode::pin_mode_output();
				
				return;
			}else
			
			if (readc == TRecv::PinModeInput) {
				i2c_events::pin_mode::pin_mode_input();
				
				return;
			}else
			if (readc == TRecv::PinModeInputPullup) {
				i2c_events::pin_mode::pin_mode_input_pullup();
				
				return;
			}else
	
			if (readc == TRecv::PinTimerSetFreq) {
				i2c_events::pin_mode::pin_timer_setfreq();
				
				return;
			}else
	
			if (readc == TRecv::DigitalWriteHigh) { // HIGH
				i2c_events::digital::digital_write_high();
				
				return;
			}else
			if (readc == TRecv::DigitalWriteLow) { // LOW
				i2c_events::digital::digital_write_low();
				
				return;
			}else
	
			if (readc == TRecv::AutoSleepAdcOn) { // DIS ADC
				if (!sys::sleep::is_adcsleep) {
					sys::sleep::is_adcsleep = true;
					
					sys::sleep::set_adc(true);
				}
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::AutoSleepAdcOff) { // EN ADC
				if (sys::sleep::is_adcsleep) {
					sys::sleep::is_adcsleep = false;
					
					sys::sleep::set_adc(false);
				}
				
				i2c::interrupt::en();
				
				return;
			}else
			
			if (readc == TRecv::AnalogWrite) { // analogWrite
				i2c_events::analog::analog_write();
				
				return;
			}else
	
			if (readc == TRecv::AnalogWrite16Bit) { // analogWrite
				i2c_events::analog::analog_write_16bit();
				
				return;
			}else
			
	
			if (readc == TRecv::FlushMem) {
				i2c_events::sys_mem::flush_mem();
				
				return;
			}else
	
			if (readc == TRecv::GetSizeMem) {
				const uint16_t max = sizeof(i2c_events::sys_mem::i2c_sys_mem) / sizeof(uint8_t);
				i2c::buff::write_u16(max); // i2c_events::sys_mem::i2c_sys_mem_length
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::GetMem) {
				i2c_events::sys_mem::get_mem();
				
				return;
			}else
	
			if (readc == TRecv::GetMem16BitAdress) {
				i2c_events::sys_mem::get_mem_16bit();
				
				return;
			}else
	
			if (readc == TRecv::SetMem16BitAdress) {
				i2c_events::sys_mem::set_mem_16bit();
				
				return;
			}else
	
			if (readc == TRecv::SetMem) {
				i2c_events::sys_mem::set_mem();
				
				return;
			}else
	
			if (readc == TRecv::DefCPU_PINSLEEP_Ignore) {
				//sys::sleep::a_cpu_sleep_mode = 0b00000000;
				sys::sleep::set_cpu(false);
				sys::sleep::set_adc(false);
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DefCPU_PINSLEEP_Idle) {
				//sys::sleep::a_cpu_sleep_mode = 0b00000001;
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DefCPU_PINSLEEP_ADC) {
				//sys::sleep::a_cpu_sleep_mode = 0b00000011;
				sys::sleep::set_adc(true);
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DefCPU_PINSLEEP_PowerDown) {
				//sys::sleep::a_cpu_sleep_mode = 0b00000101;
				sys::sleep::set_cpu(true);
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::CPU_SLEEP_ON) {
				sys::sleep::set_cpu(true);
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::CPU_SLEEP_OFF) {
				sys::sleep::set_cpu(false);
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::EFlushMem) {
				uint16_t i = 0;
				while (i < EEPROM.length()) {
					EEPROM.put(i, 0);
					i += 1;
					sys::uyield::next();
				}
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::EGetSizeMem) {
				i2c::buff::write_u16(EEPROM.length());
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::EGetMem) {
				if (!i2c::buff::wait_count_bytes(i2c::buff::get_buff_u16_length())) {
					return;
				}
				
				const uint16_t address = i2c::buff::read_u16();
				if (address > EEPROM.length()) {
					i2c::buff::write_u8(255);
				}else {
					const uint8_t dataread = EEPROM.read(address);
					i2c::buff::write_u8(dataread);
				}
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::ESetMem) {
				if (!i2c::buff::wait_count_bytes(1 + i2c::buff::get_buff_u16_length())) {
					return;
				}
				const uint16_t address = i2c::buff::read_u16();
				const uint8_t value = i2c::buff::read_u8();
				
				if (address > EEPROM.length()) {
					
				}else {
					EEPROM.put(address, value);
				}
				
				i2c::interrupt::en();
				return;
			}else
	
			
	
			if (readc == TRecv::DallasWireInit) {
				if (!i2c::buff::wait_count_bytes(2)) {
					return;
				}
				const uint8_t num_wire = i2c::buff::read_u8();
				const uint8_t ctemp_wire = i2c::buff::read_u8();
	
				bool is_ok = false;
				bool is_init = false;
				const uint8_t max = sizeof(all_dallas_wire) / sizeof(DallasWire);
				if (all_dallaswire_init < max) {
					uint8_t i = 0;
					bool is_continue = true;
	
					if (num_wire != 0) {
						i = 0;
						while (i < max) {
							const DallasWire* adallwire = &all_dallas_wire[i];
							sys::uyield::next();
			
							if (adallwire->num_pin == num_wire) {
								is_continue = false;
								break;
							}
							
							i += 1;
						}
					}
	
					if (is_continue) {
						i = 0;
						while (i < max) {
							DallasWire* adallwire = &all_dallas_wire[i];
							sys::uyield::next();
			
							if (!adallwire->is_init) {
								is_init = adallwire->init(num_wire, ctemp_wire);
								is_ok = true;
								break;
							}
							
							i += 1;
						}
					}
				}
				if (is_ok) {
					if (is_init) {
						i2c::buff::write_u8(2);
					}else {
						i2c::buff::write_u8(1);
					}
				}else {
					i2c::buff::write_u8(0);
				}
	
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DallasWireAllLength) {
				const uint8_t max = sizeof(all_dallas_wire) / sizeof(DallasWire);
				i2c::buff::write_u8(max);
				
				i2c::interrupt::en();
				return;
			}else
			if (readc == TRecv::DallasWireInitLength) {
				i2c::buff::write_u8(all_dallaswire_init);
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DallasInit) {
				if (!i2c::buff::wait_count_bytes(2)) {
					return;
				}
				const uint8_t num_wire = i2c::buff::read_u8();
				const uint8_t num_dallas = i2c::buff::read_u8();
	
				bool is_ok = false;
				if (all_dallaswire_init > 0) {
					uint8_t i = 0;
					const uint8_t max = sizeof(all_dallas_wire) / sizeof(DallasWire);
					while (i < max) {
						DallasWire* adallwire = &all_dallas_wire[i];
		
						if (adallwire->is_init) {
							if(adallwire->num_pin == num_wire) {
								if (adallwire->dallas_len < adallwire->get_max_all_dallas()) {
									adallwire->push_dallas(num_dallas);
									is_ok = true;
								}
							}
						}
						
						sys::uyield::next();
						i += 1;
					}
				}
				if (is_ok) {
					i2c::buff::write_u8(1);
				}else {
					i2c::buff::write_u8(0);
				}
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DallasAllInitLength) {
				i2c::buff::write_u8(dallas::all_dallas_init);
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DallasWireSearch) {
				if (!i2c::buff::wait_count_bytes(1)) {
					return;
				}
				const uint8_t num_wire = i2c::buff::read_u8();
	
				uint8_t result = 0;
				if (all_dallaswire_init > 0) {
					uint8_t i = 0;
					const uint8_t max = sizeof(all_dallas_wire) / sizeof(DallasWire);
					while (i < max) {
						sys::uyield::next();
						DallasWire* adallwire = &all_dallas_wire[i];
		
						if (adallwire->is_init) {
							if(adallwire->num_pin == num_wire) {
								result = adallwire->search_and_next();
								break;
							}
						}
						
						i += 1;
					}
				}
				i2c::buff::write_u8(result);
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::DallasGet) {
				if (!i2c::buff::wait_count_bytes(2)) {
					return;
				}
				const uint8_t num_wire = i2c::buff::read_u8();
				const uint8_t num_dallas = i2c::buff::read_u8();
	
				bool is_ok = false;
				float result = 0;
				if (all_dallaswire_init > 0) {
					uint8_t i = 0;
					const uint8_t max = sizeof(all_dallas_wire) / sizeof(DallasWire);
					while (i < max) {
						DallasWire* adallwire = &all_dallas_wire[i];
		
						if (adallwire->is_init) {
							if(adallwire->num_pin == num_wire) {
								i = 0;
								while (i < adallwire->dallas_len) {
									Dallas* adall = &adallwire->all_dallas[i];
									if (adall->is_init()) {
										if (adall->num_dallas == num_dallas) {
											if (adall->ctemp > 0) {
												float calc = adall->calc_temp();
												result = calc;
												is_ok = true;
											}
											
											break;
										}
									}
									
									i += 1;
									sys::uyield::next();
								}
								break;
							}
						}
						
						i += 1;
						sys::uyield::next();
					}
				}
				if (!is_ok) {
					i2c::buff::write_float(255.0);
				}else {
					i2c::buff::write_float(result);
				}
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::ClearButtons) {
				if (all_rbuttons > 0) {
	
					uint8_t call_rbuttons = all_rbuttons;
					all_rbuttons = 0;
					
					uint8_t i = 0;
					const uint16_t max = sizeof(all_buttons) / sizeof(Button);
					Button* ab;
					while (i < max) {
						sys::uyield::next();
						ab = &all_buttons[i];
						i += 1;
		
						if (ab->num != 255) {
							ab->destruct();
							call_rbuttons -= 1;
						}
						if (call_rbuttons == 0) {
							break;
						}
					}
					is_update_buttons = false;
				}
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::ButtonRegRising || readc == TRecv::ButtonRegChange || readc == TRecv::ButtonRegFalling) {
				if (!i2c::buff::wait_count_bytes(1)) {
					return;
				}
				const uint8_t pin = i2c::buff::read_u8();
				const uint16_t max = sizeof(all_buttons) / sizeof(Button);
				
				if (pin != 255) {
					bool is_continue = true;
					uint8_t i = 0;
					Button* ab;
					if (all_rbuttons > 0) {
						uint8_t call_rbuttons = all_rbuttons;
						while (i < max) {
							ab = &all_buttons[i];
		
							if (ab->num == pin) {
								is_continue = false;
								break;
							}
		
							call_rbuttons -= 1;
							if (call_rbuttons == 0) {
								break;
							}
							i += 1;
						}
					}
					
					if (is_continue) {
						i = 0;
						
						while (i < max) {
							ab = &all_buttons[i];
							i += 1;
			
							if (ab->num == 255) {
								ab->num = pin;
			
								/*uint8_t readt;
								if (readc == TRecv::ButtonRegRising) {
									readt = 'R';
								}else
								if (readc == TRecv::ButtonRegChange) {
									readt = 'C';
								}else
								if (readc == TRecv::ButtonRegFalling) {
									readt = 'F';
								}else {
									readt = 'C';
								}*/
		
								all_rbuttons = all_rbuttons + 1;
								ab->reg(readc);
								
								break;
							}
						}
					}
				}
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::ButtonInterruptOff) {
				if (!i2c::buff::wait_count_bytes(1)) {
					return;
				}
				const uint8_t pin = i2c::buff::read_u8();
				const uint16_t max = sizeof(all_buttons) / sizeof(Button);
	
				if (pin != 255) {
					if (all_rbuttons > 0) {
						uint8_t i = 0;
						Button* ab;
						while (i < max) {
							ab = &all_buttons[i];
							i += 1;
			
							if (ab->num == pin) {
								all_rbuttons = all_rbuttons - 1;
								ab->destruct();
								
								break;
							}
						}
					}
				}
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::GetCountButtons) {
				i2c::buff::write_u8(all_rbuttons);
				
				i2c::interrupt::en();
				return;
			}else
	
			if (readc == TRecv::ReadButtons) {
				if (all_rbuttons > 0 && is_update_buttons) {
					is_update_buttons = false;
	
					uint8_t cnofreebuttons = 0;
					uint8_t i = 0;
					const uint16_t max = sizeof(all_buttons) / sizeof(Button);
					Button* ab;
					while (i < max) {
						ab = &all_buttons[i];
						i += 1;
	
						if (ab->cupdate > 0) {
							cnofreebuttons += 1;
						}
						sys::uyield::next();
					}
					//write_u8(cnofreebuttons);
					high_write_len = cnofreebuttons;
	
					if (cnofreebuttons > 0) {
						i = 0;
						while (i < max) {
							ab = &all_buttons[i];
							i += 1;
			
							if (ab->cupdate > 0) {
								const uint8_t cupdate = ab->get_cupdate_and_flush();
								
								// TODO IGNORE HEAD AND i2c::buff::write_twou8(ab->num, cupdate);
								i2c::buff::i2c_send_buff[i2c::buff::a_send_buff] = ab->num;
								i2c::buff::a_send_buff += 1;
								i2c::buff::i2c_send_buff[i2c::buff::a_send_buff] = cupdate;
								i2c::buff::a_send_buff += 1;
	
								cnofreebuttons = cnofreebuttons - 1;
								if (cnofreebuttons == 0) {
									break;
								}
							}
							sys::uyield::next();
						}
					}
				}else {
					i2c::buff::write_u8(0);
				}
	
				
				i2c::interrupt::en();
				return;
			}else 
			
			if (readc == TRecv::WatchDogInit) {
				if (!i2c::buff::wait_count_bytes(2 + i2c::buff::get_buff_u16_length())) {
					return;
				}
				const uint8_t pin = i2c::buff::read_u8();
				const bool is_onemodreboot = i2c::buff::read_u8() == (uint8_t)1;
				const uint16_t time = i2c::buff::read_u16();
				
				pinMode(pin, OUTPUT); // INVERS
				digitalWrite(pin, HIGH);
				i2c_events::wdt::en_set_trig_pin(pin, time);
				i2c_events::wdt::set_is_onemode_reboot(is_onemodreboot);
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::WatchDogDis) {
				i2c_events::wdt::disable();
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::WatchDogTrig) {
				i2c_events::wdt::wdt_trig(millis());
				//clear
				i2c_events::wdt::is_old_trig_and_clear();
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::WatchDogIsOldTrig) {
				if (i2c_events::wdt::is_old_trig_and_clear()) {
					i2c::buff::write_u8(1);
				}else {
					i2c::buff::write_u8(0);
				}
				
				i2c::interrupt::en();
				return;
			}else
			
			if (readc == TRecv::WatchDogUpdate) {
				i2c_events::wdt::reset();
				
				i2c::interrupt::en();
				return;
			}
		}
	}
}
