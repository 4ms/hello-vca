# Debugging


## Attaching the JTAG Debugger

1. Attach a JTAG debugger to the 10-pin connector at the top of the module labeled
"SWD/JTAG". The protocol is actually JTAG, despite the header's name, though SWD may
work since the only difference is the tRST pin instead of NRST.

2. Install a "Freeze Jumper" on the two left-most pins of the 2x4 debug header.
This is the header located next to the SWD/JTAG header that contains the connections for
the UART (RX/TX).

```
     _      RX  TX
    |o|  o   o   o
    |o|  o   o   o
     -
==================================== <<< PCB
```

3. Power off and back on (full power-cycle is required).

The console will show:

```
Freeze pin detected active: booting from DDR
Please load a multi-uimg binary into an address in DDR RAM.
Then write the address to the TAMP_BKP6 register at 0x5C00A118
System will hang until TAMP_BKP6 register is changed...
```

At this point you can choose to debug with gdb, Segger Ozone, VSCode, or TRACE32.
Skip to the relevant section below.

## GDB

First, you need to start a gdb server. Either OpenOCD (v0.12.0) or Segger JLinkGDBServer
(v7.92) are known to work.

For OpenOCD, run this in a separate terminal window:

```
make start-openocd
```

For JLinkGDBServer, run this in a separate terminal window:

```
make start-jlinkgdb
```

This command expects the JLink GDB executable to be `JLinkGDBServer`. If that's not the case
on your system, adjust the following command and use it instead. You also may need
to adjust the USB port and speed settings.

```
JLinkGDBServer -select USB=0 -device STM32MP15xx_A7 -endian little -if JTAG -speed 25000 -noir -noLocalhostOnly -nologtofile -port 3333
```

Or, you can launch the JLinkGDBServer GUI program. Choose JTAG for the
interface, and set the port to 3333 manually (by default it's 2331). Note: if
you want to use a different port than 3333, you can change the first line in the
gdbscript (see next step).

Now that the gdb server is running in a separate terminal window, start
debugging using arm-none-eabi-gdb:

```
make debug
```

This will launch arm-none-eabi-gdb and run the `multi.gdbinit` script. The
script is very short and you should inspect the contents if anything is not
working. All it does is connect to the gdb server at port 3333, and then load
the symbols from the A7 elf file into gdb (not onto the board). Then it loads
the multi-uimg file (`build/main.uimg`) to an arbitrary address, 0xC0000000.
Finally, it notifies the bootloader to start parsing the multi-uimg file by
writing 0xC0000000 to a designated register (the TAMP_BKP6R register).

The loading may take a while, depending on the debugger hardware (ST-LINK or JLink)
and the binary size.


## Ozone

Instead of using command-line gdb, you can also use Segger's Ozone GUI program.
Ozone 3.28d is known to work.

Open Ozone and load the project file `flashing/metamodule.jdebug`.

After power-cycling with the Freeze Jumper installed, click the green "Power" icon,
or select "Download & Reset Program" from the Debug > Start Debug Session menu.
Note that this will not actually reset the program since the reset behavior has been
overridden via the project file.

## TRACE32

Lauterbach TRACE32 can be used to debug as well.

After power-cycling with the Freeze Jumper installed, run the
`flashing/mm-a7-t32.cmm` script to load the firmware and symbols.

Both A7 cores can be accessed (use the command `Core 0` for the main core, or
`Core 1` to switch to the secondary A7 core). To debug the M4 core, you can
use the `flashing/switch-to-m4.cmm` script.


## Using VSCode

VSCode can be used to debug, using OpenOCD or JLinkGDBServer.

An example `launch.json` file is in the `flashing/vscode-example/` directory.
Put this file into your `.vscode` directory, or otherwise merge its contents
with your existing `launch.json` file. You will probably want to customize this
configuration file, but it should work as-is.

After rebooting with the Freeze Jumper installed, start a debugging session,
and it will load the firmware and then break at main.

The Cortex-Debug plugin is required. The example configuration is for JLink,
but Cortex-Debug also works with OpenOCD. See the [Cortex-Debug
wiki](https://github.com/Marus/cortex-debug/wiki) for details.


## GPIO Pin Debugging (Pin Flipping)

You can toggle some GPIO pins to indicate states from firmware with minimal
impact on firmware timing. Typically you would read the pins using an
oscilloscope or logic probe.

There are several header pins on the PCB dedicated to this.

```
   Debug::Pin0------|   |------- UART RX (from computer to board)
                    |   |        or Debug::Pin2
 Debug::Pin1        v   v
   or Freeze -> o   o   o   o <- UART TX (from board to computer)
                o   o   o   o
          GND---^   ^   ^   ^--GND on mp15x, +3.3V on mp13x-v0.4.1
                    |   |
                    |---+------ floating: *See note*
================================================================== <<< PCB
```

To use Debug::Pin1 you obviously have to remove the Freeze Jumper, so remember
to re-install it before rebooting.

To use Debug::Pin2, you have to put a call to `Debug::Pin2::init()` somewhere
after the UART is initialized.

*Note: The two floating pins can be set to GND or to Debug::Pin3/Pin4, by
blobbing solder jumper JP2 and/or JP3 on the reverse side of the header. Take
care when blobbing and verify before and after with a multimeter that you
didn't short anything out.*

Usage:
```c++
#include "debug.hh"

Debug::Pin0::high();
Debug::Pin0::low();
Debug::Pin1::set(true); //same as ::high()
Debug::Pin1::set(false); //same as ::low()
```


## Console Output (printf Debugging)

You can view the console output by connecting a USB-UART cable to the TX pin of
the debug header (next to the SWD header). The TX pin is labeled (upper-right
pin).
**CAUTION**: On some mp13x boards, the pin underneath the TX is +3.3V. On all
mp15x and some mp13x boards, this pin is ground. There are jpg images floating
around emails and Google Docs that show this pin being used as ground, so
always check before connecting (it could fry your board, UART adapter, or even
your USB port if you're really unlucky).

Settings are 115200, 8N1.

Use `pr_dbg()`, `pr_warn()`, `pr_err()`, and `pr_trace()` for debug output. These
require the `pr_dbg.hh` header.

Make sure to set the console log level equal to or lower than what you want to see. The levels are:

- NONE (no console output)
- ERROR (`pr_err` only)
- WARN (`pr_warn` and above)
- INFO (`pr_info` and above)
- DEBUG (`pr_dbg` and above)
- TRACE (`pr_trace` and above)
- DUMP (`pr_dump` and above)

The default log level is DEBUG. To set a different log level, configure with cmake like this:

```
cmake --fresh --preset full -GNinja -DLOG_LEVEL=TRACE
```

The convention for each level is as follows:

- Error: unexpected conditions that may lead to program failure
- Warn: conditions that deserve attention but do not lead to program failure
- Info: important or useful information that is neither a warning nor an error
- Debug: temporary messages used for debugging
- Trace: detailed log of execution
- Dump: large amounts of verbose data

Debug (`pr_dbg`) is particularly useful to use ONLY when debugging. Then, when the
feature is ready to merge, it's easy to search for `pr_dbg` and remove all
instances (or selectively replace with a more suitable level).
This technique reduces clutter in the console as you continually add features.
