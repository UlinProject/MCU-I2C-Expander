#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "sys/uyield.h"

namespace dallas {
	extern uint8_t all_dallas_init;
	extern uint8_t all_dallaswire_init;
	
	struct Dallas {
		bool _is_init;
		bool _is_connect;
		
		uint8_t num_dallas;
		float temp;
		float ctemp;
	
		Dallas();
		Dallas(const uint8_t num_dallas);
	
		const void flush_temp();
		const uint8_t get_num_dallas();
	
		const void connect();
		const void disconnect();
	
		const void push_temp(const double temp);
		const uint8_t get_ctemp();
		const float calc_temp();
	
		const bool is_init();
		const bool is_connect();
	};
	
	struct DallasWire {
		bool is_init;
		uint8_t num_pin;
		uint8_t dallas_len;
		uint8_t exp_ctemp_search;
		OneWire oneWire;
		DallasTemperature sensors;
		Dallas all_dallas[4];
	
		DallasWire();
	
		const uint8_t get_max_all_dallas();
		
		const bool init(const uint8_t num, const uint8_t exp_ctemp_search);
		const bool push_dallas(const uint8_t num_dallas);
	
		const bool resetwiredallas_and_getlen();
		const uint8_t get_real_dallas_len();
	
		const uint8_t search_and_next();
	};

	extern DallasWire all_dallas_wire[4];
	const void init();
}
