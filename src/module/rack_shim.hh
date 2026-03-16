#pragma once
#include "board/conf/stream_conf.hh"
#include "math.hh"
#include "settings_json.hh"
#include <array>
#include <string_view>

namespace rack
{

struct ParamQuantity {
	float minValue = 0.f;
	float maxValue = 1.f;
	float defaultValue = 0.f;
	bool integral = false;
};

struct Param {
	float value; //value is scaled to min/max range
	ParamQuantity pq;

	float getValue() const {
		return value;
	}
	void setValue(float v) {
		value = v;
	}

	void setFrom01(float v) {
		value = v * (pq.maxValue - pq.minValue) + pq.minValue;
	}

	float get01() const {
		return (value - pq.minValue) / (pq.maxValue - pq.minValue);
	}
};

struct Jack {
	float voltage = 0;
	bool is_connected = false;

	float getVoltage() const {
		return voltage;
	}
	void setVoltage(float v) {
		voltage = v;
	}
	bool isConnected() const {
		return is_connected;
	}
};

struct Light {
	float value;

	float getBrightness() const {
		return value;
	}

	void setBrightness(float v) {
		value = v;
	}

	void setBrightnessSmooth(float v, float time) {
		value = v;
	}
};

struct ProcessArgs {
	float sampleRate = Board::StreamConf::Audio::SampleRate;
	float sampleTime = 1.f / sampleRate;
};

struct Module {
	Param params[13];

	Light lights[28];
	Jack inputs[3];
	Jack outputs[2];

	void config(int, int, int, int) {
	}

	void configSwitch(int id, float min, float max, float def, std::string_view, std::array<std::string_view, 3> = {}) {
		params[id].pq.minValue = min;
		params[id].pq.maxValue = max;
		params[id].pq.defaultValue = def;
		params[id].pq.integral = true;
	}

	void
	configParam(int id, float min, float max, float def, std::string_view, std::string_view, float = 0, float = 1) {
		params[id].pq.minValue = min;
		params[id].pq.maxValue = max;
		params[id].pq.defaultValue = def;
		params[id].pq.integral = false;
	}

	void configInput(int, std::string_view) {
	}
	void configOutput(int, std::string_view) {
	}

	virtual void process(const ProcessArgs &args) {
	}

	virtual json_t *dataToJson() {
		return nullptr;
	}

	virtual void dataFromJson(json_t *) {
	}
};

static int *pluginInstance;
constexpr float RACK_GRID_WIDTH = 0;
constexpr float RACK_GRID_HEIGHT = 0;

namespace asset
{
inline int plugin(int *, std::string_view) {
	return 0;
}
} // namespace asset

struct ScrewBlack {};
struct _9mmKnob {};
struct LEDBezel {};
struct _3PosHorizontal {};
struct Davies1900hBlack {};
struct PJ301MPort {};
struct RedLight {};
struct YellowLight {};
struct GreenLight {};
struct BlueLight {};
template<typename T>
struct MediumLight {};

struct Vec {
	float x;
	float y;
};
struct Rect {
	Vec pos;
	Vec size;
};

inline Vec mm2px(Vec) {
	return {};
}

struct ModuleWidget {
	Rect box;

	void setModule(Module *) {
	}

	int createPanel(auto x) {
		return 0;
	}

	void setPanel(int) {
	}

	template<typename T>
	int createWidget(Vec) {
		return 0;
	}

	template<typename T>
	int createParamCentered(Vec, Module *, int) {
		return 0;
	}

	template<typename T>
	int createInputCentered(Vec, Module *, int) {
		return 0;
	}

	template<typename T>
	int createOutputCentered(Vec, Module *, int) {
		return 0;
	}

	template<typename T>
	int createLightCentered(Vec, Module *, int) {
		return 0;
	}

	void addChild(auto x) {
	}

	void addParam(auto x) {
	}

	void addInput(auto x) {
	}

	void addOutput(auto x) {
	}
};

struct Model {};

template<typename T, typename U>
Model *createModel(std::string_view) {
	return nullptr;
}

} // namespace rack
