#include "chip8.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM>\n";
        return 1;
    }

    const char* filename = argv[1];

    Chip8 chip8;

    chip8.LoadROM(filename);

    return 0;
}