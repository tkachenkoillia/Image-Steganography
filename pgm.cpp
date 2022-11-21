#include "pgm.hpp"

#include <iostream>
#include <fstream>

#include <sys/stat.h>
#include <bitset>
#include <ctime>

/** constructor and destructor of a pgm image **/
pgm::pgm::pgm(){}
pgm::pgm::~pgm() {}

/** getting image info **/
auto pgm::pgm::check_pgm_info(std::string path) -> void {
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

/** check if file is pgm **/
auto pgm::pgm::check_pgm(std::string path) -> bool {
    char line [100];

    auto file = std::fstream();
    file.open(path, std::ios::in | std::ios::binary);

    file.getline(line,100,'\n');

    if (line[0] == 'P' && line[1] == '5'){
        file.close();
        return true;
    }
    file.close();
    return false;
}

/** check if message fits in file **/
auto pgm::pgm::check_if_fits(std::string path, std::string message) -> bool {
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

    if((_width * _height) / 8 > _message_length + 1){
        file.close();
        return true;
    }
    file.close();
    return false;
}

/** read pgm file **/
auto pgm::pgm::read_pgm(std::string path) -> void {
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
            unsigned char char_color[1];
            file.read(reinterpret_cast<char*>(char_color), 1);
            set_pixel_val(x, y, (int)(char_color[0]));
        }
    }

    file.close();

}

/** write pgm file **/
auto pgm::pgm::write_pgm(std::string path) -> void {

    auto file = std::fstream();
    file.open(path, std::ios::out | std::ios::binary);

    file << "P5\n";
    file << _width << " " << _height << "\n";
    file << _max_value << '\n';
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            auto val = static_cast<unsigned char>(get_pixel_val(x, y));
            file << val;
        }
    }

}

/** getting pixel value from a given coordinates **/
auto pgm::pgm::get_pixel_val(int x, int y) -> int {
    return _colors[y * _width + x];
}

/** setting pixel value in a given coordinates **/
auto pgm::pgm::set_pixel_val(int x, int y, int value) -> void{
    _colors[y * _width + x] = value;
}

/** encrypting image **/
auto pgm::pgm::encrypt(std::string message) -> void{
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
            if(get_pixel_val(x, y) % 2 == 0){
                if (char_bit[pos][bit_num] % 2 == 1)
                    set_pixel_val(x, y, get_pixel_val(x, y) + 1);
            }
            else if(get_pixel_val(x, y) % 2 == 1){
                if (char_bit[pos][bit_num] % 2 == 0)
                    set_pixel_val(x, y, get_pixel_val(x, y) + 1);
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
auto pgm::pgm::get_message() -> void{
    std::string message;
    auto ch = '\0';
    auto bit_num = 0;
    std::bitset<8> char_bit;

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if(get_pixel_val(x, y) % 2 == 0){
                char_bit.set(bit_num, 0);
            }
            else if(get_pixel_val(x, y) % 2 == 1){
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

