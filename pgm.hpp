#pragma once

#include <vector>
#include <string>

namespace pgm{
    struct pgm {
        int _height;
        int _width;
        int _max_value;

        std::vector<int> _colors;

        pgm();
        ~pgm();

        auto check_pgm_info(std::string path) -> void;
        auto check_pgm(std::string path) -> bool;
        auto check_if_fits(std::string path, std::string message) -> bool;

        auto read_pgm(std::string path) -> void;
        auto write_pgm(std::string path) -> void;

        auto get_pixel_val(int x, int y) -> int;
        auto set_pixel_val(int x, int y, int value) -> void;

        auto encrypt(std::string message) -> void;
        auto get_message() -> void;

    };
}
