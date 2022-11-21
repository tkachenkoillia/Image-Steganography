cmake_minimum_required(VERSION 3.21)
project(2122_PJC_eng_s24387)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageSteganography
        main.cpp
        bmp.cpp bmp.hpp
        pgm.cpp pgm.hpp
        ppm.cpp ppm.hpp)
