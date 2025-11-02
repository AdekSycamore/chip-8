#pragma once
#include "chip8.hpp"
#include <chrono>

void updateKeypad(Chip8 &chip8);
void updateTimers(Chip8 &chip8, std::chrono::high_resolution_clock::time_point &lastUpdate);
void updateEmulation(Chip8 &chip8, int cyclesPerFrame);
void sleepToNextFrame(std::chrono::high_resolution_clock::time_point frameStart,
                      std::chrono::microseconds targetFrameTime);
