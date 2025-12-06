cmake_minimum_required(VERSION 3.15)

project(lean_led C ASM)

# CPU + FPU flags
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Compiler flags
set(CMAKE_C_FLAGS "${CPU_FLAGS} -O2 -ffunction-sections -fdata-sections -Wall")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -Wl,--gc-sections -Wl,-Map=lean_led.map")


# Paths
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR      ${PROJECT_ROOT}/sources)
set(HAL_DIR      ${PROJECT_ROOT}/external/STM32F4xx_HAL_Driver)
set(CMSIS_DIR    ${PROJECT_ROOT}/external/CMSIS)

# Project sources
file(GLOB PROJECT_SOURCES
    ${SRC_DIR}/*.c
)

# HAL sources
file(GLOB HAL_SOURCES
    ${HAL_DIR}/Src/*.c
)

# Startup file 
set(STARTUP_FILE ${SRC_DIR}/startup_stm32f411vehx.s)

# CMSIS system file (defines SystemInit() and SystemCoreClock)
set(SYSTEM_FILE ${CMSIS_DIR}/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c)

add_executable(${PROJECT_NAME}.elf
    ${PROJECT_SOURCES}
    ${HAL_SOURCES}
    ${STARTUP_FILE}
    ${SYSTEM_FILE}
)

# Include directories
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${SRC_DIR}
    ${HAL_DIR}/Inc
    ${CMSIS_DIR}/Include
    ${CMSIS_DIR}/Device/ST/STM32F4xx/Include
)

# HAL requires these preprocessor flags
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32F411xE
    USE_HAL_DRIVER
)

# Linker script
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${SRC_DIR}/STM32F411VEHX_FLASH.ld
)

# Binary output
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Generating binary"
)
