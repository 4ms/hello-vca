#pragma once
#include "conf/codec_sai_conf.hh"
#include "conf/i2c_shared_conf.hh"
#include "drivers/codec_PCM3060.hh"

namespace Board
{

using CodecT = mdrivlib::CodecPCM3060;

struct Hardware {
	static inline mdrivlib::I2CPeriph codec_i2c{Board::a7m4_shared_i2c_conf};
	static inline CodecT codec{codec_i2c, Board::codec_sai_conf};
};

} // namespace Board
