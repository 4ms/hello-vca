#pragma once
#include "conf/stream_conf.hh"
#include "devboard.hh"
#include "drivers/pin.hh"
#include "drivers/sai_config_struct.hh"

namespace Board
{

using mdrivlib::GPIO;
using mdrivlib::PinAF;
using mdrivlib::PinNum;
using mdrivlib::SaiConfig;

const SaiConfig codec_sai_conf = {
	.sai = SAI1,
	.tx_block = SAI1_Block_A,
	.rx_block = SAI1_Block_B,

	.mode = SaiConfig::TXMaster,

	.dma_init_tx =
		{
			.DMAx = DMA1,
			.stream = DMA1_Stream0,
			.channel = DMA_REQUEST_SAI1_A,
			.IRQn = DMA1_Stream0_IRQn,
			.pri = 1,
			.subpri = 1,
		},
	.dma_init_rx =
		{
			.DMAx = DMA1,
			.stream = DMA1_Stream1,
			.channel = DMA_REQUEST_SAI1_B,
			.IRQn = DMA1_Stream1_IRQn,
			.pri = 1,
			.subpri = 1,
		},

	.datasize = SAI_DATASIZE_24,
	.framesize = 64, //24bit extends to 32bits * max(num_tdm_outs, num_td_ins)
	.samplerate = Board::StreamConf::Audio::SampleRate,

	.MCLK = {GPIO::D, PinNum::_0, PinAF::AltFunc2},
	.SCLK = {GPIO::H, PinNum::_12, PinAF::AltFunc6},
	.LRCLK = {GPIO::E, PinNum::_11, PinAF::AltFunc6},  //A45
	.SD_DAC = {GPIO::D, PinNum::_6, PinAF::AltFunc6},  // SD A
	.SD_ADC = {GPIO::G, PinNum::_10, PinAF::AltFunc6}, // SD B

	.reset_pin = DevBoard::A41,

	.bus_address = 0b00,

	.num_tdm_ins = 2,
	.num_tdm_outs = 2,

	.sync_send = SaiConfig::NoSendSync,
	.sync_receive_from = SaiConfig::NoReceiveSync,
};

} // namespace Board
