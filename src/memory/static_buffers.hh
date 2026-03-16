#pragma once
#include "conf/pin_conf.hh"
#include "params/param_block.hh"

namespace HelloVCA
{

// StaticBuffers sets up our regions of memory that have specific requirements in the target chip architecture.
namespace StaticBuffers
{

extern Board::StreamConf::Audio::AudioInBlock audio_in_dma_block;
extern Board::StreamConf::Audio::AudioOutBlock audio_out_dma_block;
extern DoubleBufParamBlock param_blocks;
extern std::array<uint16_t, Board::Adc1Confs.size()> adc1_vals;
extern std::array<uint16_t, Board::Adc2Confs.size()> adc2_vals;

void init();

}; // namespace StaticBuffers

} // namespace HelloVCA
