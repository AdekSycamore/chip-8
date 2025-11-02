#pragma once
#include <array>
#include <cstdint>
#include <ncurses.h>

class Display {
public:
    static constexpr int WIDTH = 64;
    static constexpr int HEIGHT = 32;

    Display() = default;

    void clear();
    void initNcurses();
    void render();
    void setPixel(int x, int y, bool value);
    void drawFrame();

    std::array<uint32_t, WIDTH * HEIGHT> pixels{};

private:
    bool frameDrawn{false};
};
