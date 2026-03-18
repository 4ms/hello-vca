# Hello VCA

An example bare-metal audio firmware project for the STM32MP1, implementing a Voltage Controlled Amplifier (VCA).

## Quick Start

Requirements:

- arm-none-eabi-gcc toolchain v12.2 or newer (tested up to 14.3) on your PATH
- python3
- cmake 3.24 or later
- ninja (optional)

See [Setting Up Your Environment](docs/setup-environment.md) for more details.

```bash
# Clone and get submodules:
git clone https://github.com/4ms/hello-vca
cd hello-vca
git submodule update --init --recursive

# Configure project:
make configure

# Build:
make
```

The `make` commands are shortcuts for using cmake to configure and build the project.

## Developer Docs

1. [Setting Up Your Environment](docs/setup-environment.md)
   - [All Platforms](docs/setup-environment.md#all-platforms)
   - [macOS](docs/setup-environment.md#macos)
   - [Linux](docs/setup-environment.md#linux)
   - [Windows](docs/setup-environment.md#windows)

2. [Building Firmware](docs/building_firmware.md)
   - [Using an SD Card](docs/building_firmware.md#using-an-sd-card)
   - [Specifying the Toolchain](docs/building_firmware.md#specifying-the-toolchain)

3. [Loading Firmware](docs/loading-firmware.md)
   - [Load in RAM over SWD/JTAG](docs/loading-firmware.md#load-in-ram-over-swdjtag)
   - [Boot from SD Card](docs/loading-firmware.md#boot-from-sd-card)
   - [Load into NOR Flash](docs/loading-firmware.md#load-into-nor-flash-by-booting-from-an-sd-card-that-runs-norflash-writer-to-copy-the-application--bootloaders-to-flash)

4. [Debugging](docs/firmware-debugging.md)
   - [GDB](docs/firmware-debugging.md#gdb)
   - [Ozone](docs/firmware-debugging.md#ozone)
   - [TRACE32](docs/firmware-debugging.md#trace32)
   - [VSCode](docs/firmware-debugging.md#using-vscode)
   - [GPIO Pin Debugging](docs/firmware-debugging.md#gpio-pin-debugging-pin-flipping)
   - [Console Output](docs/firmware-debugging.md#console-output-printf-debugging)
