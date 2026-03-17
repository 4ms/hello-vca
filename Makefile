PRESET ?= full

# Detect MinGW and use Make
ifdef SYSTEMROOT
GEN := "Unix Makefiles"
else
GEN := Ninja
endif

all:
	cmake --build --preset ${PRESET} 

configure:
	cmake --fresh --preset ${PRESET} -G ${GEN}


flash-norwriter: 
	cmake --build --preset ${PRESET} -- bootloader 
	cmake --build --preset ${PRESET} -- norflash-writer 
	cmake --build --preset ${PRESET}
	ls -l bootloader/mp1-boot/norwriter/norwriter.stm32
	@echo
	@echo "******************************************************************************"
	@echo "To create an SD card that flashes your production image (bootloaders + app) onto a board's flash chip:"
	@echo "1) Format the card with 'flashing/format-partition-sdcard.sh'"
	@echo "2) Run these (replace diskX1/2/4 with your actual sd card device path to partitions 1, 2 and 4):"
	@echo "   sudo dd if=bootloader/mp1-boot/norwriter/norwriter.stm32 of=/dev/diskX1"
	@echo "   sudo dd if=bootloader/mp1-boot/norwriter/norwriter.stm32 of=/dev/diskX2"
	@echo "   sudo dd if=build/prod.uimg of=/dev/diskX4"
	@echo "******************************************************************************"

# Auto-complete these targets:
bootloader tests dep_tests flash-bootloader-sd flash-app-sd flash-dfu jprog debug start-jlinkgdb start-openocd:
	cmake --build --preset ${PRESET} -- $(MAKECMDGOALS)

# Forward any other arguments
%:
	cmake --build --preset ${PRESET} -- $(MAKECMDGOALS)

.PHONY: bootloader tests dep_tests flash-bootloader-sd flash-app-sd flash-dfu jprog debug start-jlinkgdb start-openocd 

