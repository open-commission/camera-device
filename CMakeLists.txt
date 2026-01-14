cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

# Set the project name
set(CMAKE_PROJECT_NAME stellaris-c-luxinyu)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Core/Src/ttp229.c
        Core/Src/DebugUart/uart2.c
        Core/Src/NetUart/uart1.c
        Core/Src/RFIDUart/uart3.c
        Core/Src/CJSON/cJSON.c
        Core/Src/HX711/hx711.c
        Core/Src/Delay/delay.c
        Core/Src/Sen/0121.c
        Core/Src/Dht11/11.c
        Core/Src/GlobalVar/vars.c
        Core/Src/INIT/start.c
        Core/Src/SG/90.c
        Core/Src/IOCTL/ctl.c
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user sources here
        Core/Src/DebugUart/uart2.c
        Core/Src/NetUart/uart1.c
        Core/Src/RFIDUart/uart3.c
        Core/Src/CJSON/cJSON.c
        Core/Src/HX711/hx711.c
        Core/Src/Delay/delay.c
        Core/Src/Sen/0121.c
        Core/Src/Dht11/11.c
        Core/Src/GlobalVar/vars.c
        Core/Src/INIT/start.c
        Core/Src/SG/90.c
        Core/Src/IOCTL/ctl.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined include paths
        Core/Src/DebugUart
        Core/Src/NetUart
        Core/Src/RFIDUart
        Core/Src/CJSON
        Core/Src/HX711
        Core/Src/Delay
        Core/Src/Sen
        Core/Src/Dht11
        Core/Src/GlobalVar
        Core/Src/INIT
        Core/Src/SG
        Core/Src/IOCTL
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx

        # Add user defined libraries
)