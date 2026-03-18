#pragma once
#include "../../lib/gcem/include/gcem.hpp"
#include "util/lut.hh"

// This is just an example of how to use the compile-time look up tables for math functions.
// Using these instead of standard library functions like exp(), log(), sin(), etc can have
// huge CPU savings without a controllable amount of precision.
// Always measure and compare performance before and after changing to a LUT.
//
struct ExpfRange {
	static constexpr float min = 0.f;
	static constexpr float max = 2.f;
};

static constexpr auto Expf = LookupTable<1024>::generate<ExpfRange>([](float x) { return gcem::exp(x); });
