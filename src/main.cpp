#include "chip8.hpp"
#include "utils.hpp"
#include <iostream>

constexpr int CYCLES_PER_FRAME = 5;
constexpr int TARGET_FPS = 240;  // Increased from 120 to 240 for better keyboard responsiveness
constexpr auto TARGET_FRAME_TIME = std::chrono::microseconds(1000000 / TARGET_FPS);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <ROM>\n";
        return 1;
    }

    const char *filename = argv[1];
    Chip8 chip8;
    if (!chip8.loadROM(filename))
    {
        return 1;
    }

    chip8.display.initNcurses();

    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();

    bool running = true;
    while (running)
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        updateKeypad(chip8);
        updateEmulation(chip8, CYCLES_PER_FRAME);
        updateTimers(chip8, lastTimerUpdate);

        chip8.display.render();

        sleepToNextFrame(frameStart, TARGET_FRAME_TIME);
    }

    endwin();
    return 0;
}
