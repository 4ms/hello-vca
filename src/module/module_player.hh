#pragma once
#include "conf/stream_conf.hh"
#include "params/param_block.hh"
#include "settings_json.hh"

struct ModulePlayer {
	ModulePlayer();

	void process(Board::StreamConf::Audio::CombinedAudioBlock &audio_block, MetaModule::ParamBlock &param_block);

	void init(UserSettings &settings);
	void update_settings(UserSettings &settings);

	uint32_t settings_write_ctr = 0;
	float settings_write_time_ms = 10000;
	bool should_write_settings = false;

	constexpr static float sample_rate = Board::StreamConf::Audio::SampleRate;
	constexpr static uint32_t block_size = Board::StreamConf::Audio::MaxBlockSize;
};
