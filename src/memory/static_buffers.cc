#include "conf/pin_conf.hh"
#include "param_block.hh"
#include <array>

namespace HelloVCA
{

// StaticBuffers sets up our regions of memory that have specific requirements in the target chip architecture.
// The sections are defined in the linker file.
namespace StaticBuffers
{

// These must be noncache-able because they are used by the SAI audio DMA:
__attribute__((section(".noncachable"))) Board::StreamConf::Audio::AudioInBlock audio_in_dma_block{};
__attribute__((section(".noncachable"))) Board::StreamConf::Audio::AudioOutBlock audio_out_dma_block{};

// These must be noncache-able because they are used by the ADC DMA:
alignas(16) __attribute__((section(".noncachable"))) std::array<uint16_t, Board::Adc1Confs.size()> adc1_vals{};
alignas(16) __attribute__((section(".noncachable"))) std::array<uint16_t, Board::Adc2Confs.size()> adc2_vals{};

// Params do not need to be non-cacheable for single core:
DoubleBufParamBlock param_blocks{};

// For dual-core, the params are used by both M4 and A7 cores, so needs to be non-cacheable:
// __attribute__((section(".noncachable"))) DoubleBufParamBlock param_blocks{};

void init() {
	for (auto &block : param_blocks) {
		for (auto &param : block.params) {
			param.clear();
		}
		block.metaparams.clear();
	}

	//clear buffers
	for (auto &buff : audio_out_dma_block.codec) {
		for (auto &frame : buff)
			frame = Board::StreamConf::Audio::AudioOutFrame{};
	}

	for (auto &buff : audio_in_dma_block.codec) {
		for (auto &frame : buff)
			frame = Board::StreamConf::Audio::AudioInFrame{};
	}
}

}; // namespace StaticBuffers

} // namespace HelloVCA
