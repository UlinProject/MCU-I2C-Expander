
#include "buttons.h"
#include "i2c/interrupt.h"
#include "sys/uyield.h"

namespace buttons {
	bool is_update_buttons;
	uint8_t all_rbuttons;
	buttons::Button all_buttons[14];
	
	const void init() {
		all_rbuttons = 0;
		is_update_buttons = false;
	
		uint16_t i = 0;
		const uint16_t max = sizeof(all_buttons) / sizeof(Button);
		while (i < max) {
			all_buttons[i] = Button();
			sys::uyield::next();
			i += 1;
		}
	}
	
	void button_interrupt(const uint8_t pinn) {
		uint8_t i = 0;
		const uint16_t max = sizeof(all_buttons) / sizeof(Button);
		while (i < max) {
			auto abutton = &all_buttons[i];
	
			if (abutton->num == pinn) {
				if (abutton->cupdate < 255) {
					abutton->cupdate += 1;
				}
				is_update_buttons = true;
				i2c::interrupt::en();
				
				break;
			}
			
			i += 1;
		}
	}
	
	Button::Button() {
		this->num = 255; // <-- EN: DIS
		this->cupdate = 0;
	}
	
	const void Button::reg(const uint8_t mmode = (uint8_t)(char)'C') {
		int hmode;
		if (mmode == 'R') {
			hmode = RISING;
		}else
		if (mmode == 'C') {
			hmode = CHANGE;
		}else
		if (mmode == 'F') {
			hmode = FALLING;
		}else {
			hmode = CHANGE;
		}

		auto function = button_interrupt0;
		if (this->num == 0) { function = button_interrupt0; } else
		if (this->num == 1) { function = button_interrupt1; } else
		if (this->num == 2) { function = button_interrupt2; } else
		if (this->num == 3) { function = button_interrupt3; } else
		if (this->num == 4) { function = button_interrupt4; } else
		if (this->num == 5) { function = button_interrupt5; } else
		if (this->num == 6) { function = button_interrupt6; } else
		if (this->num == 7) { function = button_interrupt7; } else
		if (this->num == 8) { function = button_interrupt8; } else
		if (this->num == 9) { function = button_interrupt9; } else
		if (this->num == 10) { function = button_interrupt10; } else
		if (this->num == 11) { function = button_interrupt11; } else
		if (this->num == 12) { function = button_interrupt12; } else
		if (this->num == 13) { function = button_interrupt13; } else
		if (this->num == 14) { function = button_interrupt14; } else
		if (this->num == 15) { function = button_interrupt15; } else
		if (this->num == 16) { function = button_interrupt16; } else
		if (this->num == 17) { function = button_interrupt17; } else
		if (this->num == 18) { function = button_interrupt18; } else
		if (this->num == 19) { function = button_interrupt19; }

		attachInterrupt(digitalPinToInterrupt(this->num), function, hmode);
	}
	
	const void Button::destruct(void) {
		detachInterrupt(digitalPinToInterrupt(this->num));
			
		this->num = 255;
		this->cupdate = 0;
	}
	
	const uint8_t Button::get_cupdate_and_flush(void) {
		const uint8_t result = this->cupdate;
		this->cupdate = 0;
		
		return result;
	}
	
	void inline button_interrupt0() { button_interrupt(0); }
	void inline button_interrupt1() { button_interrupt(1); }
	void inline button_interrupt2() { button_interrupt(2); }
	void inline button_interrupt3() { button_interrupt(3); }
	void inline button_interrupt4() { button_interrupt(4); }
	void inline button_interrupt5() { button_interrupt(5); }
	void inline button_interrupt6() { button_interrupt(6); }
	void inline button_interrupt7() { button_interrupt(7); }
	void inline button_interrupt8() { button_interrupt(8); }
	void inline button_interrupt9() { button_interrupt(9); }
	void inline button_interrupt10() { button_interrupt(10); }
	void inline button_interrupt11() { button_interrupt(11); }
	void inline button_interrupt12() { button_interrupt(12); }
	void inline button_interrupt13() { button_interrupt(13); }
	void inline button_interrupt14() { button_interrupt(14); }
	void inline button_interrupt15() { button_interrupt(15); }
	void inline button_interrupt16() { button_interrupt(16); }
	void inline button_interrupt17() { button_interrupt(17); }
	void inline button_interrupt18() { button_interrupt(18); }
	void inline button_interrupt19() { button_interrupt(19); }

}
