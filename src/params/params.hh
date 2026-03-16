#pragma once
#include <array>

namespace HelloVCA
{

// Sample-rate parameters
struct Params {
	// See conf/analog_pins.hh for list:
	std::array<float, 2> analog_ins{};

	// Add any more params you ened to keep track of

	void clear() {
		*this = Params{};
	}
};

static constexpr auto Params_Size = sizeof(Params);

} // namespace HelloVCA
