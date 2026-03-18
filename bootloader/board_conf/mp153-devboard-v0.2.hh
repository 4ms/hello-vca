#pragma once
#include "drivers/clocks.hh"
#include "drivers/i2c_conf.hh"
#include "drivers/leds.hh"

namespace Board
{

using RedLED = Led<GPIO::D, PinNum::_6, LedActive::Low>;   // not connected
using GreenLED = Led<GPIO::D, PinNum::_4, LedActive::Low>; // not connected
using BlueLED = Led<GPIO::D, PinNum::_5, LedActive::Low>;  // not connected

// Boot Select pin: press Button 1 to select DFU USB
constexpr bool UseBootSelect = true;
constexpr PinConf BootSelectPin{GPIO::D, PinNum::_8, PinAF::AFNone, PinPolarity::Inverted};

// Freeze mode: jumper on debug header (left-most column)
constexpr bool UseFreezePin = true;
constexpr PinConf FreezePin{GPIO::E, PinNum::_5}; //DEBUG3 of debug header

constexpr uint32_t ConsoleUART = UART4_BASE;
constexpr PinConf UartRX{GPIO::D, PinNum::_0, PinAF::AF_8};
constexpr PinConf UartTX{GPIO::D, PinNum::_1, PinAF::AF_8};

namespace SDMMC
{
constexpr int DataWidth = 4;
}

namespace NORFlash
{
constexpr bool HasNORFlash = true;

constexpr PinConf clk{GPIO::F, PinNum::_10, PinAF::AF_9};
constexpr PinConf cs{GPIO::B, PinNum::_6, PinAF::AF_10};
constexpr PinConf d0{GPIO::F, PinNum::_8, PinAF::AF_10};
constexpr PinConf d1{GPIO::F, PinNum::_9, PinAF::AF_10};
constexpr PinConf d2{GPIO::F, PinNum::_7, PinAF::AF_9};
constexpr PinConf d3{GPIO::F, PinNum::_6, PinAF::AF_9};

constexpr uint32_t SizeBytes = 16 * 1024 * 1024;

enum AddressBits { AddrBits24 = 24, AddrBits32 = 32 };
constexpr inline AddressBits address_bits = AddrBits24;

enum ChipID { IS25L, S25FLxxxL, W25Q128JV };
constexpr inline ChipID chip_id = W25Q128JV;

enum IOMode { SingleSPI, DualSPI, QuadSPI };
constexpr inline IOMode io_mode = QuadSPI;

enum Bank { Bank1, Bank2 };
constexpr inline Bank bank = Bank1;
} // namespace NORFlash

namespace PMIC
{
constexpr bool HasSTPMIC = false;
constexpr I2C_Config I2C_config{};
} // namespace PMIC

constexpr uint32_t HSE_Clock_Hz = 24000000;
constexpr uint32_t MPU_MHz = 800;
constexpr auto ClockType = SystemClocks::HSEClockSource::Resonator;

} // namespace Board
