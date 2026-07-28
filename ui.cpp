#include "ui.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <iomanip>
#include <cstdio>
#include <windows.h>

// =====================================================================
//  HANG SO GIAO DIEN
// =====================================================================
static const int INNER_W = 60;   // Do rong noi dung ben trong khung

// Ky tu ve khung (UTF-8)
static const char* TL = "\xE2\x95\x94"; // Top-Left       
static const char* TR = "\xE2\x95\x97"; // Top-Right      
static const char* BL = "\xE2\x95\x9A"; // Bottom-Left    
static const char* BR = "\xE2\x95\x9D"; // Bottom-Right   
static const char* HZ = "\xE2\x95\x90"; // Horizontal     
static const char* VT = "\xE2\x95\x91"; // Vertical       
static const char* ML = "\xE2\x95\xA0"; // Mid-Left       
static const char* MR = "\xE2\x95\xA3"; // Mid-Right      
static const char* TH = "\xE2\x94\x80"; // Thin Horizontal

// =====================================================================
//  KHOI TAO CONSOLE
// =====================================================================

void initConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    SetConsoleTitleA("Flash Card - Hoc Tu Vung Tieng Anh");

    // An con tro nhap nhay
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    // Kich thuoc cua so
    SMALL_RECT rect = {0, 0, 79, 34};
    SetConsoleWindowInfo(hOut, TRUE, &rect);
}

void setColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            (WORD)(textColor | (bgColor << 4)));
}

void resetColor() {
    setColor(CLR_WHITE, CLR_BLACK);
}

void clearScreen() {
    system("cls");
}
// =====================================================================
//  VE KHUNG
// =====================================================================

// Padding trai cho moi dong
static void leftPad() {
    cout << "  ";
}

void drawTopBorder(int w) {
    setColor(CLR_CYAN);
    leftPad();
    cout << TL;
    for (int i = 0; i < w; i++) cout << HZ;
    cout << TR << "\n";
}

void drawMidBorder(int w) {
    setColor(CLR_CYAN);
    leftPad();
    cout << ML;
    for (int i = 0; i < w; i++) cout << HZ;
    cout << MR << "\n";
}

void drawBottomBorder(int w) {
    setColor(CLR_CYAN);
    leftPad();
    cout << BL;
    for (int i = 0; i < w; i++) cout << HZ;
    cout << BR << "\n";
}

void drawEmptyBoxLine(int w) {
    setColor(CLR_CYAN);
    leftPad();
    cout << VT;
    resetColor();
    for (int i = 0; i < w; i++) cout << " ";
    setColor(CLR_CYAN);
    cout << VT << "\n";
}

void drawBoxLine(const string& text, int w, int color) {
    setColor(CLR_CYAN);
    leftPad();
    cout << VT;
    setColor(color);
    cout << " " << text;
    int pad = w - 1 - displayWidth(text) - 1;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << " ";
    setColor(CLR_CYAN);
    cout << VT << "\n";
}

void drawBoxLineCentered(const string& text, int w, int color) {
    int textW = displayWidth(text);
    int totalPad = w - textW;
    int leftP = totalPad / 2;
    int rightP = totalPad - leftP;

    setColor(CLR_CYAN);
    leftPad();
    cout << VT;
    setColor(color);
    for (int i = 0; i < leftP; i++) cout << " ";
    cout << text;
    for (int i = 0; i < rightP; i++) cout << " ";
    setColor(CLR_CYAN);
    cout << VT << "\n";
}

void drawThinSeparator(int w) {
    setColor(CLR_CYAN);
    leftPad();
    cout << VT;
    setColor(CLR_GRAY);
    cout << " ";
    for (int i = 0; i < w - 2; i++) cout << TH;
    cout << " ";
    setColor(CLR_CYAN);
    cout << VT << "\n";
}
