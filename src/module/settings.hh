#pragma once
#include <cstdint>

// This is stored in flash, and automatically wear-leveled.
// The MarkerByte is used to make the wear-leveling optimized,
// letting it skip segments that don't start with the MarkerByte.
//
struct UserSettings {
	static constexpr uint8_t MarkerByte = 0x01; //any arbitrary byte, just not 0xFF or 0x00
	static_assert(MarkerByte != 0 && MarkerByte != 0xFF);

	uint8_t marker{MarkerByte};

	// Add all your settings fields here. They can be any type/class
	bool expo_mode{true};

	void reset_to_default() {
		*this = {};
	}

	bool validate() {
		// If you have strict valid/invalid requirements for field values, you can check them here
		return (marker == MarkerByte);
	}
};
