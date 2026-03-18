#pragma once
#include "analog_pins.hh"
#include "debug.hh"
#include "devboard_mp153_v0.2.hh"
#include "drivers/adc_builtin_conf.hh"
#include "drivers/pin.hh"
#include <array>

namespace Board
{

using mdrivlib::GPIO;
using mdrivlib::PinAF;
using mdrivlib::PinDef;
using mdrivlib::PinMode;
using mdrivlib::PinNum;

///////////////////////////////////////////////////////////
// Define leds, buttons, switches, digital jacks, etc here:

// Debug::Pin2, aka RX pin on header
constexpr inline PinDef ModeLED = {Debug::Pin2::Gpio_v, (PinNum)Debug::Pin2::PinNum_v};

constexpr inline PinDef GateInJack = DevBoard::B2;

///// ADC pins:
using mdrivlib::AdcChannelConf;
constexpr auto AdcSampTime = mdrivlib::AdcSamplingTime::_2Cycles;

constexpr auto Adc1Confs = std::to_array({
	AdcChannelConf{DevBoard::B58, DevBoard::AdcChan::B58, HelloVCA::GainKnob, AdcSampTime}, //B58 ADC is only for ADC1
	AdcChannelConf{DevBoard::B30, DevBoard::AdcChan::B30, HelloVCA::GainCV, AdcSampTime},	//B30 ADC is only for ADC1
	// add more ADC channels that have an ADC1 pin here
});

constexpr auto Adc2Confs = std::to_array({

	//dummy placeholder: remove this if using ADC2:
	AdcChannelConf{},

	// add more ADC channels that have an ADC2 pin here

});

constexpr int32_t MinPotValue = 72;
constexpr float MaxPotValue = 4020.f;

///////////////////////////////////////////////////////////

struct Adc1Conf : mdrivlib::DefaultAdcPeriphConf {
	static constexpr mdrivlib::AdcResolution resolution = mdrivlib::Bits12;
	static constexpr auto adc_periph_num = mdrivlib::AdcPeriphNum::_1;
	static constexpr auto oversample = true;
	static constexpr auto oversampling_ratio = 1024;
	static constexpr auto oversampling_right_bitshift = mdrivlib::AdcOversampleRightBitShift::Shift10Right;
	static constexpr auto clock_div = mdrivlib::PLL_Div1;

	static constexpr bool enable_end_of_sequence_isr = true;
	static constexpr bool enable_end_of_conversion_isr = false;
	static constexpr bool continuous_adc = true;

	struct DmaConf : mdrivlib::DefaultAdcPeriphConf::DmaConf {
		static constexpr auto DMAx = 2;
		static constexpr auto StreamNum = 7;
		static constexpr auto RequestNum = DMA_REQUEST_ADC1;
		static constexpr auto dma_priority = Low;
	};
};

struct Adc2Conf : mdrivlib::DefaultAdcPeriphConf {
	static constexpr mdrivlib::AdcResolution resolution = mdrivlib::Bits12;
	static constexpr auto adc_periph_num = mdrivlib::AdcPeriphNum::_2;
	static constexpr auto oversample = true;
	static constexpr auto oversampling_ratio = 1024;
	static constexpr auto oversampling_right_bitshift = mdrivlib::AdcOversampleRightBitShift::Shift10Right;
	static constexpr auto clock_div = mdrivlib::PLL_Div1;

	static constexpr bool enable_end_of_sequence_isr = true;
	static constexpr bool enable_end_of_conversion_isr = false;
	static constexpr bool continuous_adc = true;

	struct DmaConf : mdrivlib::DefaultAdcPeriphConf::DmaConf {
		static constexpr auto DMAx = 2;
		static constexpr auto StreamNum = 6;
		static constexpr auto RequestNum = DMA_REQUEST_ADC2;
		static constexpr auto dma_priority = Low;
	};
};

//////////////////////////////////////////

} // namespace Board
