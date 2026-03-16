#pragma once
#include "rack_shim.hh"
#include <algorithm>

using namespace rack;

inline float clamp(float x, float mn, float mx) {
	return std::clamp(x, mn, mx);
}

inline float rescale(float x, float xMin, float xMax, float yMin, float yMax) {
	return yMin + (x - xMin) / (xMax - xMin) * (yMax - yMin);
}

inline float crossfade(float a, float b, float p) {
	return a + (b - a) * p;
}
