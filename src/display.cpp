#include "display.hpp"
#include <iostream>
#include <ncurses.h>

void Display::clear() {
    pixels.fill(0);
}

void Display::setPixel(int x, int y, bool value) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    pixels[y * WIDTH + x] = value ? 0xFFFFFFFF : 0;
}

void Display::drawFrame() const {
    // górna i dolna linia
    for (int x = 0; x < WIDTH + 2; ++x) {
        mvaddch(0, x, '-');
        mvaddch(HEIGHT + 1, x, '-');
    }
    // boczne linie
    for (int y = 1; y <= HEIGHT; ++y) {
        mvaddch(y, 0, '|');
        mvaddch(y, WIDTH + 1, '|');
    }
    // rogi
    mvaddch(0, 0, '+');
    mvaddch(0, WIDTH + 1, '+');
    mvaddch(HEIGHT + 1, 0, '+');
    mvaddch(HEIGHT + 1, WIDTH + 1, '+');
}

void Display::drawPixels() const {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            const char* c = pixels[y * WIDTH + x] ? "█" : " ";
            mvaddstr(y + 1, x + 1, c); // +1 dla ramki
        }
    }
}

void Display::render() {
    ::clear();  // Call ncurses clear() function
    drawFrame();
    drawPixels();
    refresh();
}
