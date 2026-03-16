#include "audio/audio.hh"
#include "console/pr_dbg.hh"
#include "debug.hh"
#include "param_block.hh"
#include <cstring>

namespace MetaModule
{
using namespace mdrivlib;

AudioStream::AudioStream(AudioInBlock &audio_in_block,
						 AudioOutBlock &audio_out_block,
						 DoubleBufParamBlock &pblk,
						 Controls &controls,
						 json_t &settings)

	: param_blocks{pblk}
	, audio_in_block{audio_in_block}
	, audio_out_block{audio_out_block}
	, controls{controls}
	, settings{settings}
	, codec_{Board::Hardware::codec} {

	size_t block_size_ = Board::StreamConf::Audio::MaxBlockSize;

	if (codec_.init() == Board::CodecT::CODEC_NO_ERR)
		pr_info("Codec initialized\n");
	else
		pr_err("ERROR: No codec detected\n");

	audio_blocks[0].in_codec = {audio_in_block.codec[0].data(), block_size_};
	audio_blocks[1].in_codec = {std::next(audio_in_block.codec[0].begin(), block_size_), block_size_};

	audio_blocks[0].out_codec = {audio_out_block.codec[0].data(), block_size_};
	audio_blocks[1].out_codec = {std::next(audio_out_block.codec[0].begin(), block_size_), block_size_};

	codec_.set_tx_buffer(audio_blocks[0].out_codec, block_size_);
	codec_.set_rx_buffer(audio_blocks[0].in_codec, block_size_);

	auto audio_callback = [this]<unsigned block>() {
		// Debug::Pin0::high();
		this->controls.start_param_block<1 - block>();
		module.process(audio_blocks[block], param_blocks[block]);
		// Debug::Pin0::low();
	};

	codec_.set_callbacks([audio_callback]() { audio_callback.operator()<1>(); },
						 [audio_callback]() { audio_callback.operator()<0>(); });
}

void AudioStream::start() {
	module.init(settings);
	codec_.start();
}

// Happens outside audio context:
bool AudioStream::settings_changed() {
	if (module.should_write_settings) {
		module.should_write_settings = false;

		// There's a chance this function gets interrupted and the state is
		// partially the old and partially the new values.
		// However, in this module all fields of the state are atomically written, so
		// it would still be a valid state.
		module.update_settings(settings);

		return true;
	}
	return false;
}

uint32_t AudioStream::get_audio_errors() {
	return codec_.get_sai_errors();
}

} // namespace MetaModule
