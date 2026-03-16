#pragma once
#include <cstdint>

// Assume a 2MB chip
// Last 4k is for Calibration
// 4k before that is for Settings
// First 512kB is for bootloaders
// Rest is for the app (1.5MB - 8k)

static constexpr uint32_t SettingsFlashOffset = 2 * 1024 * 1024 - (4096 * 2);
static constexpr uint32_t SettingsFlashSize = 4096;

static constexpr uint32_t CalDataFlashOffset = 2 * 1024 * 1024 - 4096;
static constexpr uint32_t CalDataFlashSize = 4096;
