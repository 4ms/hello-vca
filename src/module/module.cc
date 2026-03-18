#include "module.hh"
#include "conf/analog_pins.hh"
#include "math.hh"
#include "util/zip.hh"
#include <utility>

namespace HelloVCA
{

//inputs are swapped on carrier board
enum { RightIn = 0, LeftIn = 1 };
enum { LeftOut = 0, RightOut = 1 };

Module::Module() {
	// ...
}

void Module::process(Board::StreamConf::Audio::CombinedAudioBlock &audio_block, ParamBlock &param_block) {
	// Update the edge detector with the latest raw value:
	gate_jack.update(param_block.metaparams.gate_in);

	// Trigger on the jack causes expo_mode to toggle:
	if (gate_jack.went_high()) {
		expo_mode = !expo_mode;

		// Raise the flag that we should save settings
		settings_changed = true;
	}

	param_block.leds.mode_led = expo_mode;

	// Iterate the audio and params.
	for (auto [in, out, params] : zip(audio_block.in_codec, audio_block.out_codec, param_block.params)) {
		// The audio data comes in as a signed 24-bit value
		// We convert to [0..1] float with scale_input_chan()
		// The carrier board inverts the inputs, so we invert again here
		float left_input = -in.scale_input_chan(LeftIn);
		float right_input = -in.scale_input_chan(RightIn);

		float offset = params.analog_ins[GainCV];

		// Knob is wired backwards on the carrier board, oops!
		float gain = (1.f - params.analog_ins[GainKnob]) + offset;

		if (expo_mode) {
			gain = Expf(gain);
		}

		float left_output = left_input * gain;
		float right_output = right_input * gain;

		// The audio data needs to go out as a signed 24-bit value
		// We convert from [0..1] float with set_scaled_output()
		out.set_scaled_output(LeftOut, left_output);
		out.set_scaled_output(RightOut, right_output);
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
