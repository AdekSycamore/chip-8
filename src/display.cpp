#include "display.hpp"
#include <iostream>
#include <ncurses.h>
#include <locale.h>

void Display::initNcurses()
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    cbreak();
}

void Display::render()
{
    if (!frameDrawn)
    {
        clear();
        drawFrame();
        frameDrawn = true;
    }

    for (int y = 0; y < HEIGHT; y += 2)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            uint32_t curTop = pixels[y * WIDTH + x];
            uint32_t curBottom = (y + 1 < HEIGHT) ? pixels[(y + 1) * WIDTH + x] : 0;

            const char *c = (curTop && curBottom) ? "█"
                            : curTop              ? "▀"
                            : curBottom           ? "▄"
                                                  : " ";

            mvaddstr((y / 2) + 1, x + 1, c);
        }
    }

    refresh();
}

void Display::drawFrame()
{
    int rows = (HEIGHT / 2) + 2;
    int cols = WIDTH + 2;
    
    for (int x = 1; x < cols - 1; ++x) {
        mvaddstr(0, x, "─");
        mvaddstr(rows - 1, x, "─");
    }

    for (int y = 1; y < rows - 1; ++y) {
        mvaddstr(y, 0, "│");
        mvaddstr(y, cols - 1, "│");
    }

    mvaddstr(0, 0, "┌");
    mvaddstr(0, cols - 1, "┐");
    mvaddstr(rows - 1, 0, "└");
    mvaddstr(rows - 1, cols - 1, "┘");
}

void Display::setPixel(int x, int y, bool value)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    pixels[y * WIDTH + x] = value ? 0xFFFFFFFF : 0;
}

void Display::clear()
{
    pixels.fill(0);
    frameDrawn = false;
}