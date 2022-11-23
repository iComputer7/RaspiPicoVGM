cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_RELEASE "-Os")

pico_sdk_init()

add_subdirectory(../no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

add_executable(pico_opl3_player
  main.cpp
)

target_include_directories(pico_opl3_player PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(pico_opl3_player 
  pico_stdlib 
  hardware_pwm
  #hardware_pio
  pico_multicore
  hardware_spi
  FatFs_SPI
)

#pico_generate_pio_header(pico_opl3_player ${CMAKE_CURRENT_LIST_DIR}/song_tick.pio)

pico_enable_stdio_usb(pico_opl3_player 1)
pico_enable_stdio_uart(pico_opl3_player 0)

pico_add_extra_outputs(pico_opl3_player)