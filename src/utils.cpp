#include "utils.hpp"
#include <iostream>
#include <thread>
#include <ncurses.h>
#include <cctype>

constexpr int KEYPAD_RESET_TIMEOUT_MS = 100;
constexpr int TIMER_UPDATE_INTERVAL_US = 33334; // ~30 Hz
constexpr char EXIT_KEY = 'Q';

void updateKeypad(Chip8 &chip8)
{
    static auto lastPressTime = std::chrono::high_resolution_clock::now();

    int ch;
    while ((ch = getch()) != ERR)
    {
        lastPressTime = std::chrono::high_resolution_clock::now();

        if (ch == EXIT_KEY)
        {
            endwin();
            exit(0);
        }

        chip8.updateKeypad(ch);
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPressTime);
    if (elapsed.count() > KEYPAD_RESET_TIMEOUT_MS)
    {
        chip8.resetKeypad();
    }
}

void updateTimers(Chip8 &chip8, std::chrono::high_resolution_clock::time_point &lastUpdate)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - lastUpdate;
    if (elapsed >= std::chrono::microseconds(TIMER_UPDATE_INTERVAL_US))
    {
        chip8.updateTimers();
        lastUpdate = now;
    }
}

void updateEmulation(Chip8 &chip8, int cyclesPerFrame)
{
    for (int i = 0; i < cyclesPerFrame; ++i)
    {
        chip8.cycle();
    }
}

void sleepToNextFrame(std::chrono::high_resolution_clock::time_point frameStart,
                      std::chrono::microseconds targetFrameTime)
{
    auto frameEnd = std::chrono::high_resolution_clock::now();
    auto frameDuration = frameEnd - frameStart;
    auto sleepDuration = targetFrameTime - frameDuration;

    if (sleepDuration > std::chrono::microseconds::zero())
    {
        std::this_thread::sleep_for(sleepDuration);
    }
}
