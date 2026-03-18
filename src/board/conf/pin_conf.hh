#pragma once
#include "analog_pins.hh"
#include "debug.hh"
#include "devboard_mp153_v0.2.hh"
#include "drivers/pin.hh"
#include <array>

namespace Board
{

using mdrivlib::AdcChannelConf;
using mdrivlib::GPIO;
using mdrivlib::PinDef;
using mdrivlib::PinNum;

///////////////////////////////////////////////////////////
// Define leds, buttons, switches, digital jacks, etc here:

// Mode "LED" is Debug::Pin2, aka RX pin on header
constexpr inline PinDef ModeLED = {Debug::Pin2::Gpio_v, (PinNum)Debug::Pin2::PinNum_v};

constexpr inline PinDef GateInJack = DevBoard::B2;

constexpr auto AdcSampTime = mdrivlib::AdcSamplingTime::_2Cycles;

constexpr auto Adc1Confs = std::to_array({
	AdcChannelConf{DevBoard::B58, DevBoard::AdcChan::B58, HelloVCA::GainKnob, AdcSampTime}, //B58 ADC is only for ADC1
	AdcChannelConf{DevBoard::B30, DevBoard::AdcChan::B30, HelloVCA::GainCV, AdcSampTime},	//B30 ADC is only for ADC1
	// add more ADC channels that have an ADC1 pin here
});

constexpr std::array<AdcChannelConf, 0> Adc2Confs;
// Replace the above with this if you use Adc2:
// constexpr auto Adc2Confs = std::to_array({
// 	// add more ADC channels that have an ADC2 pin here
// });

// This may vary on your board:
constexpr int32_t MinPotValue = 72;
constexpr float MaxPotValue = 4020.f;

} // namespace Board
