# Loading Firmware

You have several choices for how to load the firmware application. Each one is covered
in a section below:

1. [Load in RAM over SWD/JTAG](#load-in-ram-over-swdjtag) (requires mp1-boot installed on internal flash or SD card)

2. [Boot from SD card](#boot-from-sd-card)

3. [Load into NOR Flash](#load-into-nor-flash-by-booting-from-an-sd-card-that-runs-norflash-writer-to-copy-the-application--bootloaders-to-flash) by booting from an SD card that runs a program to copy the application + bootloaders to flash


## Load in RAM over SWD/JTAG


This is the preferred method for active firmware development. It requires a
JTAG programmer.

Attach a JTAG debugger to the 10-pin connector labeled "SWD/JTAG". The protocol is
actually JTAG, though SWD may work since the only difference is the tRST pin
instead of NRST.

If you are already running the application and just need to debug, you can just
attach without loading.

If you need to load new firmware and then debug it, then follow the guide in
[Debugging with gdb](firmware-debugging.md).


### Freeze Jumper

To load firmware with a JLink programmer, you need to install a "Freeze Jumper".

The jumper goes on the two left-most pins of the 2x4 debug header. This is the
header located next to the SWD/JTAG header that contains the connections for
the UART (RX/TX).

Looking at the pins (so the pins are pointing at your eyes):

```
     _      RX  TX
    |o|  o   o   o
    |o|  o   o   o
     -
================================ <<< PCB
```

### Loading Firmware

Power off and back on (full power-cycle is required).

The UART console will show:

```
Freeze pin detected active: booting from DDR
Please load a multi-uimg binary into an address in DDR RAM.
Then write the address to the TAMP_BKP6 register at 0x5C00A118
System will hang until TAMP_BKP6 register is changed...
```

Connect a JLink programmer and run this:

```
make jprog
```

This should take a couple seconds at most. The firmware should boot automatically.


## Boot from SD Card

You need a dedicated SD card; all contents will be erased. A 16GB card is common and works fine,
but smaller or larger should work too.

You first need to format, partition, and install the bootloader on the card. This only needs
to happen once when you use a new SD card.

```
make format-sd
```

This will ask you for the device path (`/dev/disk4`, for example). Make sure you get it right, because the
script will run `mkfs` or `diskutil eraseDisk`. Double-check! You can
accidentally erase your hard drive, so be CAREFUL.

After running this, you will need to eject and re-insert the SD card because the volumes have changed.

Then do:

```
make flash-bootloader-sd
```

This will build the bootloader (mp1-boot) and use `dd` to load it onto the first two partitions of the SD card.
You will again be asked for the drive name.

You now have a bootable SD card. You shouldn't need to repeat the above steps unless you get a new SD card.

To flash the application, do this:

```
make flash-app-sd
```

This will build the application as normal, and then use `dd` to copy it to the fourth partition.

Eject the card and insert it into the board.

To tell the board to boot using the SD card, you need to change the BOOT DIP switches.
They are labeled "BOOT0_2". There are two switches. Look at the diagram printed on the PCB.
To boot with SD, both switches should be pushed to the left.
If you want to go back to booting from flash (internal flash chip), then flip the bottom switch to the right.


## Load into NOR Flash by Booting from an SD Card that Runs norflash-writer to Copy the Application + Bootloaders to Flash

This sounds complex, but is actually a streamlined way to get firmware onto production units.

It requires a dedicated SD card that's used to flash the bootloaders and application onto the internal NOR flash on the board.

First, prepare the SD card:

```
make format-sd
```

Next, build the norflash-writer helper app:

```bash
make flash-norwriter
```

Next, follow the instructions it printed out (replace `/dev/diskXN` with the path to the Nth partition on your computer):

```bash
sudo dd if=bootloader/mp1-boot/norwriter/norwriter.stm32 of=/dev/diskX1
sudo dd if=bootloader/mp1-boot/norwriter/norwriter.stm32 of=/dev/diskX2
sudo dd if=build/prod.uimg of=/dev/diskX4
```

The `prod.uimg` file contains the bootloader and the application. What the norwriter helper does is boot up,
initialize the flash chip, and then install the bootloader and application onto the NOR flash chip.

Now, flip the BOOT switches to SD card (see the previous section). Boot from the SD card.
The console will tell you when it's done.

Power down, remove the SD card, and flip the BOOT switches back to flash boot.

Power up and it should run your application normally.

---

**Next: [Debugging](firmware-debugging.md)**
