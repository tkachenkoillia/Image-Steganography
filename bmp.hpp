#pragma once

#include <vector>
#include <string>

namespace bmp{
    struct color {
        int _r, _g, _b;

        color();
        color(int r, int g, int b);
        ~color();
    };

    struct bmp{

        const int file_header_size = 14;
        const int info_header_size = 40;

        unsigned char _file_header[14];
        unsigned char _info_header[40];

        int _file_size;
        int _compression_method;
        int _height;
        int _width;

        std::vector<color> _colors;

        bmp();
        ~bmp();

        auto check_bmp_info(std::string path) -> void;
        auto check_bmp(std::string path) -> bool;
        auto check_if_fits(std::string path, std::string message) -> bool;

        auto read_bmp(std::string path) -> void;
        auto write_bmp(std::string path) -> void;

        auto get_pixel_val(int x, int y) -> color;
        auto set_pixel_val(int x, int y, const color & color) -> void;

        auto encrypt(std::string message) -> void;
        auto get_message() -> void;

    };
}