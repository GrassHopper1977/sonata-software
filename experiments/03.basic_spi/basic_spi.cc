// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.hh>
#include <thread.h>
#include <platform-gpio.hh>
#include <platform-pinmux.hh>
#include <platform-spi.hh>

// If defined then use the GPIO on the RPi Header as CS0, otherwise the SPI module will drive the line for us.
#define SPI_1_CS0_GPIO
// If defined then use the GPIO on the RPi Header as CS0, otherwise the SPI module will drive the line for us.
#define SPI_2_CS0_GPIO

// The SPI clock calculation
// The settings is the length of a half period of the SPI clock, measured in system clock cycles reduced by 1.
// The system clock is 40000000Hz (40MHz - 25ns)
// The Arduino code example uses a 1MHz SPI clock. Which gives us a clock period of 1E-06 or 1uS.
// Therefore, we want to try to set the half period to 500ns. So our setting, s, will be:
// s = (500 / 25ns) - 1 = 20 - 1 = 19
#define SPI_CLOCK_SPEED_SETTING	(19)

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

#define CLEAR_BIT(REG, BIT) (REG = REG & (~(1U << (BIT))))
#define SET_BIT(REG, BIT)   (REG = REG | (1U << (BIT)))


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
	Debug::log("Manually drive RPi GPIO8 for SPI1_CE0");
	// SPI1 - CE0 (GPIO set to Output and default to high)
	uint32_t spi1_ce0_num = 8;
	rpi_gpio()->set_output_enable(spi1_ce0_num, true);
	rpi_gpio()->set_output(spi1_ce0_num, true);
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g8, 2)) {
		Debug::log("ERROR! Failed to set RPi GPIO8 set to GPIO");
	}
#else
	Debug::log("Letting SPI module drive RPi GPIO8 for SPI1_CE0");
	// SPI1 - CE0 (using SPI module to drive the CE0 pin)
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g8, 1)) {
		Debug::log("Success! Set RPi GPIO8 to SPI1_CE0");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO8 to SPI1_CE0");
	}
#endif
	// Set up the SPI1 module
	spi_mod1()->init(
		false,	// Clock Polarity = 0
	    false,	// Clock Phasse = 0
	    true,	// MSB first = true
	    SPI_CLOCK_SPEED_SETTING);	// The settings is the length of a half period of the SPI clock, measured in system clock cycles reduced by 1.
#ifndef SPI_1_CS0_GPIO
	SET_BIT(spi_mod1()->cs, 0);
#endif
	Debug::log("SPI1: Configured.");

	// SPI2 - COPI
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g20, 1)) {
		Debug::log("Success! Set RPi GPIO20 to SPI2_COPI");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO20 to SPI2_COPI");
	}
	// SPI2 - SCLK
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g21, 1)) {
		Debug::log("Success! Set RPi GPIO21 to SPI2_SCLK");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO21 to SPI2_SCLK");
	}
	// SPI2 - CIPO
	if(true == pmx.block_input_select(SonataPinmux::BlockInput::spi_2_cipo, 1)) {
		Debug::log("Success! Set SPI1_CIPO to RPi GPIO19");
	} else {
		Debug::log("ERROR! Failed to set SPI1_CIPO to RPi GPIO19");
	}
#ifdef SPI_2_CS0_GPIO
	Debug::log("Manually drive RPi GPI18 for SPI2_CE0");
	// SPI2 - CE0 (GPIO set to Output and default to high)
	uint32_t spi2_ce0_num = 18;
	rpi_gpio()->set_output_enable(spi2_ce0_num, true);
	rpi_gpio()->set_output(spi2_ce0_num, true);
	if(false == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g18, 2)) {
		Debug::log("ERROR! Failed to set RPi GPI18 set to GPIO");
	}
#else
	Debug::log("Letting SPI module drive RPi GPI18 for SPI2_CE0");
	// SPI2 - CE0 (using SPI module to drive the CE0 pin)
	if(true == pmx.output_pin_select(SonataPinmux::OutputPin::rph_g18, 1)) {
		Debug::log("Success! Set RPi GPI18 to SPI2_CE0");
	} else {
		Debug::log("ERROR! Failed to set RPi GPIO8 to SPI2_CE0");
	}
#endif
	// Set up the SPI2   module
	// The system clock is 40000000Hz (40MHz - 25ns)
	// The tickrate is 100Hz (10ms)
	spi_mod2()->init(
		false,	// Clock Polarity = 0
	    false,	// Clock Phasse = 0
	    true,	// MSB first = true
	    SPI_CLOCK_SPEED_SETTING);	// The settings is the length of a half period of the SPI clock, measured in system clock cycles reduced by 1.
#ifndef SPI_2_CS0_GPIO
	SET_BIT(spi_mod1()->cs, 0);
#endif
	Debug::log("SPI2: Configured.");

	// SPI1: RESET the MCP2518FD
	uint8_t data_reset = MCP_CMD_RESET;
	Debug::log("SPI1: MCP 2518FD RESET: {}", data_reset);
#ifdef SPI_1_CS0_GPIO
	rpi_gpio()->set_output(spi1_ce0_num, false);
#else
	CLEAR_BIT(spi_mod1()->cs, 0);
#endif
	spi_mod1()->blocking_write(&data_reset, 1);		// Only sending CLK, no COPI, CIPO or CE
	spi_mod1()->wait_idle();	// Wait for the Tx to finish.
#ifdef SPI_1_CS0_GPIO
	rpi_gpio()->set_output(spi1_ce0_num, true);
#else
	SET_BIT(spi_mod1()->cs, 0);
#endif
	Debug::log("SPI1: Reset sent.");

	// SPI2: RESET the MCP2518FD
	Debug::log("SPI2: MCP 2518FD RESET: {}", data_reset);
#ifdef SPI_2_CS0_GPIO
	rpi_gpio()->set_output(spi2_ce0_num, false);
#else
	CLEAR_BIT(spi_mod2()->cs, 0);
#endif
	spi_mod2()->blocking_write(&data_reset, 1);		// Only sending CLK, no COPI, CIPO or CE
	spi_mod2()->wait_idle();	// Wait for the Tx to finish.
#ifdef SPI_2_CS0_GPIO
	rpi_gpio()->set_output(spi2_ce0_num, true);
#else
	SET_BIT(spi_mod2()->cs, 0);
#endif
	Debug::log("SPI2: Reset sent.");

	// The first two bytes send the read command and the address. READ from 0x0E03
	uint8_t data_tx[2] = {MCP_CMD_READ | 0x0E, 0x03};
	uint8_t data_rx[4] = {0x0, 0x0, 0x0, 0x0};	// Empty bytes which will be written to.
	int i;
	while(true) {
		thread_millisecond_wait(1000);

		// SPI 1
		for(i = 0; i < 4; i++) {
			data_rx[i] = 0;
		}
		Debug::log("SPI1: Reading addr 0x0E03...");
		Debug::log("SPI1: Tx 0: {}", data_tx[0]);
		Debug::log("SPI1: Tx 1: {}", data_tx[1]);
#ifdef SPI_1_CS0_GPIO
		rpi_gpio()->set_output(spi1_ce0_num, false);
#endif
		spi_mod1()->blocking_write(data_tx, 2);	// Write the command bytes
		spi_mod1()->wait_idle();	// Wait for the Tx to finish.
		spi_mod1()->blocking_read(data_rx, 4);	// Read the data bytes.
#ifdef SPI_1_CS0_GPIO
		spi_mod1()->wait_idle();	// Wait for the Rx to finish.
		rpi_gpio()->set_output(spi1_ce0_num, true);
#endif
		Debug::log("SPI1: Data sent.");
		Debug::log("SPI1: Rx 0: {}", data_rx[0]);
		Debug::log("SPI1: Rx 1: {}", data_rx[1]);
		Debug::log("SPI1: Rx 2: {}", data_rx[2]);
		Debug::log("SPI1: Rx 3: {}", data_rx[3]);
		
		thread_millisecond_wait(1000);

		// SPI 2
		for(i = 0; i < 4; i++) {
			data_rx[i] = 0;
		}
		Debug::log("SPI2: Reading addr 0x0E03...");
		Debug::log("SPI2: Tx 0: {}", data_tx[0]);
		Debug::log("SPI2: Tx 1: {}", data_tx[1]);
#ifdef SPI_2_CS0_GPIO
		rpi_gpio()->set_output(spi2_ce0_num, false);
#endif
		spi_mod2()->blocking_write(data_tx, 2);	// Write the command bytes
		spi_mod2()->wait_idle();	// Wait for the Tx to finish.
		spi_mod2()->blocking_read(data_rx, 4);	// Read the data bytes.
#ifdef SPI_2_CS0_GPIO
		spi_mod2()->wait_idle();	// Wait for the Rx to finish.
		rpi_gpio()->set_output(spi2_ce0_num, true);
#endif
		Debug::log("SPI2: Data sent.");
		Debug::log("SPI2: Rx 0: {}", data_rx[0]);
		Debug::log("SPI2: Rx 1: {}", data_rx[1]);
		Debug::log("SPI2: Rx 2: {}", data_rx[2]);
		Debug::log("SPI2: Rx 3: {}", data_rx[3]);
	}
}
