#pragma once

#define I2C_ADRESS 8

#include <Wire.h>

namespace i2c {
	extern TwoWire wire_current;
	extern uint8_t high_write_len;

	const void init();
	void receiveEvent(int howMany);
	void requestEvent();
	
	enum TRecv: uint8_t {
		SysRestart = 10,
		SysPinsRestart = 11,
		
		LED_BUILTIN_ON = 20,
		LED_BUILTIN_OFF = 21,
	
		Read_ADC_Temperature = 30,
		AnalogRead = 31,
		
		INTERRUPT_ON = 40,
		INTERRUPT_OFF = 41,
	
		PinModeOutput = 50,
		PinModeInput = 51,
		PinModeInputPullup = 52,
		
		PinTimerSetFreq = 55,
	
		DigitalWriteHigh = 60,
		DigitalWriteLow = 61,
	
		AnalogWrite = 70,
		AnalogWrite16Bit = 71,
	
		AutoSleepAdcOn = 80,
		AutoSleepAdcOff = 81,
	
		ButtonInterruptOff = 90,
		ButtonRegRising = 91,
		ButtonRegChange = 92,
		ButtonRegFalling = 93,
		ClearButtons = 94,
		ReadButtons = 95,
		GetCountButtons = 96,
	
		GetSizeMem = 100,
		FlushMem = 101,
		SetMem = 102,
		GetMem = 103,
		SetMem16BitAdress = 104,
		GetMem16BitAdress = 105,
	
		I2CSetSpeed = 110,
	
		DefCPU_PINSLEEP_Ignore = 120,
		DefCPU_PINSLEEP_Idle = 121,
		DefCPU_PINSLEEP_ADC = 122,
		DefCPU_PINSLEEP_PowerDown = 122,
	
		CPU_SLEEP_ON = 125,
		CPU_SLEEP_OFF = 126,
	
		EGetSizeMem = 130,
		EFlushMem = 131,
		ESetMem = 132,
		EGetMem = 133,
	
		GetMillis = 140,
	
		DallasWireInit = 150,
		DallasWireAllLength = 151,
		DallasWireInitLength = 152,
		DallasInit = 153,
		DallasAllInitLength = 155,
		DallasWireSearch = 156,
		DallasGet = 157,
	
		WatchDogInit = 158,
		WatchDogDis = 159,
		WatchDogUpdate = 160,
		WatchDogTrig = 161,
		WatchDogIsOldTrig = 162,
	};
}
