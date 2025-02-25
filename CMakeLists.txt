# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(LeitoInteligente C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(LeitoInteligente src/main.c src/matriz_5x5.c src/config.c
               src/ssd1306.c src/funcoes_gpio.c src/funcoes_pwm.c
               src/uart.c src/figuras_ws2812.c src/sensores.c
               src/figuras_ssd1306.c)

pico_set_program_name(LeitoInteligente "LeitoInteligente")
pico_set_program_version(LeitoInteligente "0.1")

# Define o diretório de saída apenas para os arquivos de cabeçalho
set(PIO_HEADER_OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/inc)

# Gera o arquivo PIO na pasta especificada apenas para headers
pico_generate_pio_header(LeitoInteligente 
    ${CMAKE_CURRENT_LIST_DIR}/InterfacePio.pio 
    OUTPUT_DIR ${PIO_HEADER_OUTPUT_DIR}
)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(LeitoInteligente 1)
pico_enable_stdio_usb(LeitoInteligente 1)

# Add the standard library to the build
target_link_libraries(LeitoInteligente
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(LeitoInteligente PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Add any user requested libraries
target_link_libraries(LeitoInteligente 
        hardware_spi
        hardware_i2c
        hardware_pio
        hardware_timer
        hardware_clocks
        hardware_pwm
        hardware_adc
        )

pico_add_extra_outputs(LeitoInteligente)

