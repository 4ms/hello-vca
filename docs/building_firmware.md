# Building Firmware

This requires `arm-none-eabi-gcc` version 12.2 or later installed on your PATH.
Please see the [setup guide](setup-environment.md) for some important notes about this.

Make sure you are on the right branch and you have already updated the submodules.

To prepare the build system (only needs to be run once):

```bash
make configure
```

To compile, run:

```bash
make all
```

The `make configure` command is a shortcut for running:

```bash
# macOS, Linux:
cmake --fresh --preset full -GNinja

# MinGW:
cmake --fresh --preset full -G"Unix Makefiles"
```

The `make all` command is a shortcut for running:

```bash
cmake --build --preset full
```

You can set a different `LOG_LEVEL` if you want to control
how much gets sent to the console via UART. See [Debugging](firmware-debugging.md). The default is `DEBUG`.

## Building using docker

There is a Dockerfile you can use to build the binaries.
```bash
docker build -t hello-vca-fw .
docker run --rm -it -v $(pwd):/workspace/hello-vca hello-vca-fw
```

## Building for booting from an SD card

*Optional*: If you plan to flash firmware to an SD card, then you can specify the
path to the SD card device to save time. If you don't do this, then the system
will prompt you whenever you run one of the SD card flashing scripts. The
device path should be to the entire SD card device (not just one partition).

```bash
cmake --preset full -DSD_DISK_DEV=/dev/disk4

# Alternatively, set an environment variable:
export SD_DISK_DEV=/dev/disk4
```

The firmware is built as `build/main.uimg`. This binary file contains
a header that our bootloader uses, and then the raw binary data.
For debugging, the symbols are in `build/src/main.elf`.


## Specifying the Toolchain

*Optional*: If you have multiple versions of the gcc arm toolchain installed and don't want to
change your PATH for this project, you can set the `TOOLCHAIN_BASE_DIR` variable like this:

```bash
# Put in your bashrc/zshrc for convenience:
# Note the trailing slash (required)
export TOOLCHAIN_BASE_DIR=/path/to/arm-gnu-toolchain-12.x-relX/bin/
```

Or specify it in the cmake configuration:

```bash
cmake --fresh --preset full -GNinja -DTOOLCHAIN_BASE_DIR=/path/to/arm-gnu-toolchain-12.x-relX/bin/
```

---

**Next: [Loading Firmware](loading-firmware.md)**
