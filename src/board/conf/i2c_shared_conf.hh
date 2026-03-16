#pragma once
#include "drivers/i2c_config_struct.hh"

namespace Board
{

using mdrivlib::GPIO;
using mdrivlib::PinAF;
using mdrivlib::PinNum;

// Used for LED driver (LP5009), and codec
const mdrivlib::I2CConfig a7m4_shared_i2c_conf = {
	.I2Cx = I2C1,
	.SCL = {GPIO::D, PinNum::_12, PinAF::AltFunc5},
	.SDA = {GPIO::E, PinNum::_8, PinAF::AltFunc5},
	.timing =
		{
			.PRESC = 0x40,
			.SCLDEL_SDADEL = 0xFF,
			.SCLH = 0x90,
			.SCLL = 0x90,
		},
	.priority1 = 2,
	.priority2 = 1,
};

} // namespace Board
