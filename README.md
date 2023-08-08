# MCU-I2C-Expander

The main idea of this project is to use small microcontrollers like `Atmega328`, `Attiny88`, `...` to extend functionality (e.g. `watchdog timer`, `volatile memory`, `RAM`, `I/O pins`, `PWM`, ...) of main master microcontrollers connected via I2C bus.

<b>ATTENTION:</b> The `platformio` project is initially configured for `atmega328` microcontrollers with clocking from the built-in quartz resonator with the ability to work in `3.3v` mode.

<b>ATTENTION:</b> All project settings are scattered across header files. Default i2c address is `8`. Please note that the project uses a separate interrupt pin. Note that the byte order must match on both devices.

# Features

Commands supported by the controller, writing to commands is indicated by a leading character `~` + command code + required bytes for the command + interrupt (if specified in the input).

| | Feature | Code (u8) | RecvBytes (count) | RecvInt (exp(+)/ignore(-)) | SendBytes(count) | SendInt(exp(+)/ignore(-)) |
|-| -------- | --- |            - | - | - | - |
| |SysRestart| 10|              0 | - | 0 |!+ |
| |SysPinsRestart| 11|          0 | - | 0 | + |
| |LED_BUILTIN_ON| 20|          0 | - | 0 | + |
| |LED_BUILTIN_OFF| 21|         0 | - | 0 | + |
| |Read_ADC_Temperature| 30|    
| |AnalogRead| 31|              1 | - | 2 | + |
| |INTERRUPT_ON| 40|            0 | - | 0 | + |
| |INTERRUPT_OFF| 41|           0 | - | 0 | - |
| |PinModeOutput| 50|           1 | - | 0 | + |
| |PinModeInput| 51|            1 | - | 0 | + |
| |PinModeInputPullup| 52|      1 | - | 0 | + |
| |PinTimerSetFreq| 55|         2 | - | 0 | + |
| |DigitalWriteHigh| 60|        1 | - | 0 | + |
| |DigitalWriteLow| 61|         1 | - | 0 | + |
| |AnalogWrite| 70|             2 | - | 0 | + |
| |AnalogWrite16Bit| 71|        3 | - | 0 | + |
| |AutoSleepAdcOn| 80|          0 | - | 0 | + |
| |AutoSleepAdcOff| 81|         0 | - | 0 | + |
| |ButtonInterruptOff| 90|      1 | - | 0 | + |
| |ButtonRegRising| 91|         1 | - | 0 | + |
| |ButtonRegChange| 92|         1 | - | 0 | + |
| |ButtonRegFalling| 93|        1 | - | 0 | + |
| |ClearButtons| 94|            0 | - | 0 | + |
| |ReadButtons| 95|             0 | - | 0 or array (num+count) | + |
| |GetCountButtons| 96|         1 | - | 0 | + |
| |GetSizeMem| 100|             0 | - | 2 | + |
| |FlushMem| 101|                0 | - | 0 | + |
| |SetMem| 102|                  2 | - | 0 | + |
| |GetMem| 103|                  1 | - | 1 | + |
| |SetMem16BitAdress| 104|       3 | - | 0 | + |
| |GetMem16BitAdress| 105|       2 | - | 1 | + |
| |I2CSetSpeed| 110|             4 | - | 0 | + |
| |DefCPU_PINSLEEP_Ignore| 120|  0 | - | 0 | + |
| |DefCPU_PINSLEEP_Idle| 121|    0 | - | 0 | + |
| |DefCPU_PINSLEEP_ADC| 122|     0 | - | 0 | + |
| |DefCPU_PINSLEEP_PowerDown| 122| 0 | - | 0 | + |
| |CPU_SLEEP_ON| 125|            0 | - | 0 | + |
| |CPU_SLEEP_OFF| 126|           0 | - | 0 | + |
| |EGetSizeMem| 130|            0 | - | 2 | + |
| |EFlushMem| 131|              0 | - | 0 | + |
| |ESetMem| 132|                3 | - | 0 | + |
| |EGetMem| 133|                2 | - | 1 | + |
| |GetMillis| 140|              4 | - | 0 | + |
| |DallasWireIni| 150|          2 | - | 1 | + |
| |DallasWireAllLength| 151|    0 | - | 1 | + |
| |DallasWireInitLength| 152|   1 | - | 0 | + |
| |DallasInit| 153|             2 | - | 1 | + |
| |DallasAllInitLength| 155|    0 | - | 1 | + |
| |DallasWireSearch| 156|       1 | - | 1 | + |
| |DallasGet| 157|              2 | - | 4 | + |
| |WatchDogInit| 158|           4 | - | 0 | + |
| |WatchDogDis| 159|            0 | - | 0 | + |
| |WatchDogUpdate| 160|         0 | - | 0 | + |
| |WatchDogTrig| 161|           0 | - | 0 | + |
| |WatchDogIsOldTrig| 162|      0 | - | 1 | + |

# License

Copyright 2022-2023 #UlinProject Denis Kotlyarov (Денис Котляров)

Licensed under the Apache License, Version 2.0

