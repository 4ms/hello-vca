#pragma once
#include "devboard_mp133_v0.3.hh"
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

// Define leds, buttons, switches, digital jacks, etc here:
constexpr inline PinDef LevelLED = DevBoard::A1;

constexpr inline PinDef GateInJack = DevBoard::B2;

///// ADC pins:
using mdrivlib::AdcChannelConf;
enum Pots1 : uint32_t { GainCV, GainKnob, /* add more adc names here */ };
enum Pots2 : uint32_t { /* add more adc names here */ };

constexpr auto AdcSampTime = mdrivlib::AdcSamplingTime::_2Cycles;

constexpr auto Adc1Confs = std::to_array({
	AdcChannelConf{DevBoard::B58, DevBoard::AdcChan::B58, GainKnob, AdcSampTime}, //B58 ADC is only for ADC1
	AdcChannelConf{DevBoard::B30, DevBoard::AdcChan::B30, GainCV, AdcSampTime},	  //B30 ADC is only for ADC1
	/* add more ADC channels that have an ADC1 pin here */
});

constexpr auto Adc2Confs = std::to_array({
	/* add more ADC channels that have an ADC2 pin here */
});

constexpr int32_t MinPotValue = 72;
constexpr float MaxPotValue = 4095.f - (float)MinPotValue - 4.f;

//////////////////////////////////////////
///// ADC
struct Adc1Conf : mdrivlib::DefaultAdcPeriphConf {
	static constexpr mdrivlib::AdcResolution resolution = mdrivlib::Bits12;
	static constexpr auto adc_periph_num = mdrivlib::AdcPeriphNum::_1;
	static constexpr auto oversample = true;
	static constexpr auto oversampling_ratio = ADC_OVERSAMPLING_RATIO_256;
	static constexpr auto oversampling_right_bitshift = mdrivlib::AdcOversampleRightBitShift::Shift8Right;
	static constexpr auto clock_div = mdrivlib::PLL_Div2;
	//PLL is about 24MHz: PLL_Div2 with 640cycle sample time = 18.75kHz ISR freq

	static constexpr bool enable_end_of_sequence_isr = true;
	static constexpr bool enable_end_of_conversion_isr = false;
	static constexpr bool continuous_adc = true;

	struct DmaConf : mdrivlib::DefaultAdcPeriphConf::DmaConf {
		static constexpr auto DMAx = 3;
		static constexpr auto StreamNum = 0;
		static constexpr auto RequestNum = DMA_REQUEST_ADC1;
		static constexpr auto dma_priority = High;
	};
};

struct Adc2Conf : mdrivlib::DefaultAdcPeriphConf {
	static constexpr mdrivlib::AdcResolution resolution = mdrivlib::Bits12;
	static constexpr auto adc_periph_num = mdrivlib::AdcPeriphNum::_2;
	static constexpr auto oversample = true;
	static constexpr auto oversampling_ratio = ADC_OVERSAMPLING_RATIO_256;
	static constexpr auto oversampling_right_bitshift = mdrivlib::AdcOversampleRightBitShift::Shift8Right;
	static constexpr auto clock_div = mdrivlib::PLL_Div2;

	static constexpr bool enable_end_of_sequence_isr = true;
	static constexpr bool enable_end_of_conversion_isr = false;
	static constexpr bool continuous_adc = true;

	struct DmaConf : mdrivlib::DefaultAdcPeriphConf::DmaConf {
		static constexpr auto DMAx = 3;
		static constexpr auto StreamNum = 1;
		static constexpr auto RequestNum = DMA_REQUEST_ADC2;
		static constexpr auto dma_priority = High;
	};
};

//////////////////////////////////////////

} // namespace Board
