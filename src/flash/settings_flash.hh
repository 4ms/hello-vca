#pragma once
#include "console/pr_dbg.hh"
#include "drivers/qspi_flash_cellblock.hh"
#include "flash/norflash_layout.hh"
#include "module/settings.hh"
#include "util/wear_level.hh"

namespace MetaModule
{

class SettingsStorage {

private:
	WearLeveler<mdrivlib::FlashCellBlock<SettingsFlashOffset, UserSettings, mdrivlib::QSpiFlash::ErasableSizes::SECTOR>>
		settings_storage;

public:
	UserSettings settings{};

	SettingsStorage(mdrivlib::QSpiFlash &qspi)
		: settings_storage{qspi} {

		if (!settings_storage.read(settings)) {
			pr_err("Cannot read user settings\n");
			settings.reset_to_default();
			write();
		}
	}

	void write() {
		if (!settings_storage.write(settings))
			pr_err("Cannot write user settings\n");
	}
};
} // namespace MetaModule
