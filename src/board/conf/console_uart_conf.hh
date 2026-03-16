#pragma once
#include "drivers/uart_conf.hh"

constexpr inline UartConf LogUartConfig{
	.base_addr = UART7_BASE,
	.TXPin = {mdrivlib::GPIO::H, mdrivlib::PinNum::_2, mdrivlib::PinAF::AltFunc8},
	.RXPin = {mdrivlib::GPIO::B, mdrivlib::PinNum::_3, mdrivlib::PinAF::AltFunc13},
	.mode = UartConf::Mode::TXRX,
	.baud = 115200,
	.wordlen = 8,
	.parity = UartConf::Parity::None,
	.stopbits = UartConf::StopBits::_1,
};
