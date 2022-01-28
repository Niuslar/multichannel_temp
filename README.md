# Multi-channel temperature control

The goal of this project is to develop a temperature control system for a bioreactor based on an STM32L053 MCU. 

Broadly speaking, a bioreactor is a tank with a controlled environment where a biological process takes place. In order to work properly, bioreactors need tight temperature control. To achieve this, the temperature control system will include the following features.

## Features

* UART for logging data and sending commands
* 10 ADC Channels for temperature reading
* 10 Control Channels for heaters/coolers using timers
* 4  ADC Channels for telemetry
* PID Control loop to regulate temperature
* LEDs to monitor general application status


## Credits
* [Salavat Magazov](https://github.com/Vavat)



