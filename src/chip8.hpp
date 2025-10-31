#pragma once
#include "display.hpp"
#include <array>
#include <cstdint>

struct Instruction
{
    uint16_t nnn; // 12-bitowy adres
    uint8_t n;    // ostatni nibble
    uint8_t x;    // nibble X
    uint8_t y;    // nibble Y
    uint8_t kk;   // 8-bitowa stała
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

    uint8_t delayTimer{};
    uint8_t soundTimer{};

    using OpcodeHandler = void (Chip8::*)();

    std::array<OpcodeHandler, 16> table{};
    std::array<OpcodeHandler, 16> table0{};
    std::array<OpcodeHandler, 16> table8{};
    std::array<OpcodeHandler, 16> tableE{};
    std::array<OpcodeHandler, 256> tableF{};

    uint16_t opcode{};

    Instruction decodeOpcode() const;
    uint8_t randomByte();
    void initTables();

    void Table0();
    void Table8();
    void TableE();
    void TableF();

    void OP_00E0(); // Wyczyść ekran
    void OP_00EE(); // Powrót z podprogramu

    // 0x1***
    void OP_1nnn(); // Skok do adresu NNN

    // 0x2***
    void OP_2nnn(); // Wywołaj podprogram pod adresem NNN

    // 0x3***
    void OP_3xkk(); // Pomiń następną instrukcję jeśli Vx == kk

    // 0x4***
    void OP_4xkk(); // Pomiń następną instrukcję jeśli Vx != kk

    // 0x5***
    void OP_5xy0(); // Pomiń następną instrukcję jeśli Vx == Vy

    // 0x6***
    void OP_6xkk(); // Ustaw Vx = kk

    // 0x7***
    void OP_7xkk(); // Ustaw Vx = Vx + kk

    // 0x8***
    void OP_8xy0(); // Ustaw Vx = Vy
    void OP_8xy1(); // Ustaw Vx = Vx OR Vy
    void OP_8xy2(); // Ustaw Vx = Vx AND Vy
    void OP_8xy3(); // Ustaw Vx = Vx XOR Vy
    void OP_8xy4(); // Ustaw Vx = Vx + Vy, VF = carry
    void OP_8xy5(); // Ustaw Vx = Vx - Vy, VF = NOT borrow
    void OP_8xy6(); // Ustaw Vx = Vx SHR 1
    void OP_8xy7(); // Ustaw Vx = Vy - Vx, VF = NOT borrow
    void OP_8xyE(); // Ustaw Vx = Vx SHL 1

    // 0x9***
    void OP_9xy0(); // Pomiń następną instrukcję jeśli Vx != Vy

    // 0xA***
    void OP_Annn(); // Ustaw I = nnn

    // 0xB***
    void OP_Bnnn(); // Skok do lokacji nnn + V0

    // 0xC***
    void OP_Cxkk(); // Ustaw Vx = losowy bajt AND kk

    // 0xD***
    void OP_Dxyn(); // Rysuj sprite w (Vx, Vy) o wysokości n

    // 0xE***
    void OP_Ex9E(); // Pomiń następną instrukcję jeśli klawisz o wartości Vx jest wciśnięty
    void OP_ExA1(); // Pomiń następną instrukcję jeśli klawisz o wartości Vx nie jest wciśnięty

    // 0xF***
    void OP_Fx07(); // Ustaw Vx = wartość timera opóźnienia
    void OP_Fx0A(); // Czekaj na wciśnięcie klawisza, zapisz w Vx
    void OP_Fx15(); // Ustaw timer opóźnienia = Vx
    void OP_Fx18(); // Ustaw timer dźwięku = Vx
    void OP_Fx1E(); // Ustaw I = I + Vx
    void OP_Fx29(); // Ustaw I = lokacja sprite'a dla cyfry Vx
    void OP_Fx33(); // Zapisz BCD z Vx w pamięci pod adresami I, I+1, I+2
    void OP_Fx55(); // Zapisz rejestry V0 do Vx w pamięci zaczynając od I
    void OP_Fx65(); // Wczytaj rejestry V0 do Vx z pamięci zaczynając od I

    void OP_NULL();

public:
std::array<uint8_t, 16> keypad{}; 
    Display display;
    Chip8();
    ~Chip8() = default;

    void initialize();
    bool loadROM(const char *filename);
    void cycle();
    void updateTimers();
    void render();
    void updateKeypad(int ch);
    void resetKeypad();
};