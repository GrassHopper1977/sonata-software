// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.hh>
#include <thread.h>
//#include <platform-gpio.hh>
#include <platform-pinmux.hh>
#include <platform-pwm.hh>


/// Expose debugging features unconditionally for this compartment.
using Debug = ConditionalDebug<true, "Main compartment">;

/// Thread entry point.
void __cheri_compartment("main_comp") main_entry()
{
	// Print welcome, along with the compartment's name to the default UART.
	Debug::log("Sonata PWM tests");

	// Setting up the pinmux.
	auto pmx = SonataPinmux();
	// PWM0
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g12, 2)) {
		Debug::log("Success! Set RPi GPIO12 to PWM0");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO12 to PWM0");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::ah_tmpio3, 2)) {
		Debug::log("Success! Set Arduino GPIO3 to PWM0");
	} else {
		Debug::log("ERROR! Failed to set Arduino GPIO3 to PWM0");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::mb10, 1)) {
		Debug::log("Success! Set mikroBUS pin 10 to PWM0");
	} else {
		Debug::log("ERROR! Failed to set mikroBUS pin 10 to PWM0");
	}
	// PWM1
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g13, 2)) {
		Debug::log("Success! Set RPi GPIO13 to PWM1");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO13 to PWM1");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::ah_tmpio5, 2)) {
		Debug::log("Success! Set Arduino GPIO5 to PWM1");
	} else {
		Debug::log("ERROR! Failed to set Arduino GPIO5 to PWM1");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::pmod0_2, 3)) {
		Debug::log("Success! Set PMOD0 pin 2 to PWM1");
	} else {
		Debug::log("ERROR! Failed to set PMOD0 pin 2 to PWM1");
	}
	// PWM2
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g18, 3)) {
		Debug::log("Success! Set RPi GPIO18 to PWM2");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO18 to PWM2");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::ah_tmpio6, 2)) {
		Debug::log("Success! Set Arduino GPIO6 to PWM2");
	} else {
		Debug::log("ERROR! Failed to set Arduino GPIO6 to PWM2");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::pmod0_8, 3)) {
		Debug::log("Success! Set PMOD0 pin 8 to PWM2");
	} else {
		Debug::log("ERROR! Failed to set PMOD0 pin 8 to PWM2");
	}
	// PWM3
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g19, 2)) {
		Debug::log("Success! Set RPi GPIO19 to PWM3");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO19 to PWM3");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::ah_tmpio9, 2)) {
		Debug::log("Success! Set Arduino GPIO5 to PWM9");
	} else {
		Debug::log("ERROR! Failed to set Arduino GPIO5 to PWM9");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::pmod1_2, 3)) {
		Debug::log("Success! Set PMOD1 pin 2 to PWM3");
	} else {
		Debug::log("ERROR! Failed to set PMOD1 pin 2 to PWM3");
	}
	// PWM4
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g20, 3)) {
		Debug::log("Success! Set RPi GPIO20 to PWM4");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO20 to PWM4");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::ah_tmpio10, 3)) {
		Debug::log("Success! Set Arduino GPIO10 to PWM4");
	} else {
		Debug::log("ERROR! Failed to set Arduino GPIO10 to PWM4");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::pmod1_8, 2)) {
		Debug::log("Success! Set PMOD1 pin 8 to PWM4");
	} else {
		Debug::log("ERROR! Failed to set PMOD1 pin 8 to PWM4");
	}
	// PWM5
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g21, 3)) {
		Debug::log("Success! Set RPi GPIO21 to PWM5");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO21 to PWM5");
	}
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::ah_tmpio11, 3)) {
		Debug::log("Success! Set Arduino GPIO11 to PWM5");
	} else {
		Debug::log("ERROR! Failed to set Arduino GPIO11 to PWM5");
	}

	// Now configure the PWM with duty cycles.
	uint8_t dutyCycle0 = 33;
	uint8_t dutyCycle1 = 66;
	uint8_t dutyCycle2 = 99;
	uint8_t dutyCycle3 = 132;
	uint8_t dutyCycle4 = 165;
	uint8_t dutyCycle5 = 198;
	uint32_t pwmOutput0 = 0;
	uint32_t pwmOutput1 = 1;
	uint32_t pwmOutput2 = 2;
	uint32_t pwmOutput3 = 3;
	uint32_t pwmOutput4 = 4;
	uint32_t pwmOutput5 = 5;
	uint32_t pwmPeriod = 200;
	Debug::log("PWM{} duty cycle {}/0xc8", pwmOutput0, dutyCycle0);
	MMIO_CAPABILITY(SonataPwm, pwm)->output_set(pwmOutput0, pwmPeriod, dutyCycle0);
	Debug::log("PWM{} duty cycle {}/0xc8", pwmOutput1, dutyCycle1);
	MMIO_CAPABILITY(SonataPwm, pwm)->output_set(pwmOutput1, pwmPeriod, dutyCycle1);
	Debug::log("PWM{} duty cycle {}/0xc8", pwmOutput2, dutyCycle2);
	MMIO_CAPABILITY(SonataPwm, pwm)->output_set(pwmOutput2, pwmPeriod, dutyCycle2);
	Debug::log("PWM{} duty cycle {}/0xc8", pwmOutput3, dutyCycle3);
	MMIO_CAPABILITY(SonataPwm, pwm)->output_set(pwmOutput3, pwmPeriod, dutyCycle3);
	Debug::log("PWM{} duty cycle {}/0xc8", pwmOutput4, dutyCycle4);
	MMIO_CAPABILITY(SonataPwm, pwm)->output_set(pwmOutput4, pwmPeriod, dutyCycle4);
	Debug::log("PWM{} duty cycle {}/0xc8", pwmOutput5, dutyCycle5);
	MMIO_CAPABILITY(SonataPwm, pwm)->output_set(pwmOutput5, pwmPeriod, dutyCycle5);

	uint32_t mins = 0;
	while(true) {
		Debug::log("Mins: {}", mins);
		thread_millisecond_wait(60000);
		mins += 1;
	}

}
