#pragma once

#include <Arduino.h>

namespace buttons {
	struct Button {
		uint8_t num;
		uint8_t cupdate;
		
		Button();
	
		const uint8_t get_cupdate_and_flush();
		const void reg(const uint8_t mmode = 'C');
		const void destruct(void);
	};
	
	const void init();

	void button_interrupt(const uint8_t pinn);
	void inline button_interrupt0();
	void inline button_interrupt1();
	void inline button_interrupt2();
	void inline button_interrupt3();
	void inline button_interrupt4();
	void inline button_interrupt5();
	void inline button_interrupt6();
	void inline button_interrupt7();
	void inline button_interrupt8();
	void inline button_interrupt9();
	void inline button_interrupt10();
	void inline button_interrupt11();
	void inline button_interrupt12();
	void inline button_interrupt13();
	void inline button_interrupt14();
	void inline button_interrupt15();
	void inline button_interrupt16();
	void inline button_interrupt17();
	void inline button_interrupt18();
	void inline button_interrupt19();

	extern bool is_update_buttons;
	extern uint8_t all_rbuttons;
	extern buttons::Button all_buttons[14];
}
