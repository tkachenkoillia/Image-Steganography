#pragma once

#include <vector>
#include <string>

namespace ppm{
    struct color {
        int _r, _g, _b;

        color();
        color(int r, int g, int b);
        ~color();
    };

    struct ppm {

        int _height;
        int _width;
        int _max_value;

        std::vector<color> _colors;

        ppm();
        ~ppm();

        auto check_ppm_info(std::string path) -> void;
        auto check_ppm(std::string path) -> bool;
        auto check_if_fits(std::string path, std::string message) -> bool;

        auto read_ppm(std::string path) -> void;
        auto write_ppm(std::string path) -> void;

        auto get_pixel_val(int x, int y) -> color;
        auto set_pixel_val(int x, int y, const color & color) -> void;

        auto encrypt(std::string message) -> void;
        auto get_message() -> void;

    };
}
