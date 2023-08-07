#include "dallas.h"

namespace dallas {
	DallasWire all_dallas_wire[4];
	uint8_t all_dallas_init;
	uint8_t all_dallaswire_init;
	
	const void init() {
		all_dallas_init = 0;
		all_dallaswire_init = 0;
		
		uint8_t i = 0;
		const uint8_t max = sizeof(all_dallas_wire) / sizeof(DallasWire);
		while (i < max) {
			all_dallas_wire[i] = DallasWire();
			sys::uyield::next();
			
			i += 1;
		}
	}
	
	// DALLAS
	Dallas::Dallas() {
		this->_is_init = false;
		this->_is_connect = false;
	}
	
	Dallas::Dallas(const uint8_t num_dallas) {
		this->_is_init = true;
		this->_is_connect = false;
		this->num_dallas = num_dallas;
		
		this->flush_temp();
	}
	const void Dallas::flush_temp() {
		this->temp = 0;
		this->ctemp = 0;
	}
	const uint8_t Dallas::get_num_dallas() {
		return this->num_dallas;
	}
	const void Dallas::connect() {
		if (!this->is_connect()) {
			all_dallas_init += 1;
			this->_is_connect = true;
		}
	}
	const void Dallas::disconnect() {
		if (this->is_connect()) {
			all_dallas_init -= 1;
			this->_is_connect = false;
		}
	}
	const void Dallas::push_temp(const double temp) {
		this->ctemp += 1;
		this->temp += temp;
	}
	const uint8_t Dallas::get_ctemp() {
		return this->ctemp;
	}
	const float Dallas::calc_temp() {
		if (this->ctemp == 0) {
			return 0;
		}
		return this->temp / this->ctemp;
	}
	const bool Dallas::is_init() {
		return this->_is_init;
	}
	const bool Dallas::is_connect() {
		return this->_is_connect;
	}
	
	// Wire
	DallasWire::DallasWire() {
		this->is_init = false;
		this->dallas_len = 0;
		this->num_pin = 0;
		this->exp_ctemp_search = 0;
		
		uint8_t i = 0;
		const uint8_t max_all_dallas = sizeof(this->all_dallas) / sizeof(Dallas);
		
		while (i < max_all_dallas) {
			this->all_dallas[i] = Dallas();
			
			i += 1;
		}
	}
	const uint8_t DallasWire::get_max_all_dallas() {
		return sizeof(this->all_dallas) / sizeof(Dallas);
	}
	const bool DallasWire::init(const uint8_t num, const uint8_t exp_ctemp_search) {
		this->oneWire = OneWire(num);
		this->sensors = DallasTemperature(&this->oneWire);
		this->num_pin = num;
		this->exp_ctemp_search = exp_ctemp_search;
		this->is_init = true;
		all_dallaswire_init += 1;

		return this->resetwiredallas_and_getlen();
	}
	const bool DallasWire::push_dallas(const uint8_t num_dallas) {
		uint8_t i = 0;
		const uint8_t max_all_dallas = sizeof(this->all_dallas) / sizeof(Dallas);

		i = 0;
		while (i < max_all_dallas) {
			Dallas* acurrent = &this->all_dallas[i];
			if (acurrent->is_init()) {
				if (acurrent->num_dallas == num_dallas) {
					return false;
				}
			}
			
			i += 1;
		}


		i = 0;
		while (i < max_all_dallas) {
			Dallas* acurrent = &this->all_dallas[i];
			if (!acurrent->is_init()) {
				this->all_dallas[i] = Dallas(num_dallas);
				
				all_dallas_init += 1;
				this->dallas_len += 1;
				
				return true;
			}
			
			i += 1;
		}
		return false;
	}
	const bool DallasWire::resetwiredallas_and_getlen() {
		this->sensors.begin();
			
		if (sensors.getDS18Count() != 0) {
			this->sensors.setResolution(12);
			this->sensors.setCheckForConversion(true);
			this->sensors.setWaitForConversion(false);

			return true;
		}
		return false;
	}
	const uint8_t DallasWire::get_real_dallas_len() {
		return this->sensors.getDS18Count();
	}
	const uint8_t DallasWire::search_and_next() {
		if (this->dallas_len == 0) {
			return 0;
		}
		uint8_t i = 0;
		const uint8_t max_all_dallas = sizeof(this->all_dallas) / sizeof(Dallas);
		uint8_t all_dallasreads = 0;
		uint8_t true_dallas_reads = 0;
		
		while (i < max_all_dallas) {
			Dallas* acurrent = &this->all_dallas[i];

			if (!acurrent->is_init()) {
				continue;
			}

			if (!acurrent->is_connect()) {
				if (this->get_real_dallas_len() != this->dallas_len) {
					if (!resetwiredallas_and_getlen()) {
						return 0;
					}
				}
			}
			acurrent->flush_temp();
			const uint8_t num_dallas = acurrent->get_num_dallas();
			while (this->exp_ctemp_search > acurrent->get_ctemp()) {
				this->sensors.requestTemperaturesByIndex(num_dallas);
	
				uint16_t adelay = 0;
				while (!this->sensors.isConversionComplete()) {
					delay(1);
					adelay += 1;
					sys::uyield::next();
					if (adelay >= 2000) {
						break;
					}
				}
				const double read = this->sensors.getTempCByIndex(num_dallas);
				if (read == DEVICE_DISCONNECTED_C) {
					acurrent->disconnect();
					break;
				}else {
					if (!acurrent->is_connect()) {
						acurrent->connect();
					}

					acurrent->push_temp(read);
				}
				sys::uyield::next();
			}
			if (acurrent->get_ctemp() == this->exp_ctemp_search) {
				//double cread = acurrent->calc_temp();
				true_dallas_reads += 1;
			}
			
			
			all_dallasreads += 1;
			if (all_dallasreads >= this->dallas_len) {
				break;
			}
			
			i += 1;
			sys::uyield::next();
		}

		return true_dallas_reads;
	}
}
