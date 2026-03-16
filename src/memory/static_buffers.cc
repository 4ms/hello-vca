#include "conf/pin_conf.hh"
#include "param_block.hh"
#include <array>

namespace MetaModule
{

// StaticBuffers sets up our regions of memory that have specific requirements in the target chip architecture.
// The sections are defined in the linker file.
namespace StaticBuffers
{

//If DMA Burst mode is enabled, See RM sec 18.3.12;
//The burst configuration has to be selected in order to respect the AHB protocol, where bursts must not cross the 1 Kbyte address boundary because the minimum address space that can be allocated to a single slave is 1 Kbyte. This means that the 1-Kbyte address boundary must not be crossed by a burst block transfer, otherwise an AHB error is generated, that is not reported by the DMA registers.

__attribute__((section(".noncachable"))) Board::StreamConf::Audio::AudioInBlock audio_in_dma_block{};
__attribute__((section(".noncachable"))) Board::StreamConf::Audio::AudioOutBlock audio_out_dma_block{};
__attribute__((section(".noncachable"))) DoubleBufParamBlock param_blocks{};
alignas(16) __attribute__((section(".noncachable"))) std::array<uint16_t, Board::Adc1Confs.size()> adc1_vals{};
alignas(16) __attribute__((section(".noncachable"))) std::array<uint16_t, Board::Adc2Confs.size()> adc2_vals{};

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

} // namespace MetaModule
