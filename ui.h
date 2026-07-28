#ifndef UI_H
#define UI_H

#include "flashcard.h"

// Ma mau console
enum ConsoleColor {
    CLR_BLACK        = 0,
    CLR_BLUE         = 1,
    CLR_GREEN        = 2,
    CLR_CYAN         = 3,
    CLR_RED          = 4,
    CLR_MAGENTA      = 5,
    CLR_YELLOW       = 6,
    CLR_WHITE        = 7,
    CLR_GRAY         = 8,
    CLR_LIGHT_BLUE   = 9,
    CLR_LIGHT_GREEN  = 10,
    CLR_LIGHT_CYAN   = 11,
    CLR_LIGHT_RED    = 12,
    CLR_LIGHT_MAGENTA= 13,
    CLR_LIGHT_YELLOW = 14,
    CLR_BRIGHT_WHITE = 15
};
// --- Khoi tao & tien ich console ---
void initConsole();
void setColor(int textColor, int bgColor = CLR_BLACK);
void resetColor();
void clearScreen();

// --- Ve khung & tieu de ---
void drawTopBorder(int innerWidth);
void drawMidBorder(int innerWidth);
void drawBottomBorder(int innerWidth);
void drawEmptyBoxLine(int innerWidth);
void drawBoxLine(const string& text, int innerWidth, int color = CLR_WHITE);
void drawBoxLineCentered(const string& text, int innerWidth, int color = CLR_WHITE);
void drawThinSeparator(int innerWidth);
void drawTitle();

// --- Nhap lieu ---
string inputString(const string& prompt);
int    inputChoice(const string& prompt);
void   pauseScreen();
void   showMessage(const string& msg, int color);

#endif // UI_H