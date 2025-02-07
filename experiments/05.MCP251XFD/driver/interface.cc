#include <cdefs.h>
#include <stdint.h>
#include <thread.h>

#include <platform-gpio.hh>
#include <platform-pinmux.hh>
#include <platform-spi.hh>
#include "ErrorsDef.h"
#include "Conf_MCP251XFD.h"
#include "crc/CRC16_CMS.hh"
#include "MCP251XFD.hh"
#include "interface.hh"


// The SPI clock calculation
// The settings is the length of a half period of the SPI clock, measured in system clock cycles reduced by 1.
// The system clock is 40000000Hz (40MHz - 25ns)
// The code examples we found use a 1MHz SPI clock. Which gives us a clock period of 1E-06 or 1uS.
// Therefore, we want to try to set the half period to 500ns. So our setting, s, will be:
// s = (500 / 25ns) - 1 = 20 - 1 = 19

#define SPI_CLOCK_SPEED_SETTING	(19)
#define CLEAR_BIT(REG, BIT) (REG = REG & (~(1U << (BIT))))
#define SET_BIT(REG, BIT)   (REG = REG | (1U << (BIT)))

using Debug = ConditionalDebug<true, "Interface.cc">;

eERRORRESULT GetSpiConfig(Spi_Config_t* cfg, uint8_t spi_num, SonataPinmux::OutputPin cs0, SonataPinmux::OutputPin cs1, SonataPinmux::OutputPin cs2, SonataPinmux::OutputPin sclk, SonataPinmux::OutputPin copi, uint8_t cipo) {
    if(spi_num == 1) {
        cfg->spi = MMIO_CAPABILITY(SonataSpi, spi1);    // Access to the SPI1 module
    } else if(spi_num == 2) {
        cfg->spi = MMIO_CAPABILITY(SonataSpi, spi2);    // Access to the SPI2 module
    } else {
        return ERR__SPI_PARAMETER_ERROR;
    }
    if(spi_num == 1) {
        // SCLK
        if((sclk == SonataPinmux::OutputPin::rph_g11) || (sclk == SonataPinmux::OutputPin::ah_tmpio13)) {
            cfg->sclk_sel = 1;
        } else if(sclk == SonataPinmux::OutputPin::pmod0_4) {
            cfg->sclk_sel = 2;
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // COPI
        if((copi == SonataPinmux::OutputPin::rph_g10) || (copi == SonataPinmux::OutputPin::ah_tmpio11)) {
            cfg->copi_sel = 1;
        } else if(copi == SonataPinmux::OutputPin::pmod0_2) {
            cfg->copi_sel = 2;
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // CIPO
        if((cipo < 1) || (cipo > 3)) {
            return ERR__SPI_PARAMETER_ERROR;
        }
        cfg->cipo_sel = SonataPinmux::BlockInput::spi_1_cipo;
        // CS0
        if(cs0 == SonataPinmux::OutputPin::rph_g8) {
            cfg->cs0_sel = 1;
        } else if(cs0 == SonataPinmux::OutputPin::pmod0_1) {
            cfg->cs0_sel = 2;
        } else if(cs0 == SonataPinmux::OutputPin::ser0_tx) {
            cfg->cs0_sel = 0; // Not Used
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // CS1
        if(cs1 == SonataPinmux::OutputPin::rph_g7) {
            cfg->cs1_sel = 1;
        } else if(cs1 == SonataPinmux::OutputPin::pmod0_9) {
            cfg->cs1_sel = 2;
        } else if(cs1 == SonataPinmux::OutputPin::ser0_tx) {
            cfg->cs1_sel = 0; // Not Used
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // CS2
        if(cs2 == SonataPinmux::OutputPin::ser0_tx) {
            cfg->cs2_sel = 0; // Not Used
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
    } else if(spi_num == 2) {
        // SCLK
        if((sclk == SonataPinmux::OutputPin::rph_g21) || (sclk == SonataPinmux::OutputPin::mb2)) {
            cfg->sclk_sel = 1;
        } else if(sclk == SonataPinmux::OutputPin::pmod1_4) {
            cfg->sclk_sel = 2;
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // COPI
        if((copi == SonataPinmux::OutputPin::rph_g20) || (copi == SonataPinmux::OutputPin::mb4)) {
            cfg->copi_sel = 1;
        } else if(copi == SonataPinmux::OutputPin::pmod1_2) {
            cfg->copi_sel = 2;
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // CIPO
        if((cipo < 1) || (cipo > 3)) {
            return ERR__SPI_PARAMETER_ERROR;
        }
        cfg->cipo_sel = SonataPinmux::BlockInput::spi_2_cipo;
        // CS0
        if(cs0 == SonataPinmux::OutputPin::rph_g18) {
            cfg->cs0_sel = 1;
        } else if(cs0 == SonataPinmux::OutputPin::pmod1_1) {
            cfg->cs0_sel = 2;
        } else if(cs0 == SonataPinmux::OutputPin::ser0_tx) {
            cfg->cs0_sel = 0; // Not Used
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // CS1
        if(cs1 == SonataPinmux::OutputPin::rph_g17) {
            cfg->cs1_sel = 1;
        } else if(cs1 == SonataPinmux::OutputPin::pmod1_9) {
            cfg->cs1_sel = 2;
        } else if(cs1 == SonataPinmux::OutputPin::ser0_tx) {
            cfg->cs1_sel = 0; // Not Used
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
        // CS2
        if(cs1 == SonataPinmux::OutputPin::rph_g16) {
            cfg->cs1_sel = 1;
        } else if(cs1 == SonataPinmux::OutputPin::pmod1_10) {
            cfg->cs1_sel = 2;
        } else if(cs1 == SonataPinmux::OutputPin::ser0_tx) {
            cfg->cs1_sel = 0; // Not Used
        } else {
            return ERR__SPI_PARAMETER_ERROR;
        }
    } else {
        return ERR__SPI_PARAMETER_ERROR;
    }
    cfg->spi_num = spi_num;
    cfg->cs0 = cs0;
    cfg->cs1 = cs1;
    cfg->sclk = sclk;
    cfg->copi = copi;
    cfg->cipo = cipo;
    return ERR_OK;
}

//=============================================================================
// MCP251XFD_X get millisecond
//=============================================================================
uint32_t GetCurrentms_Sonata(void)
{
    // Sonata
    // Written - 2025-01-10
    // Tested - 
    uint64_t cycles = rdcycle64();  // Hidden in riscvreg.h and included through thread.h
    uint32_t msCount = static_cast<uint32_t>(cycles) * MS_PER_TICK;    // Driver is not bothered by it wrapping (apprently).
    return msCount;
}
//=============================================================================
// MCP251XFD_X compute CRC16-CMS
//=============================================================================
uint16_t ComputeCRC16_Sonata(const uint8_t* data, size_t size)
{
    // Sonata
    // From the MCP2518FD datasheet: 
    // The MCP2518FD device uses the following generator polynomial: CRC-16/USB (0x8005). CRC-16 detects
    // all single and double-bit errors, all errors with an odd number of bits, all burst errors of length 16 or less, and
    // most errors for longer bursts. This allows an excellent detection of SPI communication errors that can happen in the system, and heavily reduces the risk of
    // miscommunication, even under noisy environments.
    // The best way to implement this will be with a prepared crc_table.
    // 
    // Written - 2025-02-03
    // Tested - 
    return ComputeCRC16CMS(data, size);
}
//*******************************************************************************************************************
//=============================================================================
// MCP251XFD SPI driver interface configuration for the Sonata
//=============================================================================
eERRORRESULT MCP251XFD_InterfaceInit_Sonata(void *pIntDev, uint8_t chipSelect, const uint32_t sckFreq)
{
    // Initialises the hardware interface.
    // Written - 2025-01-21
    // Tested - 

    if (pIntDev == NULL)
        return ERR__SPI_PARAMETER_ERROR;
    // Get our data.
    Spi_Config_t* cfg = (Spi_Config_t*)pIntDev;
    // Configure the IO the chosen SPI
    auto pmx = SonataPinmux();
    uint8_t sel = 0;
    // SLCK
    if(false == pmx.output_pin_select(cfg->sclk, cfg->sclk_sel)) {
        return ERR__SPI_PARAMETER_ERROR;
    }
    // COPI
    if(false == pmx.output_pin_select(cfg->sclk, cfg->sclk_sel)) {
        return ERR__SPI_PARAMETER_ERROR;
    }
    // CS0
    if(cfg->cs0_sel > 0) {
        if(false == pmx.output_pin_select(cfg->cs0, cfg->cs0_sel)) {
            return ERR__SPI_PARAMETER_ERROR;
        }
    }
    // CS1
    if(cfg->cs1_sel > 0) {
        if(false == pmx.output_pin_select(cfg->cs1, cfg->cs1_sel)) {
            return ERR__SPI_PARAMETER_ERROR;
        }
    }
    // CS2
    if(cfg->cs2_sel > 0) {
        if(false == pmx.output_pin_select(cfg->cs2, cfg->cs2_sel)) {
            return ERR__SPI_PARAMETER_ERROR;
        }
    }
    // CIPO
    if(false == pmx.block_input_select(cfg->cipo_sel, cfg->cipo)) {
		return ERR__SPI_PARAMETER_ERROR;
	}
        
    //--- Configure an SPI peripheral ---
    cfg->spi->init(
		false,	// Clock Polarity = 0
	    false,	// Clock Phasse = 0
	    true,	// MSB first = true
	    SPI_CLOCK_SPEED_SETTING);	// The settings is the length of a half period of the SPI clock, measured in system clock cycles reduced by 1.

    return ERR_OK;
}

//=============================================================================
// MCP251XFD SPI transfer data for the Sonata
//=============================================================================
eERRORRESULT MCP251XFD_InterfaceTransfer_Sonata(void *pIntDev, uint8_t chipSelect, uint8_t *txData, uint8_t *rxData, size_t size)
{
    // Perform an SPI transfer.
    // Written - 2025-01-21
    // Tested - 

    if (pIntDev == NULL)
        return ERR__SPI_PARAMETER_ERROR;
    if (txData == NULL)
        return ERR__SPI_PARAMETER_ERROR;
    //Spi *SPI_Ext = (Spi *)pIntDev; // MCU specific: #define SPI0 ((Spi*)0x40008000U) // (SPI0) Base Address
    Spi_Config_t* cfg = (Spi_Config_t*)pIntDev;
    uint8_t DataRead;

    // Debug::log("{}() {}: txData = {}", __FUNCTION__, __LINE__, txData);
    // Debug::log("{}() {}: rxData = {}", __FUNCTION__, __LINE__, rxData);
    // Debug::log("{}() {}:   size = {}", __FUNCTION__, __LINE__, size);
    // if(txData != NULL) Debug::log("{}() {}: txData[]: {}, {}", __FUNCTION__, __LINE__, txData[0], txData[1]);
    // if(rxData != NULL) Debug::log("{}() {}: rxData[]: {}, {}", __FUNCTION__, __LINE__, rxData[0], rxData[1]);

    CLEAR_BIT(cfg->spi->cs, chipSelect);    // Chip Select Low
    cfg->spi->blocking_transfer(txData, rxData, size);
    cfg->spi->wait_idle();	// Wait for the Rx to finish (shouldn't be needed).
    SET_BIT(cfg->spi->cs, chipSelect);      // Chip Select High

    //__disable_irq();
    // uint32_t Timeout;
    // while (size > 0)
    // {
    //     //--- Transmit data ---
    //     Timeout = TIMEOUT_SPI_INTERFACE;
    //     while (!(SPI_Ext->SPI_SR & SPI_SR_TDRE)) // SPI Tx ready ?
    //     {
    //         if (!Timeout--) return ERR__SPI_TIMEOUT; // Timeout ? return an error
    //     }
    //     uint32_t value = SPI_TDR_TD(*txData) | SPI_TDR_PCS(spi_get_pcs(chipSelect));
    //     if (size == 1) value |= SPI_TDR_LASTXFER;
    //     //SPI_Ext->SPI_TDR = value;
    //     txData++;
    //     //--- Receive data ---
    //     Timeout = TIMEOUT_SPI_INTERFACE;
    //     while (!(SPI_Ext->SPI_SR & SPI_SR_RDRF)) // SPI Rx ready ?
    //     {
    //         if (!Timeout--) return ERR__SPI_TIMEOUT; // Timeout ? return an error
    //     }
    //     DataRead = (uint8_t)(SPI_Ext->SPI_RDR & 0xFF);
    //     if (rxData != NULL) *rxData = DataRead;
    //     rxData++;
    //     size--;
    // }
    //SPI_Ext->SPI_CR |= SPI_CR_LASTXFER;
    //__enable_irq();
    return ERR_OK;
}