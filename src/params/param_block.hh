#pragma once
#include "conf/stream_conf.hh"
#include "metaparams.hh"
#include "params.hh"
#include "params/leds.hh"

namespace HelloVCA
{

struct ParamBlock {
	std::array<Params, Board::StreamConf::Audio::MaxBlockSize> params{};
	MetaParams metaparams{};
	LedStates leds{};
};

using DoubleBufParamBlock = std::array<ParamBlock, 2>;

} // namespace HelloVCA
