#include "app_startup.hh"
#include "audio/audio.hh"
#include "controls/controls.hh"
#include "debug.hh"
#include "flash/settings_flash.hh"
#include "flash_loader/flash_loader.hh"
#include "git_version.h"
#include "memory/static_buffers.hh"
#include "uart_log.hh"

namespace MetaModule
{

struct SystemInit : AppStartup, UartLog, Debug, Board::Hardware {
} _sysinit;

} // namespace MetaModule

int main() {
	using namespace HelloVCA;

	StaticBuffers::init();

	HAL_Delay(50);
	pr_info("Build: %s (%s)\n", GIT_HASH.data(), GIT_COMMIT_TIME.data());

	FlashLoader flash_loader;
	SettingsStorage settings{flash_loader.flash_driver()};

	Controls controls{StaticBuffers::param_blocks};

	AudioStream audio{StaticBuffers::audio_in_dma_block,
					  StaticBuffers::audio_out_dma_block,
					  StaticBuffers::param_blocks,
					  controls,
					  settings.settings};

	pr_info("A7 Core 1 initialized\n");

	controls.start();
	audio.start();

	while (true) {
		controls.process();

		if (audio.get_audio_errors() > 0) {
			pr_err("Audio error\n");
			audio.start();
		}

		if (audio.settings_changed()) {
			pr_info("Writing settings...\n");
			settings.write();
		}
	}
}
