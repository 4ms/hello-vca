#pragma once

namespace HelloVCA
{

// Block-rate parameters
struct MetaParams {
	// For this example, the gate input is block-rate, just so we have something to demonstrate the use of metaparams
	bool gate_in = 0.f;

	void clear() {
		*this = MetaParams{};
	}
};

} // namespace HelloVCA
