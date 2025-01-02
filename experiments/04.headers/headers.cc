// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.hh>
// #include <fail-simulator-on-error.h>
#include <thread.h>
#include <platform-gpio.hh>
#include <platform-pinmux.hh>
#include <platform-pwm.hh>
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

#define LED_INDICATOR	0
#define RPI_GPIO_MAX	28
// //									0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23	24	25	26	27
// const uint8_t gpio_pin_nums[28] = {27, 28, 	3, 	5,	7,	29,	31,	26,	24,	21,	19,	23,	32,	33,	8,	10,	36,	11,	12,	35,	38,	40,	15,	16,	18,	22,	37,	23};
// void display_gpio_details(uint16_t num) {
// 	if(num >= RPI_GPIO_MAX)
// 		return;
// 	Debug::log("Outputting on RPi GPIO{} (pin {}).", num, gpio_pin_nums[num]);
// }
//									0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23	24	25	26	27
const char* gpio_pin_nums[28] = {
	"0 (pin 27)", 
	"1 (pin 28)", 	
	"2 (pin 3)", 	
	"3 (pin 5)",	
	"4 (pin 7)",	
	"5 (pin 29)",	
	"6 (pin 31)",	
	"7 (pin 26)",	
	"8 (pin 24)",	
	"9 (pin 21)",	
	"10 (pin 19)",	
	"11 (pin 23)",	
	"12 (pin 32)",	
	"13 (pin 33)",	
	"14 (pin 8)",	
	"15 (pin 10)",	
	"16 (pin 36)",	
	"17 (pin 11)",	
	"18 (pin 12)",	
	"19 (pin 35)",	
	"20 (pin 38)",	
	"21 (pin 40)",	
	"22 (pin 15)",	
	"23 (pin 16)",	
	"24 (pin 18)",	
	"25 (pin 22)",	
	"26 (pin 37)",	
	"27 (pin 13)"
	};
void display_gpio_details(uint16_t num) {
	if(num >= RPI_GPIO_MAX)
		return;
	Debug::log("Outputting on RPi GPIO{}.", gpio_pin_nums[num]);
}

/// Thread entry point.
void __cheri_compartment("main_comp") main_entry()
{
	// Print welcome, along with the compartment's name to the default UART.
	Debug::log("Sonata RPi Header GPIO output tests");

	Debug::log("Setting all RPi Header GPIO to outputs & off ...");
	for(int i = 0; i < RPI_GPIO_MAX; i++) {
		rpi_gpio()->set_output_enable(i, true);
		rpi_gpio()->set_output(i, false);
		Debug::log("RPi GPIO{} is output.", gpio_pin_nums[i]);
	}
	Debug::log("Done.");

	Debug::log("Configuring joystick ...");
	SonataJoystick js = gpio_data()->read_joystick();
	SonataJoystick js_old = js;
	Debug::log("Done.");

	Debug::log("Set initial output info ...");
	uint8_t out_num = 0;
	bool output_state = false;
	uint16_t timer = 0;
	Debug::log("Done.");

	display_gpio_details(out_num);
	while(true) {
		thread_millisecond_wait(100);
		timer++;
		if(timer >= 10) {
			timer = 0;
			output_state = !output_state;
			rpi_gpio()->set_output(out_num, output_state);
			Debug::log("RPi GPIO{} = {}", gpio_pin_nums[out_num], output_state);
			if(output_state) {
				gpio_data()->led_on(LED_INDICATOR);
			} else {
				gpio_data()->led_off(LED_INDICATOR);
			}
		}

		js = gpio_data()->read_joystick();
		if((js & ~js_old & SonataJoystick::Left) == SonataJoystick::Left) {
			//Debug::log("Joystick: Left");
			out_num++;
			if(out_num >= RPI_GPIO_MAX) {
				out_num = 0;
			}
			display_gpio_details(out_num);
		}
		if((js & ~js_old & SonataJoystick::Right) == SonataJoystick::Right) {
			//Debug::log("Joystick: Right");
			out_num--;
			if(out_num >= RPI_GPIO_MAX) {
				out_num = RPI_GPIO_MAX - 1;
			}
			display_gpio_details(out_num);
		}
		// if((js & ~js_old & SonataJoystick::Up) == SonataJoystick::Up) {
		// 	Debug::log("Joystick: Up");
		// }
		// if((js & ~js_old & SonataJoystick::Down) == SonataJoystick::Down) {
		// 	Debug::log("Joystick: Down");
		// }
		// if((js & ~js_old & SonataJoystick::Pressed) == SonataJoystick::Pressed) {
		// 	Debug::log("Joystick: Pressed");
		// }
		js_old = js;
	}

}
