#include "ppm.hpp"

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <ctime>
#include <bitset>

/** color constructor and destructor **/
ppm::color::color() : _r(0), _g(0), _b(0){}
ppm::color::color(int r, int g, int b) : _r(r), _g(g), _b(b){}
ppm::color::~color() {}

/** constructor and destructor of a ppm image **/
ppm::ppm::ppm() {}
ppm::ppm::~ppm() {}

/** getting image info **/
auto ppm::ppm::check_ppm_info(std::string path) -> void {
    struct stat file_info;
    char line [100], *pointer;

    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.getline(line,100,'\n');
    file.getline(line, 100, '\n');

    while (line[0] == '\n' || line[0] == '#'){
        file.getline(line, 100, '\n');
    }

    _width = strtol(line, &pointer, 0);
    _height = strtol(pointer, nullptr, 0);

    if(stat(path.c_str(), &file_info)==0){
        std::cout << "Path: " << path << "\n";
        std::cout << "Size: " << file_info.st_size << " bytes\n";
        std::cout << "Dimensions: " << _width << "x" << _height << " pixels\n";
        std::cout << "Last Modification: " << std::ctime(&file_info.st_mtime);
    }

    file.close();
}

/** check if file is ppm **/
auto ppm::ppm::check_ppm(std::string path) -> bool {
    char line [100], *pointer;

    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.getline(line,100,'\n');

    if (line[0] == 'P' && line[1] == '6'){
        file.close();
        return true;
    }
    file.close();
    return false;
}

/** check if message fits in file **/
auto ppm::ppm::check_if_fits(std::string path, std::string message) -> bool {
    int _message_length = message.length();

    char line [100], *pointer;

    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.getline(line,100,'\n');
    file.getline(line, 100, '\n');

    while (line[0] == '\n' || line[0] == '#'){
        file.getline(line, 100, '\n');
    }

    _width = strtol(line, &pointer, 0);
    _height = strtol(pointer, nullptr, 0);

    if((_width * _height * 3) / 8 > _message_length + 1){
        file.close();
        return true;
    }
    file.close();
    return false;
}

/** read ppm file **/
auto ppm::ppm::read_ppm(std::string path) -> void {
    char line [100], *pointer;

    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.getline(line,100,'\n');
    file.getline(line, 100, '\n');

    while (line[0] == '\n' || line[0] == '#'){
        file.getline(line, 100, '\n');
    }

    _width = strtol(line, &pointer, 0);
    _height = strtol(pointer, nullptr, 0);


    file.getline(line, 100, '\n');

    while (line[0] == '\n' || line[0] == '#'){
        file.getline(line, 100, '\n');
    }

    _max_value = strtol(line, nullptr, 0);


    _colors.resize(_width * _height);

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            unsigned char char_color[3];
            file.read(reinterpret_cast<char*>(char_color), 3);

            color color(static_cast<int>(char_color[0]),
                        static_cast<int>(char_color[1]),
                        static_cast<int>(char_color[2]));

            set_pixel_val(x, y, color);
        }
    }

    file.close();

}

/** write ppm file **/
auto ppm::ppm::write_ppm(std::string path) -> void {
    auto file = std::fstream();
    file.open(path, std::ios::out | std::ios::binary);

    file << "P6\n";
    file << _width << " " << _height << "\n";
    file << _max_value << '\n';
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            unsigned char r = static_cast<unsigned char>(get_pixel_val(x, y)._r);
            unsigned char g = static_cast<unsigned char>(get_pixel_val(x, y)._g);
            unsigned char b = static_cast<unsigned char>(get_pixel_val(x, y)._b);

            unsigned char color[] = {r, g, b};

            file.write(reinterpret_cast<char*>(color), 3);
        }
    }
}

/** getting pixel value from a given coordinates **/
auto ppm::ppm::get_pixel_val(int x, int y) -> color {
    return _colors[y * _width + x];
}

/** setting pixel value in a given coordinates **/
auto ppm::ppm::set_pixel_val(int x, int y, const color &color) -> void {
    _colors[y * _width + x]._r = color._r;
    _colors[y * _width + x]._g = color._g;
    _colors[y * _width + x]._b = color._b;
}

/** encrypting image **/
auto ppm::ppm::encrypt(std::string message) -> void {
    message.push_back('\0');
    auto message_length = message.length();
    auto ch = '\0';
    int char_bit[message_length][8];
    for (int i = 0; i < message_length; ++i) {
        ch = message.at(i);
        for (int j = 0; j < 8; ++j) {
            char_bit[i][j] = std::bitset<8>(ch)[j];
        }
    }

    auto pos = 0;
    auto bit_num = 0;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if(get_pixel_val(x, y)._r % 2 == 0){
                if (char_bit[pos][bit_num] % 2 == 1){
                    color color(static_cast<int>(get_pixel_val(x, y)._r +1),
                                static_cast<int>(get_pixel_val(x, y)._g),
                                static_cast<int>(get_pixel_val(x, y)._b));
                    set_pixel_val(x, y, color);
                }
            }
            else if(get_pixel_val(x, y)._r % 2 == 1){
                if (char_bit[pos][bit_num] % 2 == 0) {
                    color color(static_cast<int>(get_pixel_val(x, y)._r + 1),
                                static_cast<int>(get_pixel_val(x, y)._g),
                                static_cast<int>(get_pixel_val(x, y)._b));
                    set_pixel_val(x, y, color);
                }
            }
            bit_num++;
            if (bit_num >= 8){
                bit_num = 0;
                pos++;
            }
            if (pos >= message_length){
                return;
            }

            if(get_pixel_val(x, y)._g % 2 == 0){
                if (char_bit[pos][bit_num] % 2 == 1){
                    color color(static_cast<int>(get_pixel_val(x, y)._r),
                                static_cast<int>(get_pixel_val(x, y)._g + 1),
                                static_cast<int>(get_pixel_val(x, y)._b));
                    set_pixel_val(x, y, color);
                }
            }
            else if(get_pixel_val(x, y)._g % 2 == 1){
                if (char_bit[pos][bit_num] % 2 == 0) {
                    color color(static_cast<int>(get_pixel_val(x, y)._r),
                                static_cast<int>(get_pixel_val(x, y)._g + 1),
                                static_cast<int>(get_pixel_val(x, y)._b));
                    set_pixel_val(x, y, color);
                }
            }
            bit_num++;
            if (bit_num >= 8){
                bit_num = 0;
                pos++;
            }
            if (pos >= message_length){
                return;
            }

            if(get_pixel_val(x, y)._b % 2 == 0){
                if (char_bit[pos][bit_num] % 2 == 1){
                    color color(static_cast<int>(get_pixel_val(x, y)._r),
                                static_cast<int>(get_pixel_val(x, y)._g),
                                static_cast<int>(get_pixel_val(x, y)._b + 1));
                    set_pixel_val(x, y, color);
                }
            }
            else if(get_pixel_val(x, y)._b % 2 == 1){
                if (char_bit[pos][bit_num] % 2 == 0) {
                    color color(static_cast<int>(get_pixel_val(x, y)._r),
                                static_cast<int>(get_pixel_val(x, y)._g),
                                static_cast<int>(get_pixel_val(x, y)._b + 1));
                    set_pixel_val(x, y, color);
                }
            }
            bit_num++;
            if (bit_num >= 8){
                bit_num = 0;
                pos++;
            }
            if (pos >= message_length){
                return;
            }
        }

    }
}

/** decrypting image **/
auto ppm::ppm::get_message() -> void {
    std::string message;
    auto ch = '\0';
    auto bit_num = 0;
    std::bitset<8> char_bit;

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if(get_pixel_val(x, y)._r % 2 == 0){
                char_bit.set(bit_num, 0);
            }
            else if(get_pixel_val(x, y)._r % 2 == 1){
                char_bit.set(bit_num, 1);
            }
            bit_num++;
            if (bit_num >= 8){
                bit_num = 0;
                ch = char(char_bit.to_ulong());
                if (ch == '\0')
                    return;
                else
                    std::cout << ch;
            }

            if(get_pixel_val(x, y)._g % 2 == 0){
                char_bit.set(bit_num, 0);
            }
            else if(get_pixel_val(x, y)._g % 2 == 1){
                char_bit.set(bit_num, 1);
            }
            bit_num++;
            if (bit_num >= 8){
                bit_num = 0;
                ch = char(char_bit.to_ulong());
                if (ch == '\0')
                    return;
                else
                    std::cout << ch;
            }

            if(get_pixel_val(x, y)._b % 2 == 0){
                char_bit.set(bit_num, 0);
            }
            else if(get_pixel_val(x, y)._b % 2 == 1){
                char_bit.set(bit_num, 1);
            }
            bit_num++;
            if (bit_num >= 8){
                bit_num = 0;
                ch = char(char_bit.to_ulong());
                if (ch == '\0')
                    return;
                else
                    std::cout << ch;
            }
        }
    }
}
