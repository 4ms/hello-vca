#pragma once
#include "conf/stream_conf.hh"
#include "params/param_block.hh"
#include "settings.hh"
#include "util/edge_detector.hh"

namespace HelloVCA
{

struct Module {
	Module();

	void process(Board::StreamConf::Audio::CombinedAudioBlock &audio_block, ParamBlock &param_block);

	void save_settings(UserSettings *settings);
	void load_settings(UserSettings const &settings);
	bool did_settings_change();

private:
	EdgeStateDetector gate_jack{};
	bool expo_mode{};

	bool settings_changed = false;
};

} // namespace HelloVCA
