# Project Layout

## `src/module/` — Your DSP Module

This is where you'll spend most of your time. It contains the audio processing
code for your module.

- `module.cc` / `module.hh` — The `Module` class. Its `process()` method is
  called once per audio block with an audio buffer and a parameter block. This
  is where you read audio input, do any DSP you want, and write
  audio output.
- `settings.hh` — `UserSettings` struct for persisting state to flash (e.g.
  expo mode). You can put as much as you want in here, but good practice is
  to keep it small. I.e. use a uint8_t, not a uint32_t to store a value that
  can only be 0, 1, 2 or 3.

The module receives the param values through `ParamBlock`, which contains
per-sample interpolated analog values (knobs, CV) and per-block digital state
(gate jack, LEDs).


## `src/board/conf/` — Hardware Configuration

These headers define how the hardware is wired up. If you're adapting this
project to different hardware, this is where most changes go.

Three files in particular are worth knowing about:

- **`pin_conf.hh`** — Defines all digital I/O: LEDs, buttons, gate jacks, and
  ADC channel-to-pin mappings. This is where you add or change physical
  connections. The ADC channels are defined here as `constexpr` arrays
  (`Adc1Confs`, `Adc2Confs`) that map a board pin to an ADC channel and a
  signal name.

- **`analog_pins.hh`** — Defines the `enum` of analog signal names (e.g.
  `GainCV`, `GainKnob`). These names are used throughout the codebase to refer
  to analog inputs by meaning rather than by channel number. Add new entries
  here when adding new knobs or CV inputs.

Also closely related is **`src/controls/`** (`controls.hh` / `controls.cc`).
This is where the pins defined in `pin_conf.hh` are actually read and
processed — it instantiates the debounced buttons, LED outputs, and ADC
peripherals. When you add new I/O to `pin_conf.hh`, you'll typically update
`controls.hh`/`controls.cc` to match (e.g. adding a new `DebouncedPin`,
wiring a new LED, or adjusting ADC read frequencies).

The remaining conf files are less commonly changed:

| File | Purpose |
|------|---------|
| `stream_conf.hh` | Audio frame/block sizes and sample rate |
| `codec_sai_conf.hh` | SAI peripheral and TDM bus configuration |
| `board_codec_conf.hh` | Codec chip type (PCM3060) |
| `rcc_conf.hh` | Clock tree configuration |
| `i2c_shared_conf.hh` | I2C bus for codec control |
| `adc_periph_conf.hh` | ADC peripheral settings (DMA, clock dividers) |
| `control_conf.hh` | Control read timing (ties param updates to the sample rate) |
| `console_uart_conf.hh` | UART pins and baud rate for debug console |
| `qspi_flash_conf.hh` | QSPI flash chip configuration |


## Other `src/` Directories

| Directory | Purpose |
|-----------|---------|
| `core_a7/` | `main()` entry point — initializes hardware, starts controls and audio, runs the main loop |
| `audio/` | `AudioStream` class — manages the codec and DMA double-buffering, calls `Module::process()` on each audio block |
| `controls/` | `Controls` class — reads ADC inputs, debounces buttons, interpolates parameter values, and fills the `ParamBlock` for the audio thread |
| `params/` | Data structures passed between controls and audio: `ParamBlock`, `Params`, `MetaParams`, `LedStates` |
| `flash/` | Flash chip driver and settings storage |
| `memory/` | Static buffer allocation (DMA buffers, parameter blocks, ADC buffers) |
| `console/` | UART debug logging (`pr_dbg`, `pr_info`, etc.) |


## `system/`

Low-level startup and runtime support. You normally won't need to touch these.

- `startup_ca7.s` — ARM assembly startup (vector table, stack init, branch to `main`)
- `mmu_ca7.c` — MMU page table setup
- `linker/` — Linker scripts defining memory regions (code, RAM, DMA buffers, stacks)
- `libc_stub.c`, `libcpp_stub.cc`, etc. — Minimal libc/libcpp stubs for bare-metal


## `lib/`

External libraries (mostly submodules). You shouldn't need to modify these.

| Library | Purpose |
|---------|---------|
| `mdrivlib` | Hardware abstraction: GPIO, timers, I2C, ADC, SAI/TDM, codecs |
| `cpputil` | C++ utilities: edge detection, filters, interpolation |
| `gcem` | Compile-time math (used to generate constexpr lookup tables) |
| `cmsis` | ARM CMSIS Core-A headers |
| `cmsis-device` | STM32MP1 register definitions |
| `stm32-hal` | ST HAL/LL drivers |


## Other Top-Level Directories

| Directory | Purpose |
|-----------|---------|
| `bootloader/` | mp1-boot FSBL (first-stage bootloader) |
| `flashing/` | Scripts and configs for flashing and debugging (JLink, OpenOCD, Ozone, TRACE32) |
| `tests/` | Unit tests (doctest framework, built and run on host) |
| `cmake/` | CMake toolchain file, architecture configs, build helpers |
| `build/` | Build output (generated) |


