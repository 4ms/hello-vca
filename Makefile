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
	cmake --fresh --preset ${PRESET} -G ${GEN} -DLOG_LEVEL=DEBUG


flash-norwriter: 
	cmake --build --preset ${PRESET} -- bootloader 
	cmake --build --preset ${PRESET} -- norflash-writer 
	cmake --build --preset ${PRESET}
	ls -l bootloader/mp1-boot/norwriter/norwriter.stm32
	$(info Run 'sudo dd if=bootloader/mp1-boot/norwriter/norwriter.stm32 of=/dev/diskXs1' and again for partition 2, to copy the norflash-writer-fsbl to an sd card)
	$(info Then run 'sudo dd if=build/prod.uimg of=/dev/diskXs4' to copy the fsbl+app payload)


# Auto-complete these targets:
bootloader tests dep_tests flash-bootloader-sd flash-app-sd flash-dfu jprog debug start-jlinkgdb start-openocd norflash-writer:
	cmake --build --preset ${PRESET} -- $(MAKECMDGOALS)

# Forward any other arguments
%:
	cmake --build --preset ${PRESET} -- $(MAKECMDGOALS)

.PHONY: bootloader tests dep_tests flash-bootloader-sd flash-app-sd flash-dfu jprog debug start-jlinkgdb start-openocd norflash-writer

