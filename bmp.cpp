
#include "bmp.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <bitset>
#include <ctime>

/** color constructor and destructor **/
bmp::color::color() : _r(0), _g(0), _b(0) {}
bmp::color::color(int r, int g, int b) : _r(r), _g(g), _b(b) {}
bmp::color::~color() {}

/** constructor and destructor of a bmp image **/
bmp::bmp::bmp() {}
bmp::bmp::~bmp() {}

/** getting image info **/
auto bmp::bmp::check_bmp_info(std::string path) -> void {
    struct stat file_info;

    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.read(reinterpret_cast<char*>(_file_header), file_header_size);
    file.read(reinterpret_cast<char*>(_info_header), info_header_size);

    _width = _info_header[4] + (_info_header[5] << 8) + (_info_header[6] << 16) + (_info_header[7] << 24);
    _height = _info_header[8] + (_info_header[9] << 8) + (_info_header[10] << 16) + (_info_header[11] << 24);


    if(stat(path.c_str(), &file_info)==0){
        std::cout << "Path: " << path << "\n";
        std::cout << "Size: " << file_info.st_size << " bytes\n";
        std::cout << "Dimensions: " << _width << "x" << _height << " pixels\n";
        std::cout << "Last Modification: " << std::ctime(&file_info.st_mtime);
    }

    file.close();
}

/** check if file is bmp **/
auto bmp::bmp::check_bmp(std::string path) -> bool {
    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.read(reinterpret_cast<char*>(_file_header), file_header_size);

    if (_file_header[0] == 'B' && _file_header[1] == 'M'){
        file.close();
        return true;
    }
    file.close();
    return false;
}

/** check if message fits in file **/
auto bmp::bmp::check_if_fits(std::string path, std::string message) -> bool {
    int _message_length = message.length();

    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.read(reinterpret_cast<char*>(_file_header), file_header_size);
    file.read(reinterpret_cast<char*>(_info_header), info_header_size);

    _width = _info_header[4] + (_info_header[5] << 8) + (_info_header[6] << 16) + (_info_header[7] << 24);
    _height = _info_header[8] + (_info_header[9] << 8) + (_info_header[10] << 16) + (_info_header[11] << 24);

    if((_width * _height * 3) / 8 > _message_length + 1){
        file.close();
        return true;
    }
    file.close();
    return false;
}

/** read bmp file from path**/
auto bmp::bmp::read_bmp(std::string path) -> void {
    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.read(reinterpret_cast<char*>(_file_header), file_header_size);
    file.read(reinterpret_cast<char*>(_info_header), info_header_size);

    _file_size = _file_header[2] + (_file_header[3] << 8) + (_file_header[4] << 16) + (_file_header[5] << 24);
    _width = _info_header[4] + (_info_header[5] << 8) + (_info_header[6] << 16) + (_info_header[7] << 24);
    _height = _info_header[8] + (_info_header[9] << 8) + (_info_header[10] << 16) + (_info_header[11] << 24);
    _compression_method = _info_header[16] + (_info_header[17] << 8) + (_info_header[18] << 16) + (_info_header[19] << 24);

    _colors.resize(_width * _height);


    if (_compression_method != 0){
        std::cout << "not a bi_rgb file\n";
    }

    for (auto y = 0; y < _height; y++){

        for (auto x = 0; x < _width; x++) {

            unsigned char char_color[3];
            file.read(reinterpret_cast<char*>(char_color), 3);

            color color(static_cast<int>(char_color[2]),
                        static_cast<int>(char_color[1]),
                        static_cast<int>(char_color[0]));

            set_pixel_val(x, y, color);

        }

    }

    file.close();
}

/** write bmp file to the given path**/
auto bmp::bmp::write_bmp(std::string path) -> void {
    auto file = std::fstream();
    file.open(path, std::ios::out | std::ios::binary);

    if(!file.is_open()){
        std::cout << "error\n";
        return;
    }

    /** image type **/
    _file_header[0] = 'B';
    _file_header[1] = 'M';

    /** file size **/
    _file_header[2] = _file_size;
    _file_header[3] = _file_size >> 8;
    _file_header[4] = _file_size >> 16;
    _file_header[5] = _file_size >> 24;

    /** reserved: depends on application that creates a file '00' if created manually **/
    _file_header[6] = 0;
    _file_header[7] = 0;

    /** reserved: depends on application that creates a file '00' if created manually **/
    _file_header[8] = 0;
    _file_header[9] = 0;

    /** the offset **/
    _file_header[10] = file_header_size + info_header_size;
    _file_header[11] = 0;
    _file_header[12] = 0;
    _file_header[13] = 0;

    /** size of header (must be 40) **/
    _info_header[0] = info_header_size;
    _info_header[1] = 0;
    _info_header[2] = 0;
    _info_header[3] = 0;

    /** width **/
    _info_header[4] = _width;
    _info_header[5] = _width >> 8;
    _info_header[6] = _width >> 16;
    _info_header[7] = _width >> 24;

    /** height **/
    _info_header[8] = _height;
    _info_header[9] = _height >> 8;
    _info_header[10] = _height >> 16;
    _info_header[11] = _height >> 24;

    /** number of color planes (must be 1) **/
    _info_header[12] = 1;
    _info_header[13] = 0;

    /** number of bits per pixel **/
    _info_header[14] = 24;
    _info_header[15] = 0;

    /** compression method being used (0 for bi_rgb) **/
    _info_header[16] = 0;
    _info_header[17] = 0;
    _info_header[18] = 0;
    _info_header[19] = 0;

    /** image size (can be 0 for bi_rgb) **/
    _info_header[20] = 0;
    _info_header[21] = 0;
    _info_header[22] = 0;
    _info_header[23] = 0;

    /** horizontal resolution of the image (pixel per metre) **/
    _info_header[24] = 0;
    _info_header[25] = 0;
    _info_header[26] = 0;
    _info_header[27] = 0;

    /** vertical resolution of the image (pixel per metre) **/
    _info_header[28] = 0;
    _info_header[29] = 0;
    _info_header[30] = 0;
    _info_header[31] = 0;

    /** number of colors in the color palette (0 as default) **/
    _info_header[32] = 0;
    _info_header[33] = 0;
    _info_header[34] = 0;
    _info_header[35] = 0;

    /** number of important colors used (0 as default) **/
    _info_header[36] = 0;
    _info_header[37] = 0;
    _info_header[38] = 0;
    _info_header[39] = 0;

    file.write(reinterpret_cast<char*>(_file_header), file_header_size);
    file.write(reinterpret_cast<char*>(_info_header), info_header_size);

    for (auto y = 0; y < _height; y++){
        for (auto x = 0; x < _width; x++) {
            unsigned char r = static_cast<unsigned char>(get_pixel_val(x, y)._r);
            unsigned char g = static_cast<unsigned char>(get_pixel_val(x, y)._g);
            unsigned char b = static_cast<unsigned char>(get_pixel_val(x, y)._b);

            unsigned char color[] = {b, g, r};

            file.write(reinterpret_cast<char*>(color), 3);
        }

    }

    file.close();
}

/** getting pixel value from a given coordinates **/
auto bmp::bmp::get_pixel_val(int x, int y) -> color {
    return _colors[y * _width + x];
}

/** setting pixel value in a given coordinates **/
auto bmp::bmp::set_pixel_val(int x, int y, const color &color) -> void {
    _colors[y * _width + x]._r = color._r;
    _colors[y * _width + x]._g = color._g;
    _colors[y * _width + x]._b = color._b;
}

/** encrypting given image **/
auto bmp::bmp::encrypt(std::string message) -> void {
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
auto bmp::bmp::get_message() -> void {
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


