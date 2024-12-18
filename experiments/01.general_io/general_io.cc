// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.hh>
// #include <fail-simulator-on-error.h>
#include <platform-rgbctrl.hh>
#include <thread.h>
#include <platform-gpio.hh>
#include "gpio_helper.hh"


/// Expose debugging features unconditionally for this compartment.
using Debug = ConditionalDebug<true, "Main compartment">;

/// Access to the built-in GPIO
auto gpio_data()
{
	return MMIO_CAPABILITY(SonataGpioBoard, gpio_board);
}

/// Access to the Raspberry Pi header's GPIO
auto rpi_gpio()
{
	return MMIO_CAPABILITY(SonataGpioRaspberryPiHat, gpio_rpi);
}

void set_rgb_led_colour(auto leds, uint8_t colours[]) {
	leds->rgb(SonataRgbLed::Led0, colours[0], colours[1], colours[2]);
	leds->update();
}

/// Thread entry point.
void __cheri_compartment("main_comp") main_entry()
{
	// Print welcome, along with the compartment's name to the default UART.
	Debug::log("Sonata general IO tests");

	uint8_t colours[] = {255, 0, 0};
	auto  leds    = MMIO_CAPABILITY(SonataRgbLedController, rgbled);
	leds->rgb(SonataRgbLed::Led0, colours[0], colours[1], colours[2]);
	leds->update();
	set_rgb_led_colour(leds, colours);

	// Set RPi header GPIO27 to an output and we will flash it on and off.
	bool rp_out_state = true;
	uint32_t rpi_out_num = 27;
	rpi_gpio()->set_output_enable(rpi_out_num, true);
	rpi_gpio()->set_output(rpi_out_num, rp_out_state);

	uint8_t phase = 0;
	uint8_t currentLed = 0;
	while(true) {
		thread_millisecond_wait(50);
		switch(phase) {
			default:
				phase = 0;
				colours[0] = 255;
				colours[1] = 0;
				colours[2] = 0;
				Debug::log("Phase incorrect {} resetting to 0.", phase);
				break;
			case 0:
				colours[0]--;
				colours[1]++;
				if(colours[0] == 0) {
					phase++;
				}
				break;
			case 1:
				colours[1]--;
				colours[2]++;
				if(colours[1] == 0) {
					phase++;
				}
				break;
			case 2:
				colours[2]--;
				colours[0]++;
				if(colours[2] == 0) {
					phase = 0;
				}
				break;
		}
		set_rgb_led_colour(leds, colours);

		gpio_data()->led_off(currentLed);
		currentLed++;
		if(currentLed > gpio_data()->LEDCount) {
			currentLed = 0;
			if(rp_out_state == true) {
				rp_out_state = false;
			} else {
				rp_out_state = true;
			}
			rpi_gpio()->set_output(rpi_out_num, rp_out_state);
			Debug::log("RPi GPIO{} {}", rpi_out_num, rp_out_state);
		}
		gpio_data()->led_on(currentLed);

		auto js = gpio_data()->read_joystick();
		if((js & SonataJoystick::Left) == SonataJoystick::Left) {
			Debug::log("Joystick: Left");
		}
		if((js & SonataJoystick::Right) == SonataJoystick::Right) {
			Debug::log("Joystick: Right");
		}
		if((js & SonataJoystick::Up) == SonataJoystick::Up) {
			Debug::log("Joystick: Up");
		}
		if((js & SonataJoystick::Down) == SonataJoystick::Down) {
			Debug::log("Joystick: Down");
		}
		if((js & SonataJoystick::Pressed) == SonataJoystick::Pressed) {
			Debug::log("Joystick: Pressed");
		}
	}

}
