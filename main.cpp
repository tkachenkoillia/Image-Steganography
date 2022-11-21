#include <iostream>
#include "pgm.hpp"
#include "ppm.hpp"
#include "bmp.hpp"

auto main(int argc, char* argv[]) -> int {

    if(argc == 1){
        std::cout <<
                  "************************************************************\n"
                  "*         IMAGE ENCRYPTION AND DECRYPTION PROGRAM          *\n"
                  "*                                                          *\n"
                  "* Welcome to image encryption and decryption program.      *\n"
                  "* This program changes binary data of image pixels to hide *\n"
                  "* your message in it without any visual changes for human  *\n"
                  "* eye. It is a manual that shows all supported flags and   *\n"
                  "* file formats.                                            *\n"
                  "*                                                          *\n"
                  "* Here are all supported flags:                            *\n"
                  "*                                                          *\n"
                  "* -i (--info) + path to the file:                          *\n"
                  "*      checks if file format is supported by the program,  *\n"
                  "*      its dimensions, occupied space on disc              *\n"
                  "*      and timestamp of its last modification              *\n"
                  "*                                                          *\n"
                  "* -e (--encrypt) + path to the file + \"message\":           *\n"
                  "*      encrypts a message in quotes inside the file        *\n"
                  "*                                                          *\n"
                  "* -d (--decrypt) + path to the file:                       *\n"
                  "*      decrypts a message from the file                    *\n"
                  "*                                                          *\n"
                  "* -c (--check) + path to the file + \"message\":             *\n"
                  "*      checks if message in quotes can fit into the file   *\n"
                  "*                                                          *\n"
                  "* -h (--help):                                             *\n"
                  "*      print the manual of the program                     *\n"
                  "*                                                          *\n"
                  "* Here are all supported file formats:                     *\n"
                  "*                                                          *\n"
                  "*  bmp(bi_rgb), ppm, pgm                                   *\n"
                  "*                                                          *\n"
                  "************************************************************\n";
    }

    else if (argc == 2){
        std::string flag = argv[1];
        if (flag == "-h" || flag == "--help"){
            std::cout <<
            "************************************************************\n"
            "*         IMAGE ENCRYPTION AND DECRYPTION PROGRAM          *\n"
            "*                                                          *\n"
            "* Welcome to image encryption and decryption program.      *\n"
            "* This program changes binary data of image pixels to hide *\n"
            "* your message in it without any visual changes for human  *\n"
            "* eye. It is a manual that shows all supported flags and   *\n"
            "* file formats.                                            *\n"
            "*                                                          *\n"
            "* Here are all supported flags:                            *\n"
            "*                                                          *\n"
            "* -i (--info) + path to the file:                          *\n"
            "*      checks if file format is supported by the program,  *\n"
            "*      its dimensions, occupied space on disc              *\n"
            "*      and timestamp of its last modification              *\n"
            "*                                                          *\n"
            "* -e (--encrypt) + path to the file + \"message\":           *\n"
            "*      encrypts a message in quotes inside the file        *\n"
            "*                                                          *\n"
            "* -d (--decrypt) + path to the file:                       *\n"
            "*      decrypts a message from the file                    *\n"
            "*                                                          *\n"
            "* -c (--check) + path to the file + \"message\":             *\n"
            "*      checks if message in quotes can fit into the file   *\n"
            "*                                                          *\n"
            "* -h (--help):                                             *\n"
            "*      print the manual of the program                     *\n"
            "*                                                          *\n"
            "* Here are all supported file formats:                     *\n"
            "*                                                          *\n"
            "*  bmp(bi_rgb), ppm, pgm                                   *\n"
            "*                                                          *\n"
            "************************************************************\n";
        }
        else{
            std::cout << "UNKNOWN FLAG OR FLAG USED INCORRECTLY. TRY USING FLAG -h TO SEE PROGRAM DOCUMENTATION";
        }
    }

    else if (argc == 3) {
        std::string flag = argv[1];
        std::string path = argv[2];

         if (flag == "-d" || flag == "--decrypt"){
            bmp::bmp _bmp;
            pgm::pgm _pgm;
            ppm::ppm _ppm;
            if (_bmp.check_bmp(path)){
                _bmp.read_bmp(path);
                _bmp.get_message();
            }
            else if (_pgm.check_pgm(path)){
                _pgm.read_pgm(path);
                _pgm.get_message();
            }
            else if (_ppm.check_ppm(path)){
                _ppm.read_ppm(path);
                _ppm.get_message();
            }
            else{
                std::cout << "Unsupported file format. Try again with \'.bmp\', \'.ppm\' or \'.pgm\' file";
            }
        }
        else if (flag == "-i" || flag == "--info"){
            bmp::bmp _bmp;
            pgm::pgm _pgm;
            ppm::ppm _ppm;
            if (_bmp.check_bmp(path)){
                _bmp.check_bmp_info(path);
            }
            else if (_pgm.check_pgm(path)){
                _pgm.check_pgm_info(path);
            }
            else if (_ppm.check_ppm(path)){
                _ppm.check_ppm_info(path);
            }
            else{
                std::cout << "Unsupported file format. Try again with \'.bmp\', \'.ppm\' or \'.pgm\' file";
            }
        }
        else{
            std::cout << "UNKNOWN FLAG OR FLAG USED INCORRECTLY. TRY USING FLAG -h TO SEE PROGRAM DOCUMENTATION";
        }
    }

    else if (argc == 4) {
        std::string flag = argv[1];
        std::string path = argv[2];
        std::string message = argv[3];

        if (flag == "-c" || flag == "--check") {
            bmp::bmp _bmp;
            pgm::pgm _pgm;
            ppm::ppm _ppm;
            if (_bmp.check_bmp(path)) {
                if (_bmp.check_if_fits(path, message))
                    std::cout << "GIVEN MESSAGE FITS IN FILE";
                else
                    std::cout << "GIVEN MESSAGE DOESN\'T FIT IN FILE. TRY SHORTER MESSAGE OR BIGGER IMAGE";
            } else if (_pgm.check_pgm(path)) {
                if (_pgm.check_if_fits(path, message))
                    std::cout << "GIVEN MESSAGE FITS IN FILE";
                else
                    std::cout << "GIVEN MESSAGE DOESN\'T FIT IN FILE. TRY SHORTER MESSAGE OR BIGGER IMAGE";
            } else if (_ppm.check_ppm(path)) {
                if (_ppm.check_if_fits(path, message))
                    std::cout << "GIVEN MESSAGE FITS IN FILE";
                else
                    std::cout << "GIVEN MESSAGE DOESN\'T FIT IN FILE. TRY SHORTER MESSAGE OR BIGGER IMAGE";
            } else {
                std::cout << "Unsupported file format. Try again with \'.bmp\', \'.ppm\' or \'.pgm\' file";
            }

        } else if (flag == "-e" || flag == "--encrypt") {
            bmp::bmp _bmp;
            pgm::pgm _pgm;
            ppm::ppm _ppm;
            if (_bmp.check_bmp(path)) {
                if (_bmp.check_if_fits(path, message)) {
                    _bmp.read_bmp(path);
                    _bmp.encrypt(message);
                    _bmp.write_bmp(path);
                    std::cout << "MESSAGE ENCRYPTED";
                }
            } else if (_pgm.check_pgm(path)) {
                if (_pgm.check_if_fits(path, message)) {
                    _pgm.read_pgm(path);
                    _pgm.encrypt(message);
                    _pgm.write_pgm(path);
                    std::cout << "MESSAGE ENCRYPTED";
                }
            } else if (_ppm.check_ppm(path)) {
                if (_ppm.check_if_fits(path, message)) {
                    _ppm.read_ppm(path);
                    _ppm.encrypt(message);
                    _ppm.write_ppm(path);
                    std::cout << "MESSAGE ENCRYPTED";
                }
            } else {
                std::cout << "Unsupported file format. Try again with \'.bmp\', \'.ppm\' or \'.pgm\' file";
            }
        }
        else{
            std::cout << "UNKNOWN FLAG OR FLAG USED INCORRECTLY. TRY USING FLAG -h TO SEE PROGRAM DOCUMENTATION";
        }
    }

    else{
        std::cout << "UNKNOWN FLAG OR FLAG USED INCORRECTLY. TRY USING FLAG -h TO SEE PROGRAM DOCUMENTATION";
    }
}
