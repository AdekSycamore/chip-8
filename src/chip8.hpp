#pragma once
#include <array>
#include <cstdint>

class Chip8
{
private:
    std::array<uint8_t, 16> registers{};
    uint16_t index{};
    uint16_t pc{};

    std::array<uint16_t, 16> stack{};
    uint8_t sp{};

    std::array<uint8_t, 4096> memory{};
    std::array<uint8_t, 16> keypad{};
    std::array<uint32_t, 64 * 32> display{};

    uint8_t delayTimer{};
    uint8_t soundTimer{};

public:
    Chip8();
    ~Chip8();

    void LoadROM(const char* filename);
    void Cycle();
};