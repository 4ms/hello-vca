cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/ccache.cmake)

set(CMAKE_BUILD_TYPE "RelWithDebInfo")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/arm-none-eabi-gcc.cmake)

#TODO: select:
# - MP13x which forces SINGLE_CORE
# - MP15x in MULTI_CORE mode (M4 does controls, A7 does audio)
# - MP15x in SINGLE_CORE

set(SINGLE_CORE ON)

project(
  MetaModule
  VERSION 0.10
  DESCRIPTION "MetaModule"
  LANGUAGES C CXX ASM
)

#
# Git commit hash
#
include(cmake/CheckGit.cmake)
CheckGitSetup()

#
# Main Application
#

add_subdirectory(src)

#

#
# Package Release
#
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/main.bin
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:main.elf> ${CMAKE_CURRENT_BINARY_DIR}/main.bin
  COMMAND ls -l ${CMAKE_CURRENT_BINARY_DIR}/main.bin
  DEPENDS main.elf AlwaysCheckGit
  COMMENT "Creating main.bin file"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM USES_TERMINAL
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/main.uimg
  COMMAND flashing/elf_to_uimg.py
          # -v  #Uncomment this to have verbose output
          --a7 $<TARGET_FILE:main.elf>
          ${CMAKE_CURRENT_BINARY_DIR}/main.uimg
  COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/main.uimg ${CMAKE_CURRENT_BINARY_DIR}/main-uimg.bin #Jlink and Ozone require .bin extension
  COMMAND ls -l ${CMAKE_CURRENT_BINARY_DIR}/main.uimg 
  DEPENDS main.elf AlwaysCheckGit
  COMMENT "Creating main uimg file"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM USES_TERMINAL
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/prod.uimg
  COMMAND flashing/bin_to_uimg.py
          # -v  #Uncomment this to have verbose output
          --a7 ${CMAKE_CURRENT_BINARY_DIR}/main.uimg
          --a7-lma 0x70080000
          --a7-entry 0xC0100040
          --fsbl ${CMAKE_CURRENT_SOURCE_DIR}/bootloader/mp1-boot/build/fsbl.stm32
          --fsbl-lma 0x70000000
          --fsbl-lma2 0x70040000
          ${CMAKE_CURRENT_BINARY_DIR}/prod.uimg
  COMMAND ls -l ${CMAKE_CURRENT_BINARY_DIR}/prod.uimg 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/main.uimg ${CMAKE_CURRENT_SOURCE_DIR}/bootloader/mp1-boot/build/fsbl.stm32
  COMMENT "Creating combined uimg file"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM USES_TERMINAL
)

add_custom_target(
  mainbin ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/main.bin ${CMAKE_CURRENT_BINARY_DIR}/prod.uimg ${CMAKE_CURRENT_BINARY_DIR}/main.uimg
)



#
# Bootloaders: MP1-Boot and Audio bootloader (TODO)
# NORFLASH_WRITER writes prod.uimg (production fsbl+ssbl+app) to nor flash, via SD Card
#
if ("${TOOLCHAIN_BASE_DIR}" STREQUAL "")
	set(BOOTLOADER_TOOLCHAIN_OPT "")
else()
	set(BOOTLOADER_TOOLCHAIN_OPT "TOOLCHAIN_DIR=${TOOLCHAIN_BASE_DIR}/")
endif()

add_custom_target(
  bootloader
  COMMAND make --no-print-directory SERIES=stm32mp13x BOARD_CONF=../board_conf/mp13x-devboard-v0.4.hh ${BOOTLOADER_TOOLCHAIN_OPT}
  COMMENT "Building MP1-Boot (FSBL)"
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bootloader/mp1-boot
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/bootloader/mp1-boot/build/fsbl.stm32
  DEPENDS bootloader
)

add_custom_target(
  norflash-writer
  COMMAND make --no-print-directory SERIES=stm32mp13x BOARD_CONF=../board_conf/lc_mp1boot_conf.hh  NORFLASH_WRITER=1 BINARYNAME=norwriter BUILDDIR=norwriter ${BOOTLOADER_TOOLCHAIN_OPT}
  COMMENT "Building MP1-Boot (NORFLASH_WRITER)"
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bootloader/mp1-boot
  VERBATIM
)


# add_custom_target(
#   bootloader-dfu
#   COMMAND make --no-print-directory BOARD_CONF=../../board_conf/dfu-mp153-devboard-v0.2.hh ${BOOTLOADER_TOOLCHAIN_OPT}
#   COMMENT "Building DFU Bootloader"
#   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bootloader/mp1-usb-dfu
#   VERBATIM
# )

#
# Tests
#
set(LIBDIR ${PROJECT_SOURCE_DIR}/lib)

add_custom_target(
  tests ALL
  COMMAND cd ${PROJECT_SOURCE_DIR} && make --no-print-directory -f tests/Makefile -j8
  COMMENT "Running unit tests"
  VERBATIM USES_TERMINAL
)
file(
  GLOB
  tests_clean_files
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/tests/build/*"
)
set_target_properties(tests PROPERTIES ADDITIONAL_CLEAN_FILES "${tests_clean_files}")

add_custom_target(
  dep_tests
  COMMAND cd ${LIBDIR}/cpputil && make --no-print-directory -f tests/Makefile -j8
  COMMAND cd ${PROJECT_SOURCE_DIR}/lib/mdrivlib && make --no-print-directory -f tests/Makefile -j8
  BYPRODUCTS ${PROJECT_SOURCE_DIR}/lib/mdrivlib/tests/build/* ${LIBDIR}/cpputil/tests/build/*
  VERBATIM USES_TERMINAL
)

set_property(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES
           ${PROJECT_SOURCE_DIR}/tests/build
           ${PROJECT_SOURCE_DIR}/lib/mdrivlib/tests/build
           ${LIBDIR}/cpputil/tests/build
)

#
# Flashing/installing
#
include(flashing/flashing.cmake)

