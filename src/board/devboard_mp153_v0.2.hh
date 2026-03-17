#pragma once
#include "drivers/adc_builtin_conf.hh"
#include "drivers/pin.hh"

namespace DevBoard
{

using mdrivlib::GPIO;
using mdrivlib::PinDef;
using mdrivlib::PinNum;

constexpr inline PinDef A1 = {GPIO::D, PinNum::_3};
constexpr inline PinDef A3 = {GPIO::A, PinNum::_8};
constexpr inline PinDef A5 = {GPIO::B, PinNum::_9};
constexpr inline PinDef A7 = {GPIO::C, PinNum::_8};
constexpr inline PinDef A9 = {GPIO::C, PinNum::_9};
constexpr inline PinDef A11 = {GPIO::E, PinNum::_4};
constexpr inline PinDef A13 = {GPIO::C, PinNum::_10};
constexpr inline PinDef A15 = {GPIO::C, PinNum::_11};
constexpr inline PinDef A17 = {GPIO::D, PinNum::_2};
constexpr inline PinDef A19 = {GPIO::C, PinNum::_12};

constexpr inline PinDef A39 = {GPIO::C, PinNum::_7};
constexpr inline PinDef A41 = {GPIO::A, PinNum::_10};
constexpr inline PinDef A43 = {GPIO::E, PinNum::_0};
constexpr inline PinDef A45 = {GPIO::D, PinNum::_12};
constexpr inline PinDef A47 = {GPIO::D, PinNum::_13};
constexpr inline PinDef A49 = {GPIO::D, PinNum::_11};
constexpr inline PinDef A51 = {GPIO::G, PinNum::_10};

constexpr inline PinDef A55 = {GPIO::B, PinNum::_10};
constexpr inline PinDef A57 = {GPIO::G, PinNum::_9};
constexpr inline PinDef A59 = {GPIO::A, PinNum::_12};
constexpr inline PinDef A61 = {GPIO::A, PinNum::_11};
constexpr inline PinDef A63 = {GPIO::E, PinNum::_10};

constexpr inline PinDef B2 = {GPIO::E, PinNum::_3};
constexpr inline PinDef B4 = {GPIO::D, PinNum::_10};
constexpr inline PinDef B6 = {GPIO::B, PinNum::_4};
constexpr inline PinDef B8 = {GPIO::G, PinNum::_6};
constexpr inline PinDef B10 = {GPIO::B, PinNum::_7};
constexpr inline PinDef B12 = {GPIO::E, PinNum::_11};
constexpr inline PinDef B14 = {GPIO::C, PinNum::_6};
constexpr inline PinDef B16 = {GPIO::E, PinNum::_6};
constexpr inline PinDef B18 = {GPIO::G, PinNum::_15};
constexpr inline PinDef B20 = {GPIO::E, PinNum::_12};
constexpr inline PinDef B22 = {GPIO::E, PinNum::_13};

constexpr inline PinDef B30 = {GPIO::A, PinNum::_0};
constexpr inline PinDef B32 = {GPIO::A, PinNum::_3};
constexpr inline PinDef B34 = {GPIO::C, PinNum::_3};
constexpr inline PinDef B36 = {GPIO::C, PinNum::_2};
constexpr inline PinDef B38 = {GPIO::C, PinNum::_1};
constexpr inline PinDef B40 = {GPIO::A, PinNum::_1};
constexpr inline PinDef B42 = {GPIO::B, PinNum::_0};
constexpr inline PinDef B44 = {GPIO::A, PinNum::_2};
constexpr inline PinDef B46 = {GPIO::B, PinNum::_1};
constexpr inline PinDef B48 = {GPIO::C, PinNum::_5};
constexpr inline PinDef B50 = {GPIO::C, PinNum::_4};
constexpr inline PinDef B52 = {GPIO::C, PinNum::_0};
constexpr inline PinDef B54 = {GPIO::A, PinNum::_7};
constexpr inline PinDef B56 = {GPIO::A, PinNum::_5};
constexpr inline PinDef B58 = {GPIO::A, PinNum::_6};
constexpr inline PinDef B60 = {GPIO::A, PinNum::_4};
constexpr inline PinDef B62 = {GPIO::F, PinNum::_11};

constexpr inline PinDef B70 = {GPIO::E, PinNum::_2};
constexpr inline PinDef B72 = {GPIO::G, PinNum::_13};
constexpr inline PinDef B74 = {GPIO::B, PinNum::_8};
constexpr inline PinDef B76 = {GPIO::B, PinNum::_2};

namespace AdcChan
{
constexpr inline auto B30 = mdrivlib::AdcChanNum::_16;
constexpr inline auto B32 = mdrivlib::AdcChanNum::_15;
constexpr inline auto B34 = mdrivlib::AdcChanNum::_13;
constexpr inline auto B36 = mdrivlib::AdcChanNum::_12;
constexpr inline auto B38 = mdrivlib::AdcChanNum::_11;
constexpr inline auto B40 = mdrivlib::AdcChanNum::_17;
constexpr inline auto B42 = mdrivlib::AdcChanNum::_9;
constexpr inline auto B44 = mdrivlib::AdcChanNum::_14;
constexpr inline auto B46 = mdrivlib::AdcChanNum::_5;
constexpr inline auto B48 = mdrivlib::AdcChanNum::_8;
constexpr inline auto B50 = mdrivlib::AdcChanNum::_4;
constexpr inline auto B52 = mdrivlib::AdcChanNum::_10;
constexpr inline auto B54 = mdrivlib::AdcChanNum::_7;
constexpr inline auto B56 = mdrivlib::AdcChanNum::_19;
constexpr inline auto B58 = mdrivlib::AdcChanNum::_3;
constexpr inline auto B60 = mdrivlib::AdcChanNum::_18;
constexpr inline auto B62 = mdrivlib::AdcChanNum::_2;
} // namespace AdcChan

} // namespace DevBoard
