#pragma once
#include <cstdint>

struct UserSettings {
	uint8_t version{0x01}; //any arbitray byte, just not 0xFF or 0x00

	bool expo_mode{true};

	void reset_to_default() {
		*this = {};
	}

	bool validate() {
		return (version == 0x01);
	}
};
