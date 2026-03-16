#pragma once
#include "conf/board_codec_conf.hh"
#include "conf/stream_conf.hh"
#include "controls/controls.hh"
#include "module/module_player.hh"
#include "param_block.hh"
#include "settings_json.hh"

namespace MetaModule
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
	void start_playing();
	uint32_t get_audio_errors();

	bool settings_changed();

private:
	DoubleBufParamBlock &param_blocks;
	CombinedAudioBlock audio_blocks[2];

	AudioInBlock &audio_in_block;
	AudioOutBlock &audio_out_block;
	Controls &controls;
	UserSettings &settings;

	ModulePlayer module;

	// Hardware interface
	Board::CodecT &codec_;
};
} // namespace MetaModule
