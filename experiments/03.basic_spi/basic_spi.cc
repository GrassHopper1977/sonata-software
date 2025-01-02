// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.hh>
#include <thread.h>
#include <platform-gpio.hh>
#include <platform-pinmux.hh>
#include <platform-spi.hh>

// If defined then use the RPi Header as CS0
#define SPI_1_CS0_GPIO


/// Expose debugging features unconditionally for this compartment.
using Debug = ConditionalDebug<true, "Main compartment">;

/// Access to the Raspberry Pi header's GPIO
auto rpi_gpio()
{
	return MMIO_CAPABILITY(SonataGpioRaspberryPiHat, gpio_rpi);
}

/// Access to the SPI1 module
auto spi_mod1()
{
	return MMIO_CAPABILITY(SonataSpi, spi1);
}

/// Access to the SPI2 module
auto spi_mod2()
{
	return MMIO_CAPABILITY(SonataSpi, spi2);
}

// Do an SPI transfer
// command:
//   Binary      Hex   Command
//   0b00000000  0x00  RESET
//   0b00110000  0x30  READ
//   0b00100000  0x20  WRITE
//   0b10110000  0xB0  READ_CRC
//   0b10100000  0xA0  WRITE_CRC
//   0b11000000  0xC0  WRITE_SAFE
// address: 12-bit (max value is 0x07FF)
// length: How many bytes to transfer
// data_write: Pointer to data buffer to write. If NULL will write 0.
// data_read:  Pointer to data buffer to read into. If NULL will ignore the inputs.
#define MCP_CMD_MASK	0xF0
#define MCP_CMD_RESET	0x00
#define MCP_CMD_READ	0x30
#define MCP_CMD_WRITE	0x20
#define MCP_ADDR_MAX	0x0FFF


/// Thread entry point.
void __cheri_compartment("main_comp") main_entry()
{
	// Print welcome, along with the compartment's name to the default UART.
	Debug::log("Sonata SPI tests");

	// Setting up the pinmux.
	auto pmx = SonataPinmux();
	// SPI1 - COPI
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g10, 1)) {
		Debug::log("Success! Set RPi GPIO10 to SPI1_COPI");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO10 to SPI1_COPI");
	}
	// SPI1 - SCLK
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g11, 1)) {
		Debug::log("Success! Set RPi GPIO11 to SPI1_SCLK");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO11 to SPI1_SCLK");
	}
	// SPI1 - CIPO
	if(true == pmx.block_input_select(SonataPinmux::BlockInput::spi_1_cipo, 1)) {
		Debug::log("Success! Set SPI1_CIPO to RPi GPIO09");
	} else {
		Debug::log("ERROR! Failed to set SPI1_CIPO to RPi GPIO09");
	}
#ifdef SPI_1_CS0_GPIO
	// SPI1 - CE0 (GPIO set to Output and default to high)
	uint32_t spi1_ce0_num = 8;
	rpi_gpio()->set_output_enable(spi1_ce0_num, true);
	rpi_gpio()->set_output(spi1_ce0_num, true);
	Debug::log("Using RPi GPIO8 for SPI1_CE0");
#else
	// SPI1 - CE0 (using pinmux)
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g8, 1)) {
		Debug::log("Success! Set RPi GPIO8 to SPI1_CE0");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO8 to SPI1_CE0");
	}
#endif
	// Set up the SPI1 module
	// The system clock is 40000000Hz (40MHz - 25ns)
	// The tickrate is 100Hz (10ms)
	spi_mod1()->init(
		false,	// Clock Polarity = 0
	    false,	// Clock Phasse = 0
	    false,	// MSB first = false
	    4);		// The length of a half period of the SPI clock, measured in system clock cycles reduced by 1. Therefore (4 * 25) - 1 = 75ms


	// // SPI2 - COPI
	// if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g20, 1)) {
	// 	Debug::log("Success! Set RPi GPIO20 to SPI2_COPI");
	// } else {
	// 	Debug::log("ERROR! Failed to set RPi GPIO20 to SPI2_COPI");
	// }
	// // SPI2 - SCLK
	// if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g21, 1)) {
	// 	Debug::log("Success! Set RPi GPIO21 to SPI2_SCLK");
	// } else {
	// 	Debug::log("ERROR! Failed to set RPi GPIO21 to SPI2_SCLK");
	// }
	// // SPI2 - CIPO
	// if(true == pmx.block_input_select(SonataPinmux::BlockInput::spi_2_cipo, 1)) {
	// 	Debug::log("Success! Set SPI2_CIPO to RPi GPIO19");
	// } else {
	// 	Debug::log("ERROR! Failed to set SPI2_CIPO to RPi GPIO19");
	// }
	// // SPI2 - CE0 (using pinmux)
	// if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g18, 1)) {
	// 	Debug::log("Success! Set RPi GPIO18 to SPI2_CE0");
	// } else {
	// 	Debug::log("ERROR! Failed to set RPi GPIO18 to SPI2_CE0");
	// }
	// // // SPI2 - CE0 (GPIO set to Output and default to high)
	// // bool spi2_ce0_state = true;
	// // uint32_t spi2_ce0_num = 18;
	// // rpi_gpio()->set_output_enable(spi2_ce0_num, true);
	// // rpi_gpio()->set_output(spi2_ce0_num, spi2_ce0_state);
	// // Set up the SPI1 module
	// spi_mod2()->init(
	// 	false,	// Clock Polarity = 0
	//     false,	// Clock Phasse = 0
	//     false,	// MSB first = false
	//     4);		// The length of a half period of the SPI clock, measured in system clock cycles reduced by 1. Therefore (4 * 25) - 1 = 75ms



	// RESET the MCP2518FD
	uint8_t data_reset = MCP_CMD_RESET;
	Debug::log("Sending MCP 2518FD RESET Output: {}", data_reset);
#ifdef SPI_1_CS0_GPIO
	rpi_gpio()->set_output(spi1_ce0_num, false);	// This isn't working!
	Debug::log("CS0 false");
#endif
	spi_mod1()->blocking_write(&data_reset, 1);		// Only sending CLK, no COPI, CIPO or CE
#ifdef SPI_1_CS0_GPIO
	rpi_gpio()->set_output(spi1_ce0_num, true);
	Debug::log("CS0 true");
#endif
	Debug::log("Reset sent.");

	// The frist two bytes send the read command and the address, teh remaining 4 bytes do nothing but must be 0.
	uint8_t data_out_read_sfr[6] = {MCP_CMD_READ | 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00};
	//                             |  READ from |    0XE03  | 4 empty bytes
	uint8_t data_in[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};	// Empty bytes which will be written to.
	uint32_t mins = 0;
	while(true) {
		Debug::log("Mins: {}", mins);

		//thread_millisecond_wait(1000);
		Debug::log("Writing 6 bytes to SPI.");
		for(uint16_t i = 0; i < 6; i++) {
			Debug::log("{} = {}", i, data_out_read_sfr[i]);
		}
		//thread_millisecond_wait(1000);
#ifdef SPI_1_CS0_GPIO
		rpi_gpio()->set_output(spi1_ce0_num, false);
		Debug::log("CS0 false");
#endif
		spi_mod1()->blocking_transfer(data_out_read_sfr, data_in, 6);
#ifdef SPI_1_CS0_GPIO
		rpi_gpio()->set_output(spi1_ce0_num, true);
		Debug::log("CS0 true");
#endif
		//thread_millisecond_wait(1000);
		Debug::log("Written 6 bytes to SPI.");
		for(uint16_t i = 0; i < 6; i++) {
			Debug::log("{} = {}", i, data_in[i]);
		}
		
		thread_millisecond_wait(300000);
		mins += 5;
	}

}
