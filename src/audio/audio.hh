#pragma once
#include "conf/board_codec_conf.hh"
#include "conf/stream_conf.hh"
#include "controls/controls.hh"
#include "module/module.hh"
#include "param_block.hh"

namespace HelloVCA
{

using AudioConf = Board::StreamConf::Audio;

using AudioInFrame = AudioConf::AudioInFrame;
using AudioOutFrame = AudioConf::AudioOutFrame;
using AudioInBlock = AudioConf::AudioInBlock;
using AudioOutBlock = AudioConf::AudioOutBlock;
using AudioInBuffer = AudioConf::AudioInBuffer;
using AudioOutBuffer = AudioConf::AudioOutBuffer;
using CombinedAudioBlock = AudioConf::CombinedAudioBlock;

class AudioStream {
public:
	AudioStream(AudioInBlock &audio_in_block,
				AudioOutBlock &audio_out_block,
				DoubleBufParamBlock &p,
				Controls &controls,
				UserSettings &settings);

	void start();
	uint32_t get_audio_errors();
	bool settings_changed();

private:
	DoubleBufParamBlock &param_blocks;
	CombinedAudioBlock audio_blocks[2];

	AudioInBlock &audio_in_block;
	AudioOutBlock &audio_out_block;

	Controls &controls;

	Board::CodecT &codec_;

	// Module
	Module module;

	// Settings
	UserSettings &settings;
	bool should_write_settings = false;
	unsigned settings_write_ctr = 0;

	// 10 seconds:
	static constexpr unsigned settings_write_time =
		10 * Board::StreamConf::Audio::SampleRate / Board::StreamConf::Audio::MaxBlockSize;

	// Audio sr/bz
	uint32_t sample_rate_;
	uint32_t block_size_;
};
} // namespace HelloVCA
