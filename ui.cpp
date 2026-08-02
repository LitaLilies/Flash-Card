/*
 *  FLASH CARD - Hoc Tu Vung Tieng Anh
 *  File: ui.cpp
 *  Mo ta: Cai dat giao dien console (menu, hien thi, nhap lieu)
 */

#include "ui.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <clocale>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#endif
using namespace std;

#ifdef _WIN32
static void setConsoleUtf8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}
#else
static void setConsoleUtf8() {
    setlocale(LC_ALL, "");
}
#endif

#ifdef _WIN32
static int readConsoleKey() {
    return _getch();
}
#else
static int readConsoleKey() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch = 0;
    read(STDIN_FILENO, &ch, 1);

    if (ch == 27) {
        char seq[2] = {0, 0};
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 200000;
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        if (select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv) > 0) {
            read(STDIN_FILENO, &seq[0], 1);
            if (seq[0] == '[') {
                if (select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv) > 0) {
                    read(STDIN_FILENO, &seq[1], 1);
                }
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return 224;
                case 'B': return 224;
                case 'C': return 224;
                case 'D': return 224;
            }
        }
        return 27;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return static_cast<unsigned char>(ch);
}
#endif

#ifdef _WIN32
#define SLEEP_MS(ms) Sleep(ms)
#else
#define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

#ifndef _WIN32
int _getch() {
    return readConsoleKey();
}

void Sleep(unsigned int ms) {
    SLEEP_MS(ms);
}
#endif

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
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    SetConsoleTitleA("Flash Card - Học Từ Vựng Tiếng Anh");

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    SMALL_RECT rect = {0, 0, 79, 34};
    SetConsoleWindowInfo(hOut, TRUE, &rect);
#else
    setConsoleUtf8();
#endif
}

void setColor(int textColor, int bgColor) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            (WORD)(textColor | (bgColor << 4)));
#else
    (void)textColor;
    (void)bgColor;
#endif
}

void resetColor() {
#ifdef _WIN32
    setColor(CLR_WHITE, CLR_BLACK);
#else
    cout << "\033[0m";
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H";
    cout.flush();
#endif
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

// =====================================================================
//  TIEU DE CHUONG TRINH
// =====================================================================

void drawTitle() {
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("FLASH CARD", INNER_W, CLR_LIGHT_YELLOW);
    drawBoxLineCentered("Chương Trình Học Từ Vựng Tiếng Anh", INNER_W, CLR_YELLOW);
    drawEmptyBoxLine(INNER_W);
}

// =====================================================================
//  NHAP LIEU & TIEN ICH
// =====================================================================

static void showCursor() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(hOut, &ci);
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &ci);
#else
    cout << "\033[?25h";
#endif
}

static void hideCursor() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(hOut, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci);
#else
    cout << "\033[?25l";
#endif
}

string inputString(const string& prompt) {
    showCursor();
    setColor(CLR_LIGHT_CYAN);
    leftPad();
    cout << "  " << prompt;
    setColor(CLR_BRIGHT_WHITE);
    string input;
    getline(cin, input);
    hideCursor();
    return trim(input);
}

int inputChoice(const string& prompt) {
    string s = inputString(prompt);
    if (s.empty()) return -1;
    return atoi(s.c_str());
}

void pauseScreen() {
    cout << "\n";
    setColor(CLR_GRAY);
    leftPad();
    cout << "  Nhấn phím bất kỳ để tiếp tục...";
    resetColor();
    _getch();
}

void showMessage(const string& msg, int color) {
    cout << "\n";
    setColor(color);
    leftPad();
    cout << "  " << msg << "\n";
    resetColor();
}

// =====================================================================
//  CHON CHU DE
// =====================================================================

int selectTopic(TopicNode* root, string& selectedName) {
    int n = countTopics(root);
    if (n == 0) {
        showMessage("[!] Chưa có chủ đề nào. Hãy tạo chủ đề trước!", CLR_LIGHT_RED);
        pauseScreen();
        return -1;
    }

    string* names = new string[n];
    int cnt = 0;
    inorderTopics(root, names, cnt);

    drawMidBorder(INNER_W);
    drawBoxLineCentered("DANH SÁCH CHỦ ĐỀ", INNER_W, CLR_LIGHT_YELLOW);
    drawThinSeparator(INNER_W);

    for (int i = 0; i < cnt; i++) {
        string line = "  [" + to_string(i + 1) + "]  " + names[i];
        drawBoxLine(line, INNER_W, CLR_WHITE);
    }
    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);

    cout << "\n";
    int choice = inputChoice("Chọn chủ đề (1-" + to_string(cnt) + ", 0 = Quay lại): ");

    if (choice >= 1 && choice <= cnt) {
        selectedName = names[choice - 1];
        delete[] names;
        return choice;
    }
    delete[] names;
    return -1;
}

// =====================================================================
//  MENU CHINH
// =====================================================================

void showMainMenu(TopicNode*& root, HistoryList& history) {
    while (true) {
        clearScreen();
        drawTitle();
        drawMidBorder(INNER_W);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [1]  Quản lý chủ đề (Deck)", INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [2]  Học Flash Card",        INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [3]  Kiểm tra",               INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [4]  Ôn tập",                 INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [5]  Lịch sử kết quả",        INNER_W, CLR_BRIGHT_WHITE);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [6]  Trò chơi Nối từ",        INNER_W, CLR_LIGHT_CYAN);
        drawBoxLine("  [7]  Trò chơi Viết tiếng Anh", INNER_W, CLR_LIGHT_CYAN);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [0]  Thoát",                  INNER_W, CLR_GRAY);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Nhập lựa chọn của bạn: ");

        switch (choice) {
            case 1: menuDeckManagement(root); break;
            case 2: menuLearnFlashCard(root);  break;
            case 3: menuQuiz(root, history);   break;
            case 4: menuReview(root);          break;
            case 5: menuHistory(history);      break;
            case 6: menuWordMatching(root, history); break;
            case 7: menuReverseQuiz(root, history);  break;
            case 0:
                // Luu du lieu truoc khi thoat
                saveTopics("Topics.txt", root);
                saveHistory("History.txt", history);
                clearScreen();
                drawTopBorder(INNER_W);
                drawEmptyBoxLine(INNER_W);
                drawBoxLineCentered("Cảm ơn bạn đã sử dụng Flash Card!", INNER_W, CLR_LIGHT_GREEN);
                drawBoxLineCentered("Hẹn gặp lại!", INNER_W, CLR_YELLOW);
                drawEmptyBoxLine(INNER_W);
                drawBottomBorder(INNER_W);
                cout << "\n";
                resetColor();
                showCursor();
                return;
            default:
                showMessage("[!] Lựa chọn không hợp lệ!", CLR_LIGHT_RED);
                pauseScreen();
                break;
        }
    }
}

// =====================================================================
//  1. QUAN LY CHU DE (DECK)
// =====================================================================

void menuDeckManagement(TopicNode*& root) {
    while (true) {
        clearScreen();
        drawTitle();
        drawMidBorder(INNER_W);
        drawBoxLineCentered("QUẢN LÝ CHỦ ĐỀ", INNER_W, CLR_LIGHT_CYAN);
        drawThinSeparator(INNER_W);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [1]  Tạo chủ đề mới",      INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [2]  Đổi tên chủ đề",       INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [3]  Quản lý Flash Card",   INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [4]  Xóa chủ đề",           INNER_W, CLR_BRIGHT_WHITE);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [0]  Quay lại",             INNER_W, CLR_GRAY);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Nhập lựa chọn: ");

        switch (choice) {
            case 1: createDeck(root);      break;
            case 2: renameDeck(root);      break;
            case 3: manageCards(root);      break;
            case 4: deleteDeckMenu(root);  break;
            case 0: return;
            default:
                showMessage("[!] Lựa chọn không hợp lệ!", CLR_LIGHT_RED);
                pauseScreen();
                break;
        }
    }
}

// --- 1.1 Tao chu de ---

void createDeck(TopicNode*& root) {
    clearScreen();
    drawTitle();
    drawMidBorder(INNER_W);
    drawBoxLineCentered("TẠO CHỦ ĐỀ MỚI", INNER_W, CLR_LIGHT_CYAN);
    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);
    cout << "\n";

    string name = inputString("Nhập tên chủ đề (0 = Quay lại): ");

    if (name == "0") return;

    if (name.empty()) {
        showMessage("[!] Tên chủ đề không được để trống!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    if (!isValidTopicName(name)) {
        showMessage("[!] Tên chủ đề chứa ký tự không hợp lệ!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    if (searchTopic(root, name) != nullptr) {
        showMessage("[!] Chủ đề đã tồn tại!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    root = insertTopic(root, name);

    // Tao file rong cho chu de
    string cardFile = getCardFileName(name);
    ofstream f(cardFile);
    f.close();

    saveTopics("Topics.txt", root);
    showMessage("[OK] Đã tạo chủ đề: " + name, CLR_LIGHT_GREEN);

    // Hoi them Flash Card ngay
    cout << "\n";
    int ans = inputChoice("Bạn có muốn thêm Flash Card ngay? (1 = Có, 0 = Không): ");
    if (ans == 1) {
        addCardsFlow(name);
    }
}

// --- Them nhieu Flash Card ---

void addCardsFlow(const string& topicName) {
    CardList cards;
    initCardList(cards);
    loadCards(getCardFileName(topicName), cards);

    cout << "\n";
    setColor(CLR_LIGHT_CYAN);
    leftPad();
    cout << "  Nhập Flash Card (nhập 0 để kết thúc):\n";
    resetColor();

    while (true) {
        cout << "\n";
        string eng = inputString("  Từ tiếng Anh (0 = Xong): ");
        if (eng == "0" || eng.empty()) break;

        string vie = inputString("  Nghĩa tiếng Việt: ");
        if (vie == "0" || vie.empty()) {
            showMessage("[!] Nghĩa tiếng Việt không được để trống!", CLR_LIGHT_RED);
            continue;
        }

        addCard(cards, eng, vie);
        showMessage("  [+] Đã thêm: " + eng + " - " + vie, CLR_GREEN);
    }

    saveCards(getCardFileName(topicName), cards);
    showMessage("[OK] Đã lưu " + to_string(cards.count) + " Flash Card.", CLR_LIGHT_GREEN);
    freeCardList(cards);
    pauseScreen();
}

// --- 1.2 Doi ten chu de ---

void renameDeck(TopicNode*& root) {
    clearScreen();
    drawTitle();
    drawMidBorder(INNER_W);
    drawBoxLineCentered("ĐỔI TÊN CHỦ ĐỀ", INNER_W, CLR_LIGHT_CYAN);
    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);

    string selectedName;
    clearScreen();
    drawTitle();
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    cout << "\n";
    setColor(CLR_YELLOW);
    leftPad();
    cout << "  Chủ đề hiện tại: " << selectedName << "\n";
    resetColor();

    string newName = inputString("Nhập tên mới: ");

    if (newName.empty() || !isValidTopicName(newName)) {
        showMessage("[!] Tên mới không hợp lệ!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    if (searchTopic(root, newName) != nullptr) {
        showMessage("[!] Tên chủ đề đã tồn tại!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    // Doi ten file the
    string oldCardFile  = getCardFileName(selectedName);
    string newCardFile  = getCardFileName(newName);
    string oldWrongFile = getWrongFileName(selectedName);
    string newWrongFile = getWrongFileName(newName);

    rename(oldCardFile.c_str(), newCardFile.c_str());
    rename(oldWrongFile.c_str(), newWrongFile.c_str()); // Khong loi neu khong ton tai

    // Cap nhat BST
    root = deleteTopic(root, selectedName);
    root = insertTopic(root, newName);
    saveTopics("Topics.txt", root);

    showMessage("[OK] Đã đổi tên: " + selectedName + " -> " + newName, CLR_LIGHT_GREEN);
    pauseScreen();
}

// --- 1.3 Quan ly Flash Card ---

// Ham noi bo: in 1 hang trong bang the
static void printCardRow(int idx, const string& eng, const string& vie) {
    string stt = to_string(idx);
    while ((int)stt.length() < 3) stt = " " + stt;

    string engShow = eng;
    string vieShow = vie;
    if (displayWidth(engShow) > 22) engShow = engShow.substr(0, 20) + "..";
    if (displayWidth(vieShow) > 22) vieShow = vieShow.substr(0, 20) + "..";

    string engPad = engShow;
    int engW = displayWidth(engShow);
    for (int i = engW; i < 22; i++) engPad += " ";

    string viePad = vieShow;
    int vieW = displayWidth(vieShow);
    for (int i = vieW; i < 22; i++) viePad += " ";

    string line = "  " + stt + " | " + engPad + " | " + viePad;
    drawBoxLine(line, INNER_W, CLR_WHITE);
}

// Hien thi danh sach the theo thu tu goc (thu tu nhap vao)
static void displayCardList(CardList& cards) {
    if (cards.count == 0) {
        drawBoxLineCentered("(Chưa có Flash Card nào)", INNER_W, CLR_GRAY);
        return;
    }
    string header = "  STT | Tiếng Anh              | Tiếng Việt";
    drawBoxLine(header, INNER_W, CLR_LIGHT_YELLOW);
    drawThinSeparator(INNER_W);
    CardNode* curr = cards.head;
    for (int idx = 1; curr; idx++, curr = curr->next)
        printCardRow(idx, curr->english, curr->vietnamese);
}

// Hien thi danh sach the da duoc QUICK SORT theo ten tieng Anh A->Z
static void displaySortedCardList(CardList& cards) {
    if (cards.count == 0) {
        drawBoxLineCentered("(Chưa có Flash Card nào)", INNER_W, CLR_GRAY);
        return;
    }
    // --- AP DUNG QUICK SORT ---
    CardNode** arr = cardListToSortedArray(cards);   // O(n log n)

    drawBoxLineCentered("(Sắp xếp A->Z bằng Quick Sort)", INNER_W, CLR_LIGHT_CYAN);
    string header = "  STT | Tiếng Anh              | Tiếng Việt";
    drawBoxLine(header, INNER_W, CLR_LIGHT_YELLOW);
    drawThinSeparator(INNER_W);
    for (int i = 0; i < cards.count; i++)
        printCardRow(i + 1, arr[i]->english, arr[i]->vietnamese);

    delete[] arr;
}

void manageCards(TopicNode* root) {
    string selectedName;
    clearScreen();
    drawTitle();
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    while (true) {
        clearScreen();
        drawTitle();
        drawMidBorder(INNER_W);
        drawBoxLineCentered("QUẢN LÝ FLASH CARD", INNER_W, CLR_LIGHT_CYAN);
        drawBoxLineCentered("Chủ đề: " + selectedName, INNER_W, CLR_YELLOW);
        drawThinSeparator(INNER_W);

        // Load va hien thi danh sach
        CardList cards;
        initCardList(cards);
        loadCards(getCardFileName(selectedName), cards);

        displayCardList(cards);

        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [1] Thêm  [2] Sửa  [3] Xóa  [4] Xem thẻ  [0] Quay lại", INNER_W, CLR_LIGHT_GREEN);
        drawBoxLine("  [5] Sắp xếp A->Z (Quick Sort)", INNER_W, CLR_LIGHT_CYAN);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Nhập lựa chọn: ");

        if (choice == 0) {
            freeCardList(cards);
            return;
        }

        // [5] Sap xep bang Quick Sort
        if (choice == 5) {
            clearScreen();
            drawTitle();
            drawMidBorder(INNER_W);
            drawBoxLineCentered("SẮP XẾPH FLASH CARD - QUICK SORT", INNER_W, CLR_LIGHT_CYAN);
            drawBoxLineCentered("Chủ đề: " + selectedName, INNER_W, CLR_YELLOW);
            drawThinSeparator(INNER_W);
            displaySortedCardList(cards);
            drawEmptyBoxLine(INNER_W);
            drawBottomBorder(INNER_W);
            showMessage("[i] Quick Sort A->Z hoàn tất! (Thứ tự gốc không thay đổi)", CLR_LIGHT_CYAN);
            freeCardList(cards);
            pauseScreen();
            continue;
        }

        if (choice == 4) {
            // Xem the - duyet toi/lui su dung DSLK kep (prev/next)
            if (cards.count == 0) {
                showMessage("[!] Không có thẻ nào để xem!", CLR_LIGHT_RED);
            } else {
                CardNode* current = cards.head;
                int pos = 1;
                bool browsing = true;
                while (browsing) {
                    clearScreen();
                    drawTopBorder(INNER_W);
                    drawEmptyBoxLine(INNER_W);
                    string info = "XEM THẺ  [" + to_string(pos) + "/" + to_string(cards.count) + "]";
                    drawBoxLineCentered(info, INNER_W, CLR_LIGHT_YELLOW);
                    drawEmptyBoxLine(INNER_W);
                    drawThinSeparator(INNER_W);
                    drawEmptyBoxLine(INNER_W);
                    drawBoxLineCentered(current->english, INNER_W, CLR_BRIGHT_WHITE);
                    drawEmptyBoxLine(INNER_W);
                    drawThinSeparator(INNER_W);
                    drawEmptyBoxLine(INNER_W);
                    drawBoxLineCentered(current->vietnamese, INNER_W, CLR_LIGHT_GREEN);
                    drawEmptyBoxLine(INNER_W);
                    drawMidBorder(INNER_W);
                    string nav = "";
                    if (current->prev) nav += "[<-] Lùi  ";
                    if (current->next) nav += "[->] Tới  ";
                    nav += "[ESC] Thoát";
                    drawBoxLineCentered(nav, INNER_W, CLR_LIGHT_CYAN);
                    drawBottomBorder(INNER_W);

                    int key = _getch();
                    if (key == 27) { // ESC
                        browsing = false;
                    } else if (key == 0 || key == 224) {
                        // Phim mui ten
                        int arrow = _getch();
                        if (arrow == 75 && current->prev) { // Left arrow
                            current = current->prev;
                            pos--;
                        } else if (arrow == 77 && current->next) { // Right arrow
                            current = current->next;
                            pos++;
                        }
                    }
                }
            }
            freeCardList(cards);
        }
        else if (choice == 1) {
            // Them
            freeCardList(cards);
            addCardsFlow(selectedName);
        }
        else if (choice == 2) {
            // Sua
            if (cards.count == 0) {
                showMessage("[!] Không có thẻ nào để sửa!", CLR_LIGHT_RED);
                freeCardList(cards);
                pauseScreen();
                continue;
            }
            int idx = inputChoice("Nhập STT cần sửa (1-" + to_string(cards.count) + "): ");
            CardNode* node = getCard(cards, idx);
            if (node == nullptr) {
                showMessage("[!] STT không hợp lệ!", CLR_LIGHT_RED);
            } else {
                cout << "\n";
                setColor(CLR_YELLOW);
                leftPad();
                cout << "  Hiện tại: " << node->english << " - " << node->vietnamese << "\n";
                resetColor();

                string newEng = inputString("  Từ tiếng Anh mới: ");
                string newVie = inputString("  Nghĩa tiếng Việt mới: ");

                if (!newEng.empty() && !newVie.empty()) {
                    editCard(cards, idx, newEng, newVie);
                    saveCards(getCardFileName(selectedName), cards);
                    showMessage("[OK] Đã cập nhật Flash Card!", CLR_LIGHT_GREEN);
                } else {
                    showMessage("[!] Không được để trống!", CLR_LIGHT_RED);
                }
            }
            freeCardList(cards);
            pauseScreen();
        }
        else if (choice == 3) {
            // Xoa
            if (cards.count == 0) {
                showMessage("[!] Không có thẻ nào để xóa!", CLR_LIGHT_RED);
                freeCardList(cards);
                pauseScreen();
                continue;
            }
            int idx = inputChoice("Nhập STT cần xóa (1-" + to_string(cards.count) + "): ");
            CardNode* node = getCard(cards, idx);
            if (node == nullptr) {
                showMessage("[!] STT không hợp lệ!", CLR_LIGHT_RED);
            } else {
                cout << "\n";
                setColor(CLR_YELLOW);
                leftPad();
                cout << "  Sẽ xóa: " << node->english << " - " << node->vietnamese << "\n";
                resetColor();

                int confirm = inputChoice("  Xác nhận xóa? (1 = Có, 0 = Không): ");
                if (confirm == 1) {
                    // Xoa tu tuong ung khoi file on tap
                    string engToDelete = node->english;
                    CardList wrongList;
                    initCardList(wrongList);
                    loadCards(getWrongFileName(selectedName), wrongList);
                    if (wrongList.count > 0) {
                        CardNode* wc = wrongList.head;
                        int wIdx = 1;
                        while (wc) {
                            CardNode* nextWc = wc->next;
                            if (compareAnswer(wc->english, engToDelete)) {
                                deleteCard(wrongList, wIdx);
                                break;
                            }
                            wc = nextWc;
                            wIdx++;
                        }
                        saveCards(getWrongFileName(selectedName), wrongList);
                    }
                    freeCardList(wrongList);

                    deleteCard(cards, idx);
                    saveCards(getCardFileName(selectedName), cards);
                    showMessage("[OK] Đã xóa Flash Card!", CLR_LIGHT_GREEN);
                } else {
                    showMessage("[i] Đã hủy xóa.", CLR_GRAY);
                }
            }
            freeCardList(cards);
            pauseScreen();
        }
        else {
            freeCardList(cards);
            showMessage("[!] Lựa chọn không hợp lệ!", CLR_LIGHT_RED);
            pauseScreen();
        }
    }
}

// --- 1.4 Xoa chu de ---


void deleteDeckMenu(TopicNode*& root) {
    clearScreen();
    drawTitle();
    string selectedName;
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    cout << "\n";
    setColor(CLR_LIGHT_RED);
    leftPad();
    cout << "  CẢNH BÁO: Sẽ xóa chủ đề \"" << selectedName << "\" và tất cả dữ liệu liên quan!\n";
    resetColor();

    int confirm = inputChoice("Xác nhận xóa? (1 = Có, 0 = Không): ");
    if (confirm != 1) {
        showMessage("[i] Đã hủy xóa.", CLR_GRAY);
        pauseScreen();
        return;
    }

    // Xoa file
    remove(getCardFileName(selectedName).c_str());
    remove(getWrongFileName(selectedName).c_str());

    // Xoa khoi BST
    root = deleteTopic(root, selectedName);
    saveTopics("Topics.txt", root);

    showMessage("[OK] Đã xóa chủ đề: " + selectedName, CLR_LIGHT_GREEN);
    pauseScreen();
}

// =====================================================================
//  2. HOC FLASH CARD (su dung Queue)
// =====================================================================

static void drawFlashCardUI(const string& title, int current, int total,
                             const string& english, const string& vietnamese,
                             bool showAnswer) {
    clearScreen();
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);

    string info;
    if (current > 0 && total > 0) {
        info = title + "  [" + to_string(current) + "/" + to_string(total) + "]";
    } else {
        info = title;
    }
    drawBoxLineCentered(info, INNER_W, CLR_LIGHT_YELLOW);

    drawEmptyBoxLine(INNER_W);
    drawThinSeparator(INNER_W);
    drawEmptyBoxLine(INNER_W);

    // Mat truoc: tieng Anh
    drawBoxLineCentered(english, INNER_W, CLR_BRIGHT_WHITE);

    drawEmptyBoxLine(INNER_W);

    if (showAnswer) {
        drawThinSeparator(INNER_W);
        drawEmptyBoxLine(INNER_W);
        drawBoxLineCentered(vietnamese, INNER_W, CLR_LIGHT_GREEN);
        drawEmptyBoxLine(INNER_W);
    } else {
        drawEmptyBoxLine(INNER_W);
        drawBoxLineCentered("[Nhấn ENTER để lật thẻ]", INNER_W, CLR_GRAY);
        drawEmptyBoxLine(INNER_W);
    }

    drawEmptyBoxLine(INNER_W);
}

void menuLearnFlashCard(TopicNode* root) {
  while (true) {
    clearScreen();
    drawTitle();
    string selectedName;
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    // Load the vao danh sach lien ket doi
    CardList cards;
    initCardList(cards);
    loadCards(getCardFileName(selectedName), cards);

    if (cards.count == 0) {
        showMessage("[!] Chủ đề này chưa có Flash Card nào!", CLR_LIGHT_RED);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    // Dua tat ca the vao Queue
    CardQueue queue;
    initQueue(queue);
    CardNode* curr = cards.head;
    while (curr) {
        enqueue(queue, curr->english, curr->vietnamese);
        curr = curr->next;
    }
    int totalCards = cards.count;

    // Danh sach chua nho
    CardList wrongCards;
    initCardList(wrongCards);

    int cardNum = 0;

    while (!isQueueEmpty(queue)) {
        QueueNode* qn = dequeue(queue);
        cardNum++;

        // Hien thi mat truoc
        drawFlashCardUI("HỌC FLASH CARD - " + selectedName,
                        cardNum, totalCards,
                        qn->english, qn->vietnamese, false);
        drawBottomBorder(INNER_W);
        _getch(); // Doi nhan phim de lat the

        // Hien thi ca 2 mat
        drawFlashCardUI("HỌC FLASH CARD - " + selectedName,
                        cardNum, totalCards,
                        qn->english, qn->vietnamese, true);

        drawMidBorder(INNER_W);
        drawBoxLine("  [1] Nhớ     [2] Chưa nhớ     [0] Thoát", INNER_W, CLR_LIGHT_CYAN);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Lựa chọn của bạn: ");

        if (choice == 0) {
            // Thoat - luu cac the con lai trong queue vao wrong
            addCard(wrongCards, qn->english, qn->vietnamese);
            while (!isQueueEmpty(queue)) {
                QueueNode* remaining = dequeue(queue);
                addCard(wrongCards, remaining->english, remaining->vietnamese);
                delete remaining;
            }
            delete qn;
            break;
        }
        else if (choice == 2) {
            // Chua nho -> luu vao danh sach on tap
            addCard(wrongCards, qn->english, qn->vietnamese);
        }
        // choice == 1: Nho -> khong lam gi

        delete qn;
    }

    // Luu danh sach chua nho vao file on tap
    if (wrongCards.count > 0) {
        // Doc file on tap cu (neu co) va gop lai
        CardList existingWrong;
        initCardList(existingWrong);
        loadCards(getWrongFileName(selectedName), existingWrong);

        // Them cac tu moi (khong trung)
        CardNode* wc = wrongCards.head;
        while (wc) {
            // Kiem tra trung
            bool found = false;
            CardNode* ec = existingWrong.head;
            while (ec) {
                if (compareAnswer(ec->english, wc->english)) {
                    found = true;
                    break;
                }
                ec = ec->next;
            }
            if (!found) {
                addCard(existingWrong, wc->english, wc->vietnamese);
            }
            wc = wc->next;
        }

        saveCards(getWrongFileName(selectedName), existingWrong);
        freeCardList(existingWrong);

        showMessage("[i] Có " + to_string(wrongCards.count) + " từ chưa nhớ đã lưu vào ôn tập.",
                    CLR_LIGHT_YELLOW);
    } else {
        showMessage("[OK] Tuyệt vời! Bạn đã nhớ tất cả từ vựng!", CLR_LIGHT_GREEN);
    }

    freeCardList(wrongCards);
    freeCardList(cards);
    freeQueue(queue);
    pauseScreen();
  } // end while
}

// =====================================================================
//  3. KIEM TRA
// =====================================================================

void menuQuiz(TopicNode* root, HistoryList& history) {
  while (true) {
    clearScreen();
    drawTitle();
    string selectedName;
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    // Load the
    CardList cards;
    initCardList(cards);
    loadCards(getCardFileName(selectedName), cards);

    if (cards.count == 0) {
        showMessage("[!] Chủ đề này chưa có Flash Card nào!", CLR_LIGHT_RED);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    int totalCards = cards.count;
    int correctCount = 0;

    // Danh sach lam sai
    CardList wrongCards;
    initCardList(wrongCards);

    CardNode* curr = cards.head;
    int idx = 0;
    bool quitted = false;

    while (curr) {
        idx++;
        clearScreen();
        drawTopBorder(INNER_W);
        drawEmptyBoxLine(INNER_W);

        string info = "KIỂM TRA - " + selectedName + "  [" + to_string(idx) + "/" + to_string(totalCards) + "]";
        drawBoxLineCentered(info, INNER_W, CLR_LIGHT_YELLOW);

        drawEmptyBoxLine(INNER_W);
        drawThinSeparator(INNER_W);
        drawEmptyBoxLine(INNER_W);
        drawBoxLineCentered(curr->english, INNER_W, CLR_BRIGHT_WHITE);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);

        cout << "\n";
        string answer = inputString("Nhập nghĩa tiếng Việt (0 = Thoát): ");

        if (answer == "0") {
            // Thoat giua chung - tinh so cau da lam
            totalCards = idx - 1;
            quitted = true;
            break;
        }

        if (compareAnswerFlexible(answer, curr->vietnamese)) {
            correctCount++;
            string feedback = getCorrectFeedback();
            showMessage("[OK] ĐÚNG RỒI! " + feedback, CLR_LIGHT_GREEN);
        } else {
            string feedback = getWrongFeedback();
            showMessage("[X] SAI! " + feedback, CLR_LIGHT_RED);
            showMessage("     Đáp án đúng: " + curr->vietnamese, CLR_YELLOW);
            addCard(wrongCards, curr->english, curr->vietnamese);
        }

        pauseScreen();
        curr = curr->next;
    }

    if (quitted && totalCards == 0) {
        showMessage("[i] Đã thoát kiểm tra.", CLR_GRAY);
        freeCardList(wrongCards);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    // Hien thi ket qua
    clearScreen();
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("KẾT QUẢ KIỂM TRA", INNER_W, CLR_LIGHT_YELLOW);
    drawBoxLineCentered("Chủ đề: " + selectedName, INNER_W, CLR_YELLOW);
    drawEmptyBoxLine(INNER_W);
    drawThinSeparator(INNER_W);
    drawEmptyBoxLine(INNER_W);

    string resultStr = "Điểm: " + to_string(correctCount) + " / " + to_string(totalCards);
    int pct = (totalCards > 0) ? (correctCount * 100 / totalCards) : 0;
    string pctStr = "Tỷ lệ đúng: " + to_string(pct) + "%";

    int resultColor = (pct >= 80) ? CLR_LIGHT_GREEN :
                      (pct >= 50) ? CLR_LIGHT_YELLOW : CLR_LIGHT_RED;

    drawBoxLineCentered(resultStr, INNER_W, resultColor);
    drawBoxLineCentered(pctStr,    INNER_W, resultColor);

    drawEmptyBoxLine(INNER_W);

    if (pct >= 80) {
        drawBoxLineCentered("Xuất sắc!", INNER_W, CLR_LIGHT_GREEN);
    } else if (pct >= 50) {
        drawBoxLineCentered("Khá tốt! Cần ôn tập thêm!", INNER_W, CLR_LIGHT_YELLOW);
    } else {
        drawBoxLineCentered("Cần cố gắng hơn! Hãy ôn tập lại!", INNER_W, CLR_LIGHT_RED);
    }

    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);

    // Luu vao lich su
    string dateStr = getCurrentDate();
    addHistory(history, dateStr, selectedName, correctCount, totalCards);
    saveHistory("History.txt", history);

    // Luu tu sai vao file on tap
    if (wrongCards.count > 0) {
        CardList existingWrong;
        initCardList(existingWrong);
        loadCards(getWrongFileName(selectedName), existingWrong);

        CardNode* wc = wrongCards.head;
        while (wc) {
            bool found = false;
            CardNode* ec = existingWrong.head;
            while (ec) {
                if (compareAnswer(ec->english, wc->english)) {
                    found = true;
                    break;
                }
                ec = ec->next;
            }
            if (!found) {
                addCard(existingWrong, wc->english, wc->vietnamese);
            }
            wc = wc->next;
        }

        saveCards(getWrongFileName(selectedName), existingWrong);
        freeCardList(existingWrong);
    }

    freeCardList(wrongCards);
    freeCardList(cards);
    pauseScreen();
  } // end while
}

// =====================================================================
//  4. ON TAP (su dung Queue - the chua nho duoc dua lai cuoi queue)
// =====================================================================

void menuReview(TopicNode* root) {
  while (true) {
    clearScreen();
    drawTitle();
    string selectedName;
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    // Doc file on tap
    CardList wrongCards;
    initCardList(wrongCards);
    loadCards(getWrongFileName(selectedName), wrongCards);

    if (wrongCards.count == 0) {
        showMessage("[OK] Không có từ nào cần ôn tập! Bạn đã nhớ hết rồi!", CLR_LIGHT_GREEN);
        freeCardList(wrongCards);
        pauseScreen();
        return;
    }

    // Dua vao Queue de on tap
    CardQueue queue;
    initQueue(queue);
    CardNode* curr = wrongCards.head;
    while (curr) {
        enqueue(queue, curr->english, curr->vietnamese);
        curr = curr->next;
    }

    int totalReview = wrongCards.count;
    int rememberedCount = 0;
    int cardNum = 0;

    showMessage("[i] Có " + to_string(totalReview) + " từ cần ôn tập. Bạn sẽ học lại cho đến khi nhớ hết!",
                CLR_LIGHT_CYAN);
    pauseScreen();

    while (!isQueueEmpty(queue)) {
        QueueNode* qn = dequeue(queue);
        cardNum++;

        string info = "ÔN TẬP - " + selectedName + "  [Đã nhớ: " + to_string(rememberedCount) + "/" + to_string(totalReview) + "]";

        // Hien thi mat truoc
        drawFlashCardUI(info, 0, 0,
                        qn->english, qn->vietnamese, false);
        drawBottomBorder(INNER_W);
        _getch();

        // Hien thi ca 2 mat
        drawFlashCardUI(info, 0, 0,
                        qn->english, qn->vietnamese, true);

        drawMidBorder(INNER_W);
        drawBoxLine("  [1] Đã nhớ   [2] Chưa nhớ   [0] Thoát", INNER_W, CLR_LIGHT_CYAN);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Lựa chọn của bạn: ");

        if (choice == 0) {
            // Thoat - dua the hien tai va con lai tro lai
            enqueue(queue, qn->english, qn->vietnamese);
            delete qn;
            break;
        }
        else if (choice == 1) {
            // Da nho -> khong dua lai queue
            rememberedCount++;
            string feedback = getCorrectFeedback();
            showMessage("[OK] " + feedback + " Đã xóa khỏi danh sách ôn tập!", CLR_LIGHT_GREEN);
            Sleep(800);
        }
        else {
            // Chua nho -> dua lai cuoi queue
            enqueue(queue, qn->english, qn->vietnamese);
            string feedback = getWrongFeedback();
            showMessage("[i] " + feedback + " Thẻ sẽ quay lại sau!", CLR_LIGHT_YELLOW);
            Sleep(800);
        }

        delete qn;
    }

    // Luu lai cac the con trong queue (chua nho)
    CardList remaining;
    initCardList(remaining);
    while (!isQueueEmpty(queue)) {
        QueueNode* qn = dequeue(queue);
        addCard(remaining, qn->english, qn->vietnamese);
        delete qn;
    }

    saveCards(getWrongFileName(selectedName), remaining);

    // Thong bao ket qua
    clearScreen();
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("KẾT QUẢ ÔN TẬP", INNER_W, CLR_LIGHT_YELLOW);
    drawEmptyBoxLine(INNER_W);
    drawThinSeparator(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("Đã nhớ: " + to_string(rememberedCount) + " từ", INNER_W, CLR_LIGHT_GREEN);
    drawBoxLineCentered("Còn lại: " + to_string(remaining.count) + " từ cần ôn tập", INNER_W,
                        remaining.count > 0 ? CLR_LIGHT_YELLOW : CLR_LIGHT_GREEN);
    drawEmptyBoxLine(INNER_W);

    if (remaining.count == 0) {
        drawBoxLineCentered("Tuyệt vời! Bạn đã nhớ tất cả!", INNER_W, CLR_LIGHT_GREEN);
        // Xoa file on tap neu da nho het
        remove(getWrongFileName(selectedName).c_str());
    } else {
        drawBoxLineCentered("Hãy tiếp tục ôn tập!", INNER_W, CLR_LIGHT_YELLOW);
    }

    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);

    freeCardList(remaining);
    freeCardList(wrongCards);
    freeQueue(queue);
    pauseScreen();
  } // end while
}

// =====================================================================
//  5. LICH SU KET QUA
// =====================================================================

void menuHistory(HistoryList& history) {
  while (true) {
    clearScreen();
    drawTitle();
    drawMidBorder(INNER_W);
    drawBoxLineCentered("LỊCH SỬ KIỂM TRA", INNER_W, CLR_LIGHT_YELLOW);
    drawThinSeparator(INNER_W);

    if (history.count == 0) {
        drawEmptyBoxLine(INNER_W);
        drawBoxLineCentered("(Chưa có lịch sử kiểm tra nào)", INNER_W, CLR_GRAY);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        pauseScreen();
        return;
    }

    // Tieu de
    string header = " STT | Ngày kiểm tra     | Chủ đề         | Kết quả";
    drawBoxLine(header, INNER_W, CLR_LIGHT_CYAN);
    drawThinSeparator(INNER_W);

    HistoryNode* curr = history.head;
    int idx = 1;
    while (curr) {
        string stt = to_string(idx);
        while ((int)stt.length() < 3) stt = " " + stt;

        string date = curr->date;
        int dateW = displayWidth(date);
        for (int i = dateW; i < 16; i++) date += " ";

        string topic = curr->topic;
        int topicW = displayWidth(topic);
        if (topicW > 14) topic = topic.substr(0, 12) + "..";
        topicW = displayWidth(topic);
        for (int i = topicW; i < 14; i++) topic += " ";

        string result = to_string(curr->correct) + "/" + to_string(curr->total);
        int pct = (curr->total > 0) ? (curr->correct * 100 / curr->total) : 0;
        result += " (" + to_string(pct) + "%)";

        string line = " " + stt + " | " + date + " | " + topic + " | " + result;

        int color = (pct >= 80) ? CLR_LIGHT_GREEN :
                    (pct >= 50) ? CLR_LIGHT_YELLOW : CLR_LIGHT_RED;

        drawBoxLine(line, INNER_W, color);

        curr = curr->next;
        idx++;
    }

    drawEmptyBoxLine(INNER_W);
    drawBoxLine("  [1] Xóa 1 mục  [2] Xóa toàn bộ  [0] Quay lại", INNER_W, CLR_LIGHT_GREEN);
    drawBoxLine("  [3] Sắp xếp theo điểm % (Merge Sort)", INNER_W, CLR_LIGHT_CYAN);
    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);
    cout << "\n";

    int choice = inputChoice("Nhập lựa chọn: ");

    if (choice == 0) return;

    if (choice == 3) {
        // --- AP DUNG MERGE SORT: sap xep lich su theo diem % giam dan ---
        clearScreen();
        drawTitle();
        drawMidBorder(INNER_W);
        drawBoxLineCentered("LỊCH SỬ - SẮP XẾPH THEO ĐIỂM (MERGE SORT)", INNER_W, CLR_LIGHT_YELLOW);
        drawBoxLineCentered("Giảm dần: cao nhất -> thấp nhất", INNER_W, CLR_LIGHT_CYAN);
        drawThinSeparator(INNER_W);

        HistoryNode** arr = historyToSortedArray(history);   // O(n log n)

        string header = " STT | Ngày kiểm tra     | Chủ đề         | Kết quả";
        drawBoxLine(header, INNER_W, CLR_LIGHT_CYAN);
        drawThinSeparator(INNER_W);

        for (int i = 0; i < history.count; i++) {
            HistoryNode* h = arr[i];
            string stt = to_string(i + 1);
            while ((int)stt.length() < 3) stt = " " + stt;

            string date = h->date;
            int dateW = displayWidth(date);
            for (int k = dateW; k < 16; k++) date += " ";

            string topic = h->topic;
            int topicW = displayWidth(topic);
            if (topicW > 14) topic = topic.substr(0, 12) + "..";
            topicW = displayWidth(topic);
            for (int k = topicW; k < 14; k++) topic += " ";

            int pct = (h->total > 0) ? (h->correct * 100 / h->total) : 0;
            string result = to_string(h->correct) + "/" + to_string(h->total)
                          + " (" + to_string(pct) + "%)";

            string line = " " + stt + " | " + date + " | " + topic + " | " + result;
            int color = (pct >= 80) ? CLR_LIGHT_GREEN :
                        (pct >= 50) ? CLR_LIGHT_YELLOW : CLR_LIGHT_RED;
            drawBoxLine(line, INNER_W, color);
        }

        delete[] arr;

        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        showMessage("[i] Merge Sort hoàn tất! Thứ tự gốc không thay đổi.", CLR_LIGHT_CYAN);
        pauseScreen();
    }
    else if (choice == 1) {
        int delIdx = inputChoice("Nhập STT cần xóa (1-" + to_string(history.count) + "): ");
        if (delIdx < 1 || delIdx > history.count) {
            showMessage("[!] STT không hợp lệ!", CLR_LIGHT_RED);
        } else {
            deleteHistory(history, delIdx);
            saveHistory("History.txt", history);
            showMessage("[OK] Đã xóa mục lịch sử!", CLR_LIGHT_GREEN);
        }
        pauseScreen();
    }
    else if (choice == 2) {
        int confirm = inputChoice("Xác nhận xóa TOÀN BỘ lịch sử? (1 = Có, 0 = Không): ");
        if (confirm == 1) {
            clearHistory(history);
            saveHistory("History.txt", history);
            showMessage("[OK] Đã xóa toàn bộ lịch sử!", CLR_LIGHT_GREEN);
        } else {
            showMessage("[i] Đã hủy.", CLR_GRAY);
        }
        pauseScreen();
    }
    else {
        showMessage("[!] Lựa chọn không hợp lệ!", CLR_LIGHT_RED);
        pauseScreen();
    }
  } // end while
}


// =====================================================================
//  6. TRO CHOI NOI TU (WORD MATCHING)
// =====================================================================

void menuWordMatching(TopicNode* root, HistoryList& history) {
  while (true) {
    clearScreen();
    drawTitle();
    string selectedName;
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    // Load the
    CardList cards;
    initCardList(cards);
    loadCards(getCardFileName(selectedName), cards);

    if (cards.count == 0) {
        showMessage("[!] Chủ đề này chưa có Flash Card nào!", CLR_LIGHT_RED);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    // Toi thieu 3 tu de choi
    if (cards.count < 3) {
        showMessage("[!] Cần ít nhất 3 từ để chơi trò nối từ!", CLR_LIGHT_RED);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    // Chuyen sang mang de tron
    CardNode** cardArr = new CardNode*[cards.count];
    CardNode* curr = cards.head;
    int idx = 0;
    while (curr) {
        cardArr[idx++] = curr;
        curr = curr->next;
    }

    // Tron ngau nhien
    shuffleCards(cardArr, cards.count);

    int questionsPerRound = (cards.count >= 5) ? 5 : cards.count;
    int totalCorrect = 0;
    int totalQuestions = 0;
    bool quitted = false;

    for (int round = 0; round < cards.count; round += questionsPerRound) {
        int numInRound = (round + questionsPerRound <= cards.count) ? questionsPerRound : (cards.count - round);
        
        // Lay cac tu cho round nay
        CardNode** roundCards = cardArr + round;
        
        // Tron ngau nhien dap an
        string* shuffledAnswers = new string[numInRound];
        for (int i = 0; i < numInRound; i++) {
            shuffledAnswers[i] = roundCards[i]->vietnamese;
        }
        // Tron cac dap an
        for (int i = numInRound - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            string temp = shuffledAnswers[i];
            shuffledAnswers[i] = shuffledAnswers[j];
            shuffledAnswers[j] = temp;
        }

        // Hien thi cau hoi
        for (int q = 0; q < numInRound; q++) {
            clearScreen();
            drawTopBorder(INNER_W);
            drawEmptyBoxLine(INNER_W);
            
            string info = "TRÒ NỐI TỪ - " + selectedName + "  [" + to_string(totalQuestions + 1) + "/" + to_string(cards.count) + "]";
            drawBoxLineCentered(info, INNER_W, CLR_LIGHT_YELLOW);
            
            drawEmptyBoxLine(INNER_W);
            drawThinSeparator(INNER_W);
            drawEmptyBoxLine(INNER_W);
            
            // Hien thi tu tieng Anh
            drawBoxLineCentered("Từ cần nối: " + roundCards[q]->english, INNER_W, CLR_BRIGHT_WHITE);
            
            drawEmptyBoxLine(INNER_W);
            drawThinSeparator(INNER_W);
            drawEmptyBoxLine(INNER_W);
            
            // Hien thi cac lua chon
            drawBoxLine("  Chọn nghĩa đúng:", INNER_W, CLR_LIGHT_CYAN);
            drawEmptyBoxLine(INNER_W);
            
            for (int i = 0; i < numInRound; i++) {
                string choice = "  [" + to_string(i + 1) + "]  " + shuffledAnswers[i];
                drawBoxLine(choice, INNER_W, CLR_WHITE);
            }
            
            drawEmptyBoxLine(INNER_W);
            drawBottomBorder(INNER_W);
            
            cout << "\n";
            int answer = inputChoice("Chọn đáp án (1-" + to_string(numInRound) + ", 0 = Thoát): ");
            
            if (answer == 0) {
                quitted = true;
                break;
            }
            
            totalQuestions++;
            
            if (answer >= 1 && answer <= numInRound) {
                if (compareAnswerFlexible(shuffledAnswers[answer - 1], roundCards[q]->vietnamese)) {
                    totalCorrect++;
                    string feedback = getCorrectFeedback();
                    showMessage("[OK] ĐÚNG RỒI! " + feedback, CLR_LIGHT_GREEN);
                } else {
                    string feedback = getWrongFeedback();
                    showMessage("[X] SAI! " + feedback, CLR_LIGHT_RED);
                    showMessage("     Đáp án đúng: " + roundCards[q]->vietnamese, CLR_YELLOW);
                }
            } else {
                showMessage("[!] Lựa chọn không hợp lệ!", CLR_LIGHT_RED);
                totalQuestions--;
                q--;
            }
            
            pauseScreen();
        }
        
        delete[] shuffledAnswers;
        
        if (quitted) break;
    }

    delete[] cardArr;

    if (totalQuestions == 0) {
        showMessage("[i] Đã thoát trò chơi.", CLR_GRAY);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    // Hien thi ket qua
    clearScreen();
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("KẾT QUẢ TRÒ NỐI TỪ", INNER_W, CLR_LIGHT_YELLOW);
    drawBoxLineCentered("Chủ đề: " + selectedName, INNER_W, CLR_YELLOW);
    drawEmptyBoxLine(INNER_W);
    drawThinSeparator(INNER_W);
    drawEmptyBoxLine(INNER_W);

    string resultStr = "Điểm: " + to_string(totalCorrect) + " / " + to_string(totalQuestions);
    int pct = (totalQuestions > 0) ? (totalCorrect * 100 / totalQuestions) : 0;
    string pctStr = "Tỷ lệ đúng: " + to_string(pct) + "%";

    int resultColor = (pct >= 80) ? CLR_LIGHT_GREEN :
                      (pct >= 50) ? CLR_LIGHT_YELLOW : CLR_LIGHT_RED;

    drawBoxLineCentered(resultStr, INNER_W, resultColor);
    drawBoxLineCentered(pctStr,    INNER_W, resultColor);

    drawEmptyBoxLine(INNER_W);

    if (pct >= 80) {
        drawBoxLineCentered("Tuyệt vời! Bạn nối từ cực tốt!", INNER_W, CLR_LIGHT_GREEN);
    } else if (pct >= 50) {
        drawBoxLineCentered("Khá ổn! Làm thêm đi nào!", INNER_W, CLR_LIGHT_YELLOW);
    } else {
        drawBoxLineCentered("Cần ôn tập thêm nhé!", INNER_W, CLR_LIGHT_RED);
    }

    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);

    // Luu vao lich su
    string dateStr = getCurrentDate();
    addHistory(history, dateStr, selectedName + " (Nối từ)", totalCorrect, totalQuestions);
    saveHistory("History.txt", history);

    freeCardList(cards);
    pauseScreen();
  } // end while
}

// =====================================================================
//  7. TRO CHOI VIET TIENG ANH (REVERSE QUIZ)
// =====================================================================

void menuReverseQuiz(TopicNode* root, HistoryList& history) {
  while (true) {
    clearScreen();
    drawTitle();
    string selectedName;
    int sel = selectTopic(root, selectedName);
    if (sel == -1) return;

    // Load the
    CardList cards;
    initCardList(cards);
    loadCards(getCardFileName(selectedName), cards);

    if (cards.count == 0) {
        showMessage("[!] Chủ đề này chưa có Flash Card nào!", CLR_LIGHT_RED);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    int totalCards = cards.count;
    int correctCount = 0;

    CardNode* curr = cards.head;
    int idx = 0;
    bool quitted = false;

    while (curr) {
        idx++;
        clearScreen();
        drawTopBorder(INNER_W);
        drawEmptyBoxLine(INNER_W);

        string info = "VIẾT TIẾNG ANH - " + selectedName + "  [" + to_string(idx) + "/" + to_string(totalCards) + "]";
        drawBoxLineCentered(info, INNER_W, CLR_LIGHT_YELLOW);

        drawEmptyBoxLine(INNER_W);
        drawThinSeparator(INNER_W);
        drawEmptyBoxLine(INNER_W);
        drawBoxLineCentered(curr->vietnamese, INNER_W, CLR_BRIGHT_WHITE);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);

        cout << "\n";
        string answer = inputString("Viết từ tiếng Anh (0 = Thoát): ");

        if (answer == "0") {
            // Thoat giua chung
            totalCards = idx - 1;
            quitted = true;
            break;
        }

        if (compareAnswerFlexible(answer, curr->english)) {
            correctCount++;
            string feedback = getCorrectFeedback();
            showMessage("[OK] CHÍNH XÁC! " + feedback, CLR_LIGHT_GREEN);
        } else {
            string feedback = getWrongFeedback();
            showMessage("[X] SAI RỒI! " + feedback, CLR_LIGHT_RED);
            showMessage("     Đáp án đúng: " + curr->english, CLR_YELLOW);
        }

        pauseScreen();
        curr = curr->next;
    }

    if (quitted && totalCards == 0) {
        showMessage("[i] Đã thoát kiểm tra.", CLR_GRAY);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    // Hien thi ket qua
    clearScreen();
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("KẾT QUẢ VIẾT TIẾNG ANH", INNER_W, CLR_LIGHT_YELLOW);
    drawBoxLineCentered("Chủ đề: " + selectedName, INNER_W, CLR_YELLOW);
    drawEmptyBoxLine(INNER_W);
    drawThinSeparator(INNER_W);
    drawEmptyBoxLine(INNER_W);

    string resultStr = "Điểm: " + to_string(correctCount) + " / " + to_string(totalCards);
    int pct = (totalCards > 0) ? (correctCount * 100 / totalCards) : 0;
    string pctStr = "Tỷ lệ đúng: " + to_string(pct) + "%";

    int resultColor = (pct >= 80) ? CLR_LIGHT_GREEN :
                      (pct >= 50) ? CLR_LIGHT_YELLOW : CLR_LIGHT_RED;

    drawBoxLineCentered(resultStr, INNER_W, resultColor);
    drawBoxLineCentered(pctStr,    INNER_W, resultColor);

    drawEmptyBoxLine(INNER_W);

    if (pct >= 80) {
        drawBoxLineCentered("Xuất sắc! Tiếng Anh của bạn tốt đấy!", INNER_W, CLR_LIGHT_GREEN);
    } else if (pct >= 50) {
        drawBoxLineCentered("Khá tốt! Cần luyện thêm!", INNER_W, CLR_LIGHT_YELLOW);
    } else {
        drawBoxLineCentered("Cần ôn tập và nhớ chính tả!", INNER_W, CLR_LIGHT_RED);
    }

    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);

    // Luu vao lich su
    string dateStr = getCurrentDate();
    addHistory(history, dateStr, selectedName + " (Viết TA)", correctCount, totalCards);
    saveHistory("History.txt", history);

    freeCardList(cards);
    pauseScreen();
  } // end while
}
