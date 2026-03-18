#include "controls.hh"
#include "console/pr_dbg.hh"
#include "debug.hh"

namespace HelloVCA
{

using namespace mdrivlib;

void Controls::update_debouncers() {
	// Read and debounce pins here (buttons, switches, etc)
	// This is called every sample
	gate_in_jack.update();
}

void Controls::update_params() {
	// Populate the params and metaparams fields
	// This is called every sample

	// Interpolate knob readings across the param block, since we capture them at a slower rate than audio process
	_adcs1_bank.add_new_readings([this](unsigned i) { return get_adc1_reading(i); });
	_adcs1_bank.get_interp_values([this](unsigned i, float v) { cur_params->analog_ins[i] = std::clamp(v, 0.f, 1.f); });

	// If you end up enabling ADC2, then uncomment this:
	// _adcs2_bank.add_new_readings([this](unsigned i) { return get_adc2_reading(i); });
	// _adcs2_bank.get_interp_values(
	// 	[this](auto i, float v) { cur_params->analog_ins[i + NumAdcs1] = std::clamp(v, 0.f, 1.f); });

	// This part runs at the start of every audio block
	if (_first_param) {
		_first_param = false;

		// Set all block-rate metaparam:
		cur_metaparams->gate_in = gate_in_jack.is_high();

		// Better would be to use PWM or an LED driver so we can show a float value:
		level_led.set_to(cur_leds->level_led > 0.5f ? 1 : 0);

		// This LED is on/off:
		mode_led.set_to(cur_leds->mode_led);
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
	_adcs1_bank.set_num_updates(sample_rate / Adc1ReadFrequency + 1);
	_adcs2_bank.set_num_updates(sample_rate / Adc2ReadFrequency + 2);
}

Controls::Controls(DoubleBufParamBlock &param_blocks_ref)
	: param_blocks(param_blocks_ref)
	, cur_params(param_blocks[0].params.begin())
	, cur_metaparams(&param_blocks_ref[0].metaparams)
	, cur_leds(&param_blocks_ref[0].leds) {

	InterruptManager::register_and_start_isr(ADC1_IRQn, 0, 2, [&] {
		if (adc1_periph.end_of_seq_flag()) {
			_adcs1_bank.mark_new_data_ready();
		}
	});

	if constexpr (NumAdcs2 > 0) {
		InterruptManager::register_and_start_isr(ADC2_IRQn, 0, 2, [&] {
			if (adc2_periph.end_of_seq_flag()) {
				_adcs2_bank.mark_new_data_ready();
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

static float adc_to_float(int32_t raw) {
	float val = raw - Board::MinPotValue;
	return std::clamp(val / (Board::MaxPotValue - Board::MinPotValue), 0.f, 1.f);
}

float Controls::get_adc1_reading(uint32_t pot_id) {
	return (pot_id < StaticBuffers::adc1_vals.size()) ? adc_to_float(StaticBuffers::adc1_vals[pot_id]) : 0;
}

float Controls::get_adc2_reading(uint32_t pot_id) {
	return (pot_id < StaticBuffers::adc2_vals.size()) ? adc_to_float(StaticBuffers::adc2_vals[pot_id]) : 0;
}

} // namespace HelloVCA
