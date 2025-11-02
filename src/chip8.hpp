#pragma once
#include "display.hpp"
#include <array>
#include <cstdint>

struct Instruction
{
    uint16_t nnn; // 12-bit address
    uint8_t n;    // last nibble
    uint8_t x;    // nibble X
    uint8_t y;    // nibble Y
    uint8_t kk;   // 8-bit constant
};

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

    uint8_t delayTimer{};
    uint8_t soundTimer{};

    using OpcodeHandler = void (Chip8::*)();

    std::array<OpcodeHandler, 16> table{};
    std::array<OpcodeHandler, 256> table0{};
    std::array<OpcodeHandler, 16> table8{};
    std::array<OpcodeHandler, 256> tableE{};
    std::array<OpcodeHandler, 256> tableF{};

    uint16_t opcode{};

    Instruction decodeOpcode() const;
    uint8_t randomByte();
    void initTables();

    void Table0();
    void Table8();
    void TableE();
    void TableF();

    void OP_00E0(); // Clear screen
    void OP_00EE(); // Return from subroutine

    // 0x1***
    void OP_1nnn(); // Jump to address NNN

    // 0x2***
    void OP_2nnn(); // Call subroutine at NNN

    // 0x3***
    void OP_3xkk(); // Skip next instruction if Vx == kk

    // 0x4***
    void OP_4xkk(); // Skip next instruction if Vx != kk

    // 0x5***
    void OP_5xy0(); // Skip next instruction if Vx == Vy

    // 0x6***
    void OP_6xkk(); // Set Vx = kk

    // 0x7***
    void OP_7xkk(); // Set Vx = Vx + kk

    // 0x8***
    void OP_8xy0(); // Set Vx = Vy
    void OP_8xy1(); // Set Vx = Vx OR Vy
    void OP_8xy2(); // Set Vx = Vx AND Vy
    void OP_8xy3(); // Set Vx = Vx XOR Vy
    void OP_8xy4(); // Set Vx = Vx + Vy, VF = carry
    void OP_8xy5(); // Set Vx = Vx - Vy, VF = NOT borrow
    void OP_8xy6(); // Set Vx = Vx SHR 1
    void OP_8xy7(); // Set Vx = Vy - Vx, VF = NOT borrow
    void OP_8xyE(); // Set Vx = Vx SHL 1

    // 0x9***
    void OP_9xy0(); // Skip next instruction if Vx != Vy

    // 0xA***
    void OP_Annn(); // Set I = nnn

    // 0xB***
    void OP_Bnnn(); // Jump to location nnn + V0

    // 0xC***
    void OP_Cxkk(); // Set Vx = random byte AND kk

    // 0xD***
    void OP_Dxyn(); // Draw sprite at (Vx, Vy) with height n

    // 0xE***
    void OP_Ex9E(); // Skip next instruction if key with value Vx is pressed
    void OP_ExA1(); // Skip next instruction if key with value Vx is not pressed

    // 0xF***
    void OP_Fx07(); // Set Vx = delay timer value
    void OP_Fx0A(); // Wait for key press, store in Vx
    void OP_Fx15(); // Set delay timer = Vx
    void OP_Fx18(); // Set sound timer = Vx
    void OP_Fx1E(); // Set I = I + Vx
    void OP_Fx29(); // Set I = location of sprite for digit Vx
    void OP_Fx33(); // Store BCD of Vx in memory locations I, I+1, I+2
    void OP_Fx55(); // Store registers V0-Vx in memory starting at I
    void OP_Fx65(); // Load registers V0-Vx from memory starting at I

    void OP_NULL();

public:
    Display display;
    Chip8();
    ~Chip8() = default;

    void initialize();
    bool loadROM(const char *filename);
    void cycle();
    void updateTimers(); 
    void updateKeypad(int ch);
    void resetKeypad();
};