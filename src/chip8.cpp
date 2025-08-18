#include "chip8.hpp"
#include <random>
#include <fstream>
#include <iostream>
#include <stdexcept>

constexpr unsigned int FONT_SIZE = 80;
constexpr unsigned int FONT_START_ADDRESS = 0x50;
constexpr unsigned int START_ADDRESS = 0x200;

constexpr std::array<uint8_t, FONT_SIZE> FONTSET = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{
    pc = START_ADDRESS;

    std::copy(FONTSET.begin(), FONTSET.end(), memory.begin() + FONT_START_ADDRESS);
}

bool Chip8::loadROM(const char* filename)
{
    try
    {  
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::streamsize size = file.tellg();
        if (size > static_cast<std::streamsize>(memory.size() - START_ADDRESS)) {
            throw std::runtime_error("ROM is too big for CHIP-8 memory");
        }

        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        file.read(buffer.data(), size);

        std::copy(buffer.begin(), buffer.end(), memory.begin() + START_ADDRESS);

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR while loading ROM: " << e.what() << std::endl;
        return false;
    }
}