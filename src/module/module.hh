#pragma once
#include "conf/stream_conf.hh"
#include "params/param_block.hh"
#include "settings.hh"
#include "util/edge_detector.hh"

namespace HelloVCA
{

// This is your module. Implement each function in module.cc
struct Module {
	Module();

	// The main DSP processing goes here:
	// See the example for how audio_block and param_block work.
	void process(Board::StreamConf::Audio::CombinedAudioBlock &audio_block, ParamBlock &param_block);

	// This gets called before writing to flash. Populate settings with your internal state.
	void save_settings(UserSettings *settings);

	// This will get called on init, so copy from settings to your internal state.
	void load_settings(UserSettings const &settings);

	// When you want settings to be saved to flash, return true.
	// The platform will call save_settings() shortly after you return true
	bool did_settings_change();

private:
	// Any internal state should be private variables:

	EdgeStateDetector gate_jack{};
	bool expo_mode{};

	bool settings_changed = false;
};

} // namespace HelloVCA
