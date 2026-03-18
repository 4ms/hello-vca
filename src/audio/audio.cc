#include "audio/audio.hh"
#include "console/pr_dbg.hh"
#include "debug.hh"
#include "param_block.hh"
#include <cstring>

namespace HelloVCA
{

using namespace mdrivlib;

AudioStream::AudioStream(AudioInBlock &audio_in_block,
						 AudioOutBlock &audio_out_block,
						 DoubleBufParamBlock &pblk,
						 Controls &controls,
						 UserSettings &settings)
	: param_blocks{pblk}
	, audio_in_block{audio_in_block}
	, audio_out_block{audio_out_block}
	, controls{controls}
	, codec_{Board::Hardware::codec}
	, settings{settings}
	, sample_rate_{Board::StreamConf::Audio::SampleRate}
	, block_size_{Board::StreamConf::Audio::MaxBlockSize} {

	if (codec_.init() == Board::CodecT::CODEC_NO_ERR)
		pr_info("Codec initialized\n");
	else
		pr_info("ERROR: No codec detected\n");

	audio_blocks[0].in_codec = {audio_in_block.codec[0].data(), block_size_};
	audio_blocks[1].in_codec = {std::next(audio_in_block.codec[0].begin(), block_size_), block_size_};

	audio_blocks[0].out_codec = {audio_out_block.codec[0].data(), block_size_};
	audio_blocks[1].out_codec = {std::next(audio_out_block.codec[0].begin(), block_size_), block_size_};

	codec_.set_tx_buffer(audio_blocks[0].out_codec, block_size_);
	codec_.set_rx_buffer(audio_blocks[0].in_codec, block_size_);

	auto audio_callback = [this]<unsigned block>() {
		// Debug::Pin0::high();

		this->controls.start_param_block<block>();

		module.process(audio_blocks[1 - block], param_blocks[1 - block]);

		// Start/restart countdown if settings changed
		if (module.did_settings_change()) {
			settings_write_ctr = settings_write_time;
		}
		if (settings_write_ctr > 0) {
			if (--settings_write_ctr == 0) {
				should_write_settings = true;
			}
		}

		// Debug::Pin0::low();
	};

	codec_.set_callbacks([audio_callback]() { audio_callback.operator()<0>(); },
						 [audio_callback]() { audio_callback.operator()<1>(); });
}

void AudioStream::start() {
	module.load_settings(settings);
	codec_.start();
}

bool AudioStream::settings_changed() {
	// There's a chance this function gets interrupted and the state is
	// partially the old and partially the new values.
	// However, in this module all fields of the state are atomically written, so
	// it would still be a valid state.
	if (should_write_settings) {
		should_write_settings = false;
		module.save_settings(&settings);
		return true;
	}
	return false;
}

uint32_t AudioStream::get_audio_errors() {
	return codec_.get_sai_errors();
}

} // namespace HelloVCA
