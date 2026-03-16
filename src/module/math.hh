#pragma once
#include "../../lib/gcem/include/gcem.hpp"
#include "util/lut.hh"

struct ExpfRange {
	static constexpr float min = 0.f;
	static constexpr float max = 2.f;
};

static constexpr auto Expf = LookupTable<1024>::generate<ExpfRange>([](float x) { return gcem::exp(x); });
