macro(set_arch_flags)
add_compile_definitions(
      USE_HAL_DRIVER
      USE_FULL_LL_DRIVER
      STM32MP13
      STM32MP135Dxx
      CORE_CA7
)

# Note: MCU_FLAGS is used with linking as well
set(MCU_FLAGS
    -fno-exceptions
    -fno-math-errno
    -mcpu=cortex-a7
    -mlittle-endian
    -mfpu=neon-vfpv4
    -mfloat-abi=hard
    -mthumb-interwork
    -mno-unaligned-access
    -mtune=cortex-a7
    -mvectorize-with-neon-quad
    -funsafe-math-optimizations
)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(
  ${MCU_FLAGS}
  -nostdlib
  "SHELL:--param l1-cache-size=32"
  "SHELL:--param l1-cache-line-size=64"
  "SHELL:--param l2-cache-size=128"
  "$<$<CONFIG:Debug>:-O0>"
  "$<$<CONFIG:Debug>:-g3>"
  "$<$<CONFIG:Release>:-Os>"
  "$<$<CONFIG:RelWithDebInfo>:-Os>"
  "$<$<CONFIG:RelWithDebInfo>:-g3>"
  -fno-common
  -fdata-sections
  -ffunction-sections
  -nostartfiles
  -fno-unwind-tables
  -Wall
  -Werror=return-type
  -Wsign-compare
  -Wno-psabi
  "$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>"
  "$<$<COMPILE_LANGUAGE:CXX>:-ffold-simple-inlines>"
  "$<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>"
)

set(LINK_OPTIONS
    -nostdlib
    -fno-rtti
)

add_link_options(${MCU_FLAGS} ${LINK_OPTIONS})
endmacro()
