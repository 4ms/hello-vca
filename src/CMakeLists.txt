include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

set(FWDIR ${CMAKE_SOURCE_DIR})
set(LIBDIR ${FWDIR}/lib)

include(${CMAKE_SOURCE_DIR}/cmake/arch_mp15xa7.cmake)


include(${CMAKE_SOURCE_DIR}/cmake/log_levels.cmake)
enable_logging()

set_arch_flags()

#
# Driver interface
#
add_library(mdrivlib_interface INTERFACE)
target_include_directories(
  mdrivlib_interface
  INTERFACE 
            ${FWDIR}/lib/cmsis/Core_A/Include
            ${FWDIR}/lib/cmsis/Include
            ${FWDIR}/lib/mdrivlib

            ${FWDIR}/lib/mdrivlib/target/stm32mp1
            ${FWDIR}/lib/mdrivlib/target/stm32mp1_ca7
            ${FWDIR}/lib/stm32-hal/stm32mp15/include

            ${FWDIR}/src/core_a7
)
target_compile_options(mdrivlib_interface INTERFACE $<$<COMPILE_LANGUAGE:CXX>:-Wno-register>)

set(CMSISCORE ${CMAKE_CURRENT_LIST_DIR}/../lib/cmsis)

add_subdirectory(${FWDIR}/src/module)

#
# Main App
#
add_executable(
  main.elf
  ${FWDIR}/system/libc_stub.c
  ${FWDIR}/system/libcpp_stub.cc
  ${FWDIR}/system/libc_init_array.c
  ${FWDIR}/system/memcpy.c
  ${FWDIR}/system/memset.c
  ${FWDIR}/system/memmove.c

  ${FWDIR}/src/console/write.cc
  ${FWDIR}/system/mmu_ca7.c
  ${FWDIR}/system/startup_ca7.s
  ${FWDIR}/src/core_a7/main.cc
  ${FWDIR}/src/memory/static_buffers.cc
  ${FWDIR}/src/console/uart_log.cc

  ${FWDIR}/src/audio/audio.cc
  ${FWDIR}/src/controls/controls.cc
  #
  ${FWDIR}/src/flash/flash_loader.cc
  #
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_adc.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_adc_ex.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_rcc.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_rcc_ex.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_mdma.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_uart.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_uart_ex.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_dma.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_i2c.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_i2c_ex.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_sai.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_hal_qspi.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_ll_tim.c
  ${FWDIR}/lib/stm32-hal/stm32mp15/src/stm32mp1xx_ll_rcc.c
  #
  ${FWDIR}/lib/mdrivlib/drivers/pin.cc
  ${FWDIR}/lib/mdrivlib/drivers/tim.cc
  ${FWDIR}/lib/mdrivlib/drivers/timekeeper.cc
  ${FWDIR}/lib/mdrivlib/drivers/i2c.cc
  ${FWDIR}/lib/mdrivlib/drivers/codec_PCM3060.cc

  ${FWDIR}/lib/mdrivlib/target/stm32mp1/drivers/sai_tdm.cc
  ${FWDIR}/lib/mdrivlib/target/stm32mp1_ca7/boot/system_ca7.c
  ${FWDIR}/lib/mdrivlib/target/stm32mp1_ca7/boot/irq_ctrl.c
  ${FWDIR}/lib/mdrivlib/target/stm32mp1_ca7/drivers/interrupt_handler.cc
  ${FWDIR}/lib/mdrivlib/target/stm32mp1_ca7/drivers/hal_handlers.cc
  ${FWDIR}/lib/mdrivlib/target/stm32mp1_ca7/drivers/cycle_counter.cc
  ${FWDIR}/lib/mdrivlib/drivers/qspi_flash_driver.cc
  ${QSPI_DRIVER}
  #
  ${CMAKE_BINARY_DIR}/checkgit/git_version.cpp
)


target_include_directories(
  main.elf
  PRIVATE ${FWDIR}
          ${FWDIR}/src
          ${FWDIR}/src/console
          ${FWDIR}/src/params
          ${FWDIR}/src/core_a7
          ${FWDIR}/src/board
          ${FWDIR}/lib/mdrivlib
          ${FWDIR}/lib/mdrivlib/target/stm32mp1
          ${FWDIR}/lib/mdrivlib/target/stm32mp1_ca7
          ${LIBDIR}
          ${FWDIR}/lib/cmsis-device/stm32mp157c/include
          #
          ${CMAKE_BINARY_DIR}/checkgit
)

add_subdirectory(${LIBDIR}/cpputil cpputil)

target_link_libraries(
  main.elf
  PRIVATE mdrivlib_interface
          cpputil
          module
)



target_link_script(
  main
  ${FWDIR}/system/linker/stm32mp15xx_ca7.ld
  -L${FWDIR}
  -Wl,--gc-sections
  -Wno-psabi
  -nostartfiles
)
