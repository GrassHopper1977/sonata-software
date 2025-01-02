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
	// NOTE: You must explicitly set the pinmux the RPi header's GPIO for each GPIO that you wish to use.
	auto pmx = SonataPinmux();
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g0, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO0 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g1, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO1 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g2_sda, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO2 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g3_scl, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO3 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g4, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO4 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g5, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO5 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g6, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO6 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g7, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO7 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g8, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO8 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g9, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO9 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g10, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO10 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g11, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO11 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g12, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO12 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g13, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO13 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_txd0, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO14 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_rxd0, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO15 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g16, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO16 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g17, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO17 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g18, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO18 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g19, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO19 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g20, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO20 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g21, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO21 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g22, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO22 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g23, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO23 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g24, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO24 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g25, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO25 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g26, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO26 set to GPIO");
	}
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g27, 1)) {
		Debug::log("ERROR! Failed to set RPi GPIO27 set to GPIO");
	}
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

		// The left and right on the joystick allow us to move through the GPIOs that we are writing to.
		js = gpio_data()->read_joystick();
		if((js & ~js_old & SonataJoystick::Left) == SonataJoystick::Left) {
			out_num++;
			if(out_num >= RPI_GPIO_MAX) {
				out_num = 0;
			}
			display_gpio_details(out_num);
		}
		if((js & ~js_old & SonataJoystick::Right) == SonataJoystick::Right) {
			out_num--;
			if(out_num >= RPI_GPIO_MAX) {
				out_num = RPI_GPIO_MAX - 1;
			}
			display_gpio_details(out_num);
		}
		js_old = js;
	}

}
