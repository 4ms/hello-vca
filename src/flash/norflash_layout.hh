#pragma once
#include <cstdint>

// Assumes a 16MB chip, feel free to adjust if you use a cheaper/smaller chip
//
// First 512kB is always reserved for bootloaders.
// The app always starts after that, at 0x80000
// The last 4k is for Settings (can be adjusted or moved)

static constexpr uint32_t SettingsFlashSize = 4096;
static constexpr uint32_t SettingsFlashOffset = 16 * 1024 * 1024 - SettingsFlashSize;
