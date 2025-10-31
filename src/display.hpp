#pragma once
#include <array>
#include <cstdint>

class Display {
public:
    static constexpr int WIDTH = 64;
    static constexpr int HEIGHT = 32;

    Display() = default;

    void clear();
    void render();
    void setPixel(int x, int y, bool value);
    void drawFrame() const;
    void drawPixels() const;

    std::array<uint32_t, WIDTH * HEIGHT> pixels{};
};
