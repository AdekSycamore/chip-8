#include "chip8.hpp"
#include <random>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

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
    initialize();
}

void Chip8::initialize()
{
    registers.fill(0);
    index = 0;
    pc = START_ADDRESS;

    stack.fill(0);
    sp = 0;

    memory.fill(0);
    keypad.fill(0);

    delayTimer = 0;
    soundTimer = 0;

    std::copy(FONTSET.begin(), FONTSET.end(), memory.begin() + FONT_START_ADDRESS);

    initTables();
}

void Chip8::initTables()
{
    // set all table entries to OP_NULL
    table.fill(&Chip8::OP_NULL);
    table0.fill(&Chip8::OP_NULL);
    table8.fill(&Chip8::OP_NULL);
    tableE.fill(&Chip8::OP_NULL);
    tableF.fill(&Chip8::OP_NULL);

    // main table
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    // table 0x0***
    table0[0xE0] = &Chip8::OP_00E0;
    table0[0xEE] = &Chip8::OP_00EE;

    // table 0x8***
    table8[0x0] = &Chip8::OP_8xy0;
    table8[0x1] = &Chip8::OP_8xy1;
    table8[0x2] = &Chip8::OP_8xy2;
    table8[0x3] = &Chip8::OP_8xy3;
    table8[0x4] = &Chip8::OP_8xy4;
    table8[0x5] = &Chip8::OP_8xy5;
    table8[0x6] = &Chip8::OP_8xy6;
    table8[0x7] = &Chip8::OP_8xy7;
    table8[0xE] = &Chip8::OP_8xyE;

    // table 0xE***
    tableE[0x9E] = &Chip8::OP_Ex9E;
    tableE[0xA1] = &Chip8::OP_ExA1;

    // table 0xF***
    tableF[0x07] = &Chip8::OP_Fx07;
    tableF[0x0A] = &Chip8::OP_Fx0A;
    tableF[0x15] = &Chip8::OP_Fx15;
    tableF[0x18] = &Chip8::OP_Fx18;
    tableF[0x1E] = &Chip8::OP_Fx1E;
    tableF[0x29] = &Chip8::OP_Fx29;
    tableF[0x33] = &Chip8::OP_Fx33;
    tableF[0x55] = &Chip8::OP_Fx55;
    tableF[0x65] = &Chip8::OP_Fx65;
}

void Chip8::Table0()
{
    auto opcodeId = opcode & 0x00FF;
    auto handler = table0[opcodeId];
    if (handler == &Chip8::OP_NULL) {
        std::cout << "[DEBUG] Unknown instruction 0x0***: " << std::hex << opcode << std::dec << std::endl;
    }
    (this->*handler)();
}

// Table 0x8***
void Chip8::Table8()
{
    auto opcodeId = opcode & 0x000F;
    auto handler = table8[opcodeId];
    if (handler == &Chip8::OP_NULL) {
        std::cout << "[DEBUG] Unknown instruction 0x8***: " << std::hex << opcode << std::dec << std::endl;
    }
    (this->*handler)();
}

// Table 0xE***
void Chip8::TableE()
{
    auto opcodeId = opcode & 0x00FF;
    auto handler = tableE[opcodeId];
    if (handler == &Chip8::OP_NULL) {
        std::cout << "[DEBUG] Unknown instruction 0xE***: " << std::hex << opcode << std::dec << std::endl;
    }
    (this->*handler)();
}

// Table 0xF***
void Chip8::TableF()
{
    auto opcodeId = opcode & 0x00FF;
    auto handler = tableF[opcodeId];
    if (handler == &Chip8::OP_NULL) {
        std::cout << "[DEBUG] Unknown instruction 0xF***: " << std::hex << opcode << std::dec << std::endl;
    }
    (this->*handler)();
}

bool Chip8::loadROM(const char *filename)
{
    try
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::streamsize size = file.tellg();
        if (size > static_cast<std::streamsize>(memory.size() - START_ADDRESS))
        {
            throw std::runtime_error("ROM is too big for CHIP-8 memory");
        }

        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        file.read(buffer.data(), size);

        std::copy(buffer.begin(), buffer.end(), memory.begin() + START_ADDRESS);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR while loading ROM: " << e.what() << std::endl;
        return false;
    }
}

void Chip8::cycle() {
    opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;

    uint8_t mainNibble = static_cast<uint8_t>((opcode & 0xF000) >> 12);
    auto handler = table[mainNibble];
    
    (this->*handler)();
}


void Chip8::updateKeypad(int ch)
{
    switch (ch)
    {
        case '1': keypad[0x1] = 1; break;
        case '2': keypad[0x2] = 1; break;
        case '3': keypad[0x3] = 1; break;
        case '4': keypad[0xC] = 1; break;

        case 'q': keypad[0x4] = 1; break;
        case 'w': keypad[0x5] = 1; break;
        case 'e': keypad[0x6] = 1; break;
        case 'r': keypad[0xD] = 1; break;

        case 'a': keypad[0x7] = 1; break;
        case 's': keypad[0x8] = 1; break;
        case 'd': keypad[0x9] = 1; break;
        case 'f': keypad[0xE] = 1; break;

        case 'z': keypad[0xA] = 1; break;
        case 'x': keypad[0x0] = 1; break;
        case 'c': keypad[0xB] = 1; break;
        case 'v': keypad[0xF] = 1; break;

        default:
            // unsupported key - do nothing
            break;
    }
}

void Chip8::resetKeypad() {
    
    keypad.fill(0);
}


void Chip8::updateTimers()
{
    if (delayTimer > 0)
    {
        --delayTimer;
    }

    if (soundTimer > 0)
    {
        --soundTimer;

        if (soundTimer == 0)
        {
            std::cout << "\a";
        }
    }
}

uint8_t Chip8::randomByte()
{
    static std::random_device rd; 
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint16_t> dist(0, 255);

    return static_cast<uint8_t>(dist(gen));
}

Instruction Chip8::decodeOpcode() const
{
    Instruction inst{};
    inst.nnn = opcode & 0x0FFF;
    inst.n = opcode & 0x000F;
    inst.x = (opcode & 0x0F00) >> 8;
    inst.y = (opcode & 0x00F0) >> 4;
    inst.kk = opcode & 0x00FF;
    return inst;
}

// Instructions 0x0***
void Chip8::OP_00E0()
{
    display.clear();
}

void Chip8::OP_00EE()
{
    --sp;
    pc = stack[sp];
}

// Instructions 0x1***
void Chip8::OP_1nnn()
{
    Instruction inst = decodeOpcode();
    pc = inst.nnn;
}

void Chip8::OP_2nnn()
{
    Instruction inst = decodeOpcode();
    stack[sp] = pc;
    ++sp;

    pc = inst.nnn;
}

void Chip8::OP_3xkk()
{
    Instruction inst = decodeOpcode();

    if (registers[inst.x] == inst.kk)
    {
        pc += 2;
    }
}

void Chip8::OP_4xkk()
{
    Instruction inst = decodeOpcode();

    if (registers[inst.x] != inst.kk)
    {
        pc += 2;
    }
}

void Chip8::OP_5xy0()
{
    Instruction inst = decodeOpcode();

    if (registers[inst.x] == registers[inst.y])
    {
        pc += 2;
    }
}

void Chip8::OP_6xkk()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] = inst.kk;
}

void Chip8::OP_7xkk()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] += inst.kk;
}

// Instructions 0x8***
void Chip8::OP_8xy0()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] = registers[inst.y];
}

void Chip8::OP_8xy1()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] |= registers[inst.y];
}

void Chip8::OP_8xy2()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] &= registers[inst.y];
}

void Chip8::OP_8xy3()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] ^= registers[inst.y];
}
void Chip8::OP_8xy4()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] += registers[inst.y];
}

void Chip8::OP_8xy5()
{
    Instruction inst = decodeOpcode();

    if (registers[inst.x] >= registers[inst.y])
    {
        registers[0xF] = 1; // no borrow
        registers[inst.x] -= registers[inst.y];
    }
    else
    {
        registers[0xF] = 0; // borrow
        registers[inst.x] = (uint8_t)(registers[inst.x] - registers[inst.y]);
    }
}

void Chip8::OP_8xy6()
{
    Instruction inst = decodeOpcode();
    registers[0xF] = registers[inst.x] & 0x1;

    registers[inst.x] >>= 1;
}

void Chip8::OP_8xy7()
{
    Instruction inst = decodeOpcode();
    registers[0xF] = (registers[inst.y] > registers[inst.x]) ? 1 : 0;
    registers[inst.x] = registers[inst.y] - registers[inst.x];
}
void Chip8::OP_8xyE()
{
    Instruction inst = decodeOpcode();
    registers[0xF] = (registers[inst.x] & 0x8) >> 7;

    registers[inst.x] <<= 1;
}

// Instructions 0x9*** - 0xD***
void Chip8::OP_9xy0()
{
    Instruction inst = decodeOpcode();
    if (registers[inst.x] != registers[inst.y])
    {
        pc += 2;
    }
}

void Chip8::OP_Annn()
{
    Instruction inst = decodeOpcode();

    index = inst.nnn;
}
void Chip8::OP_Bnnn()
{
    Instruction inst = decodeOpcode();

    pc = inst.nnn + registers[0x0];
}

void Chip8::OP_Cxkk()
{
    Instruction inst = decodeOpcode();

    registers[inst.x] = randomByte() & inst.kk;
}

void Chip8::OP_Dxyn()
{
    Instruction inst = decodeOpcode();

    uint8_t xPos = registers[inst.x] % Display::WIDTH;
    uint8_t yPos = registers[inst.y] % Display::HEIGHT;

    registers[0xF] = 0;

    for (uint8_t row = 0; row < inst.n; ++row)
    {
        uint8_t spriteByte = memory[index + row];
        for (uint8_t col = 0; col < 8; ++col)
        {
            if ((spriteByte & (0x80 >> col)) != 0)
            {
                uint32_t &pixelRef = display.pixels[((yPos + row) % Display::HEIGHT) * Display::WIDTH + ((xPos + col) % Display::WIDTH)];

                if (pixelRef != 0)
                    registers[0xF] = 1; // collision

                display.setPixel(xPos + col, yPos + row, pixelRef == 0);
            }
        }
    }
}

// Instructions 0xE***
void Chip8::OP_Ex9E()
{
    Instruction inst = decodeOpcode();
    uint8_t Vx = registers[inst.x];

    if (keypad[Vx] != 0)
    {
        pc += 2;
    }
}

void Chip8::OP_ExA1()
{
    Instruction inst = decodeOpcode();
    uint8_t Vx = registers[inst.x];

    if (keypad[Vx] == 0)
    {
        pc += 2;
    }
}

// Instructions 0xF***
void Chip8::OP_Fx07()
{
    Instruction inst = decodeOpcode();
    registers[inst.x] = delayTimer;
}

void Chip8::OP_Fx0A()
{
    Instruction inst = decodeOpcode();

    for (uint8_t i = 0; i < keypad.size(); i++)
    {
        if (keypad[i])
        {
            registers[inst.x] = i;
            return;
        }
    }

    pc -= 2;
}

void Chip8::OP_Fx15()
{
    Instruction inst = decodeOpcode();
    delayTimer = registers[inst.x];
}

void Chip8::OP_Fx18()
{
    Instruction inst = decodeOpcode();
    soundTimer = registers[inst.x];
}

void Chip8::OP_Fx1E()
{
    Instruction inst = decodeOpcode();
    index += registers[inst.x];
}

void Chip8::OP_Fx29()
{
    Instruction inst = decodeOpcode();
    index = FONT_START_ADDRESS + (registers[inst.x] * 5);
}

void Chip8::OP_Fx33()
{
    Instruction inst = decodeOpcode();
    uint8_t value = registers[inst.x];

    memory[index] = value / 100;
    memory[index + 1] = (value / 10) % 10;
    memory[index + 2] = value % 10;
}

void Chip8::OP_Fx55()
{
    Instruction inst = decodeOpcode();

    for (uint8_t i = 0; i <= inst.x; i++)
    {
        memory[index + i] = registers[i];
    }
}

void Chip8::OP_Fx65()
{
    Instruction inst = decodeOpcode();

    for (uint8_t i = 0; i <= inst.x; i++)
    {
        registers[i] = memory[index + i];
    }
}

// Dummy handler
void Chip8::OP_NULL()
{
    std::cout << "[DEBUG] Unknown instruction" << std::endl;
}
