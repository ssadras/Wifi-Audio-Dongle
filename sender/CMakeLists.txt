cmake_minimum_required(VERSION 3.28)
project(WIFI_Audio_Dongle)

set(CMAKE_CXX_STANDARD 17)

add_executable(WIFI_Audio_Dongle
        main.cpp
        AudioSessionManager.cpp
        AudioSessionManager.h
        AudioCapture.cpp
        AudioCapture.h
)
