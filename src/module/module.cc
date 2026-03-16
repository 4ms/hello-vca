#include "module.hh"
#include "conf/analog_pins.hh"
#include <utility>

// #include "math.hh"

namespace HelloVCA
{

Module::Module() {
	// ...
}

void Module::process(Board::StreamConf::Audio::CombinedAudioBlock &audio_block, ParamBlock &param_block) {

	gate_jack.update(param_block.metaparams.gate_in);

	if (gate_jack.went_low()) {
		// handle gate jack going low...
		// for this simple example we just turn off expo mode
		expo_mode = false;

	} else if (gate_jack.went_high()) {
		expo_mode = true;
	}

	for (auto idx = 0u; auto const &in : audio_block.in_codec) {
		auto &out = audio_block.out_codec[idx];
		auto &params = param_block.params[idx];

		float left_input = in.scale_input_chan(0);
		float right_input = in.scale_input_chan(1);

		float offset = params.analog_ins[GainCV];
		float gain = params.analog_ins[GainKnob] + offset;

		if (expo_mode) {
			gain *= 0.5f;
			gain = gain * gain * gain * gain;
			gain *= 2.f;
		}

		float left_output = left_input * gain;
		float right_output = right_input * gain;

		out.set_scaled_output(0, left_output);
		out.set_scaled_output(1, right_output);

		idx++;
	}
}

void Module::save_settings(UserSettings *settings) {
	settings->expo_mode = expo_mode;
}

void Module::load_settings(UserSettings const &settings) {
	expo_mode = settings.expo_mode;
}

bool Module::did_settings_change() {
	return std::exchange(settings_changed, false);
}

} // namespace HelloVCA
