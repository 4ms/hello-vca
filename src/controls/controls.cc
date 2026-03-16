#include "controls.hh"
#include "conf/i2c_shared_conf.hh"
#include "console/pr_dbg.hh"
#include "debug.hh"
#include "util/zip.hh"

namespace HelloVCA
{

using namespace mdrivlib;

void Controls::update_debouncers() {
	// Read and debounce pins here (buttons, switches, etc)
	// This is called every sample
}

void Controls::update_params() {
	// Populate the params and metaparams fields
	// This is called every sample (but the `if (_first_param)` block runs once per block)

	// Interpolate knob readings across the param block, since we capture them at a slower rate than audio process
	if (_new_adc1_data_ready) {
		for (unsigned i = 0; i < NumAdcs1; i++) {
			_adcs1[i].set_new_value(adc1_filt[i].add_val(get_adc1_reading(i)));
			num_adc1_updates = 0;
		}
		_new_adc1_data_ready = false;
	}

	num_adc1_updates++;
	if (num_adc1_updates >= _adcs1[0].get_num_updates()) {
		for (unsigned i = 0; i < NumAdcs1; i++) {
			auto val = _adcs1[i].target_val;
			cur_params->analog_ins[i] = std::clamp(val, 0.f, 1.f);

			_adcs1[i].cur_val = _adcs1[i].target_val;
		}
	} else {
		for (unsigned i = 0; i < NumAdcs1; i++) {
			auto val = _adcs1[i].next();
			cur_params->analog_ins[i] = std::clamp(val, 0.f, 1.f);
		}
	}

	if constexpr (NumAdcs2 > 0) {
		if (_new_adc2_data_ready) {
			for (unsigned i = 0; i < NumAdcs2; i++) {
				_adcs2[i].set_new_value(adc2_filt[i].add_val(get_adc2_reading(i)));
				num_adc2_updates = 0;
			}
			_new_adc2_data_ready = false;
		}
		num_adc2_updates++;
		if (num_adc2_updates >= _adcs2[0].get_num_updates()) {
			for (unsigned i = 0; i < NumAdcs2; i++) {
				auto val = _adcs2[i].target_val;
				cur_params->analog_ins[i + NumAdcs1] = std::clamp(val, 0.f, 1.f);

				_adcs1[i].cur_val = _adcs2[i].target_val;
			}
		} else {
			for (unsigned i = 0; i < NumAdcs2; i++) {
				auto val = _adcs2[i].next();
				cur_params->analog_ins[i + NumAdcs1] = std::clamp(val, 0.f, 1.f);
			}
		}
	}

	if (_first_param) {
		_first_param = false;

		// Set all block-rate metaparam:
		cur_metaparams->gate_in = gate_in_jack.is_high();

		// Set LEDs here

		// Better would be to use PWM or an LED driver so we can show a float value:
		level_led.set(cur_metaparams->level_led > 0.5f ? 1 : 0);

		// This LED is on/off:
		mode_led.set(cur_metaparams->mode_led);
	}

	cur_params++;
	if (cur_params == param_blocks[0].params.end() || cur_params == param_blocks[1].params.end())
		_buffer_full = true;
}

void Controls::start() {
	adc1_periph.start();

	if constexpr (NumAdcs2 > 0)
		adc2_periph.start();

	read_controls_task.start();
}

void Controls::process() {
	// Here is a good place to deal with I2C busses, e.g. MUXes and GPIO Expanders
}

void Controls::set_samplerate(unsigned sample_rate) {
	this->sample_rate = sample_rate;
	for (auto &adc : _adcs1) {
		adc.set_num_updates(sample_rate / Adc1ReadFrequency + 1);
	}
	for (auto &adc : _adcs2) {
		adc.set_num_updates(sample_rate / Adc2ReadFrequency + 2); //FIXME: why +2?
	}
}

Controls::Controls(DoubleBufParamBlock &param_blocks_ref)
	: param_blocks(param_blocks_ref)
	, cur_params(param_blocks[0].params.begin())
	, cur_metaparams(&param_blocks_ref[0].metaparams)
	, cur_leds(&param_blocks_ref[0].leds) {

	InterruptManager::register_and_start_isr(ADC1_IRQn, 0, 2, [&] {
		if (adc1_periph.end_of_seq_flag()) {
			_new_adc1_data_ready = true;
		}
	});

	if constexpr (NumAdcs2 > 0) {
		InterruptManager::register_and_start_isr(ADC2_IRQn, 0, 2, [&] {
			if (adc2_periph.end_of_seq_flag()) {
				_new_adc2_data_ready = true;
			}
		});
	}

	set_samplerate(sample_rate);

	read_controls_task.init([this]() {
		if (_buffer_full)
			return;
		update_debouncers();
		update_params();
	});
}

float Controls::get_adc1_reading(uint32_t pot_id) {
	if (pot_id < StaticBuffers::adc1_vals.size()) {
		int32_t raw = StaticBuffers::adc1_vals[pot_id];
		float val = raw - Board::MinPotValue;
		return std::clamp(val / (Board::MaxPotValue - Board::MinPotValue), 0.f, 1.f);
	}
	return 0;
}

float Controls::get_adc2_reading(uint32_t pot_id) {
	if (pot_id < StaticBuffers::adc2_vals.size()) {
		int32_t raw = StaticBuffers::adc2_vals[pot_id];

		float val = raw - Board::MinPotValue;
		return std::clamp(val / (Board::MaxPotValue - Board::MinPotValue), 0.f, 1.f);
	}
	return 0;
}

} // namespace HelloVCA
