#pragma once
#include "conf/control_conf.hh"
#include "conf/pin_conf.hh"
#include "drivers/adc_builtin.hh"
#include "drivers/debounced_switch.hh"
#include "drivers/pin.hh"
#include "drivers/pin_change.hh"
#include "memory/static_buffers.hh"
#include "params/metaparams.hh"
#include "params/param_block.hh"
#include "params/params.hh"
#include "util/filter.hh"
#include "util/interp_param.hh"
#include <atomic>

namespace HelloVCA
{

using mdrivlib::DebouncedButton;
using mdrivlib::DebouncedPin;
using mdrivlib::PinMode;
using mdrivlib::PinPolarity;

struct Controls {
private:
	//////////////////////////
	// Switches/Buttons/LEDs/gate jacks
	DebouncedPin<Board::GateInJack, PinPolarity::Normal> gate_in_jack;

	mdrivlib::Pin level_led{Board::LevelLED, PinMode::Output};
	mdrivlib::Pin mode_led{Board::ModeLED, PinMode::Output};

	// May need to tweak values this by hand:
	// Measure the read freq and set:
	static constexpr uint32_t Adc1ReadFrequency = 3000;
	static constexpr uint32_t Adc2ReadFrequency = 3750;

	////////////////////////////////

	// Mostly boiler-plate below here:

public:
	Controls(DoubleBufParamBlock &param_blocks_ref);

	void start();
	void process();

	template<size_t block_num>
	void start_param_block() {
		static_assert(block_num <= 1, "There is only block 0 and block 1");

		// 28us width, every 1.3ms (audio block rate for 64-frame blocks) = 2.15% load
		cur_metaparams = &param_blocks[block_num].metaparams;
		cur_params = param_blocks[block_num].params.begin();
		cur_leds = &param_blocks[block_num].leds;
		_first_param = true;
		_buffer_full = false;
	}

private:
	void update_params();
	void update_debouncers();
	void update_lights();
	float get_adc1_reading(uint32_t adc_id);
	float get_adc2_reading(uint32_t adc_id);
	void set_samplerate(unsigned sample_rate);

	// Analog controls
	static constexpr size_t NumAdcs1 = Board::Adc1Confs.size();
	static constexpr size_t NumAdcs2 = Board::Adc2Confs.size();

	mdrivlib::AdcDmaPeriph<Board::Adc1Conf> adc1_periph{StaticBuffers::adc1_vals, Board::Adc1Confs};
	mdrivlib::AdcDmaPeriph<Board::Adc2Conf> adc2_periph{StaticBuffers::adc2_vals, Board::Adc2Confs};

	std::array<HysteresisFilter<4, 4096>, NumAdcs1> adc1_filt;
	std::array<HysteresisFilter<4, 4096>, NumAdcs2> adc2_filt;

	std::array<InterpParamVariable<float>, NumAdcs1> _adcs1{};
	std::array<InterpParamVariable<float>, NumAdcs2> _adcs2{};

	std::atomic<bool> _new_adc1_data_ready = false;
	std::atomic<bool> _new_adc2_data_ready = false;
	unsigned num_adc1_updates = 0;
	unsigned num_adc2_updates = 0;

	// Task
	mdrivlib::PinChangeInt<Board::FrameRatePinChangeConf> read_controls_task;

	// Params
	DoubleBufParamBlock &param_blocks;
	Params *cur_params;
	MetaParams *cur_metaparams;
	LedStates *cur_leds;

	// Internal state
	bool _buffer_full = false;
	bool _first_param = true;

	uint32_t sample_rate = Board::StreamConf::Audio::SampleRate;
};

} // namespace HelloVCA
