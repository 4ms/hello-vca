#pragma once
#include "conf/rcc_conf.hh"
#include "drivers/rcc.hh"
#include "drivers/stm32xx.h"
#include "drivers/system_clocks.hh"
#if defined(STM32MP13)
#include "stm32mp13xx_ll_etzpc.h"
#endif

namespace MetaModule
{

#if defined(STM32MP13)
static uint32_t GetPLL1PClockFreq(void) {
	uint32_t pllsource, pll1m, pll1fracen, pll1p_freq;
	float fracn1, pll1vco;

	pllsource = (RCC->RCK12SELR & RCC_RCK12SELR_PLL12SRC);
	pll1m = ((RCC->PLL1CFGR1 & RCC_PLL1CFGR1_DIVM1) >> RCC_PLL1CFGR1_DIVM1_Pos) + 1U;
	pll1fracen = (RCC->PLL1FRACR & RCC_PLL1FRACR_FRACLE) >> 16U;
	fracn1 = ((float)(uint32_t)(pll1fracen * ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACV) >> 3)));
	pll1vco = ((float)(uint32_t)(((RCC->PLL1CFGR1 & RCC_PLL1CFGR1_DIVN) + 1U)) + (fracn1 / (float)0x1FFF));

	if (pll1m != 0U) {
		switch (pllsource) {
			case 0x00: /* HSI used as PLL clock source */
				pll1vco *= ((float)(uint32_t)(HSI_VALUE >> (RCC->HSICFGR & RCC_HSICFGR_HSIDIV)) / (float)pll1m);
				break;

			case 0x01: /* HSE used as PLL clock source */
				pll1vco *= ((float)(uint32_t)HSE_VALUE / (float)pll1m);
				break;

			case 0x02: /* No clock source for PLL */
				pll1vco = (float)0;
				break;

			default:
				break;
		}
		pll1p_freq = (((uint32_t)(float)pll1vco) / (uint32_t)((RCC->PLL1CFGR2 & RCC_PLL1CFGR2_DIVP) + 1U));
	} else {
		pll1p_freq = 0U;
	}
	return pll1p_freq;
}
#endif

struct AppStartup {
	AppStartup() {
		using namespace mdrivlib;

#if defined(STM32MP13)
		SystemClocks::init_clocks(rcc_osc_conf, rcc_clk_conf, rcc_periph_clk_conf);

		SystemCoreClock = GetPLL1PClockFreq();

		__HAL_RCC_ETZPC_CLK_ENABLE();
		LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

		// Regardless of the value set here, DMA1/2 does not work with ADC1/2 (only DMA3 does)
		ETZPC->DECPROT2 = 0b0000;

		L1C_CleanDCacheAll();
		__DSB();
		__ISB();
#else
#if defined(DUAL_CORE)
		RCC_Enable::HSEM_::set();

		HWSemaphore<MainCoreReady>::disable_channel_ISR();
		HWSemaphore<MainCoreReady>::lock();

		Copro::reset();

		SystemClocks::init_clocks(rcc_osc_conf, rcc_clk_conf, rcc_periph_clk_conf);

		SecondaryCore::start();

		L1C_CleanDCacheAll();
		__DSB();
		__ISB();

		Copro::start();
#else
		SystemClocks::init_clocks(rcc_osc_conf, rcc_clk_conf, rcc_periph_clk_conf);
		L1C_CleanDCacheAll();
		__DSB();
		__ISB();
#endif
#endif

	}
};

} // namespace MetaModule
