// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT

#include <compartment.h>
#include <debug.hh>
#include <thread.h>
#include <platform-gpio.hh>
#include <platform-pinmux.hh>
#include <platform-spi.hh>
#include "interface.h"
#include "driver/MCP251XFD.h"


/// Expose debugging features unconditionally for this compartment.
using Debug = ConditionalDebug<true, "Main compartment">;


/// Thread entry point.
void __cheri_compartment("main_comp") main_entry()
{
	// Print welcome, along with the compartment's name to the default UART.
	Debug::log("Sonata MCP251XFD CAN Example");

	// Configure the CAN device
	uint32_t can0_sclk_result = 0;
	MCP251XFD_BitTimeStats can0_bittimestats;
	struct MCP251XFD can0 = {
		UserDriverData = NULL,	//!< Optional, can be used to store driver data or NULL
		DriverConfig = MCP251XFD_DRIVER_NORMAL_USE,    //!< Driver configuration, by default it is MCP251XFD_DRIVER_NORMAL_USE. Configuration can be OR'ed
		GPIOsOutLevel = 0,              //!< GPIOs pins output state (0 = set to '0' ; 1 = set to '1'). Used to speed up output change
  		SPI_ChipSelect = 0,				//!< This is the Chip Select index that will be set at the call of a transfer
		InterfaceDevice = spi_mod1(),	//!< This is the pointer that will be in the first parameter of all interface call functions
		SPIClockSpeed = 500000,				//!< SPI nominal clock speed (max is SYSCLK div by 2)
		fnSPI_Init = MCP251XFD_InterfaceInit_Sonata,	//!< This function will be called at driver initialization to configure the interface driver
		fnSPI_Transfer = MCP251XFD_InterfaceTransfer_Sonata,	//!< This function will be called at driver read/write data from/to the interface driver SPI
		fnGetCurrentms = GetCurrentms_Sonata,	//!< This function will be called when the driver need to get current millisecond
		fnComputeCRC16 = ComputeCRC16_Sonata	//!< This function will be called when a CRC16-CMS computation is needed (ie. in CRC mode or Safe Write). In normal mode, this can point to NULL
	};

	MCP251XFD_Config can_conf0 =
	{
		//--- Controller clocks ---
		XtalFreq = 40000000UL; // 40Mhz                 //!< Component CLKIN Xtal/Resonator frequency (min 4MHz, max 40MHz). Set it to 0 if oscillator is used
		OscFreq = 0,                               		//!< Component CLKIN oscillator frequency (min 2MHz, max 40MHz). Set it to 0 if Xtal/Resonator is used
		SysclkConfig = MCP251XFD_SYSCLK_IS_CLKIN,       //!< Factor of frequency for the SYSCLK. SYSCLK = CLKIN x SysclkConfig where CLKIN is XtalFreq or OscFreq
		ClkoPinConfig = MCP251XFD_CLKO_DivBy10,         //!< Configure the CLKO pin (SCLK div by 1, 2, 4, 10 or Start Of Frame)
		SYSCLK_Result = &can0_sclk_result,              //!< This is the SYSCLK of the component after configuration (can be NULL if the internal SYSCLK of the component do not have to be known)

		//--- CAN configuration ---
		NominalBitrate = 500000, 	// 500 Kbit         //!< Speed of the Frame description and arbitration
		DataBitrate = NO_CANFD,                         //!< Speed of all the data bytes of the frame (if CAN2.0 only mode, set to value NO_CANFD)
		BitTimeStats = &can0_bittimestats,           	//!< Point to a Bit Time stat structure (set to NULL if no statistics are necessary)
		Bandwidth = MCP251XFD_NO_DELAY,                 //!< Transmit Bandwidth Sharing, this is the delay between two consecutive transmissions (in arbitration bit times)
		//!< Set of CAN control flags to configure the CAN controller. Configuration can be OR'ed
		ControlFlags = MCP251XFD_CAN_LISTEN_ONLY_MODE_ON_ERROR,

		//--- GPIOs and Interrupts pins ---
		GPIO0PinMode = MCP251XFD_PIN_AS_GPIO0_OUT,      //!< Startup INT0/GPIO0/XSTBY pins mode (INT0 => Interrupt for TX)
		GPIO1PinMode = MCP251XFD_PIN_AS_GPIO1_OUT,      //!< Startup INT1/GPIO1 pins mode (INT1 => Interrupt for RX)
		INTsOutMode = MCP251XFD_PINS_PUSHPULL_OUT,      //!< Define the output type of all interrupt pins (INT, INT0 and INT1)
		TXCANOutMode = MCP251XFD_PINS_PUSHPULL_OUT,     //!< Define the output type of the TXCAN pin

		//--- Interrupts ---
		//!< Set of system interrupt flags to enable. Configuration can be OR'ed
		SysInterruptFlags = MCP251XFD_INT_NO_EVENT
	};

	eERRORRESULT result = Init_MCP251XFD(&can0, &can_conf0);

	while(true) {
		thread_millisecond_wait(1000);
	}
}
