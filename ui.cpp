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
// =====================================================================
//  TIEU DE CHUONG TRINH
// =====================================================================

void drawTitle() {
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("FLASH CARD", INNER_W, CLR_LIGHT_YELLOW);
    drawBoxLineCentered("Chuong Trinh Hoc Tu Vung Tieng Anh", INNER_W, CLR_YELLOW);
    drawEmptyBoxLine(INNER_W);
}

// =====================================================================
//  NHAP LIEU & TIEN ICH
// =====================================================================

static void showCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(hOut, &ci);
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &ci);
}

static void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(hOut, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci);
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
    cout << "  Nhan phim bat ky de tiep tuc...";
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
        showMessage("[!] Chua co chu de nao. Hay tao chu de truoc!", CLR_LIGHT_RED);
        pauseScreen();
        return -1;
    }

    string* names = new string[n];
    int cnt = 0;
    inorderTopics(root, names, cnt);

    drawMidBorder(INNER_W);
    drawBoxLineCentered("DANH SACH CHU DE", INNER_W, CLR_LIGHT_YELLOW);
    drawThinSeparator(INNER_W);

    for (int i = 0; i < cnt; i++) {
        string line = "  [" + to_string(i + 1) + "]  " + names[i];
        drawBoxLine(line, INNER_W, CLR_WHITE);
    }
    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);

    cout << "\n";
    int choice = inputChoice("Chon chu de (1-" + to_string(cnt) + ", 0 = Quay lai): ");

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
        drawBoxLine("  [1]  Quan ly chu de (Deck)", INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [2]  Hoc Flash Card",        INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [3]  Kiem tra",               INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [4]  On tap",                 INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [5]  Lich su ket qua",        INNER_W, CLR_BRIGHT_WHITE);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [0]  Thoat",                  INNER_W, CLR_GRAY);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Nhap lua chon cua ban: ");

        switch (choice) {
            case 1: menuDeckManagement(root); break;
            case 2: menuLearnFlashCard(root);  break;
            case 3: menuQuiz(root, history);   break;
            case 4: menuReview(root);          break;
            case 5: menuHistory(history);      break;
            case 0:
                // Luu du lieu truoc khi thoat
                saveTopics("Topics.txt", root);
                saveHistory("History.txt", history);
                clearScreen();
                drawTopBorder(INNER_W);
                drawEmptyBoxLine(INNER_W);
                drawBoxLineCentered("Cam on ban da su dung Flash Card!", INNER_W, CLR_LIGHT_GREEN);
                drawBoxLineCentered("Hen gap lai!", INNER_W, CLR_YELLOW);
                drawEmptyBoxLine(INNER_W);
                drawBottomBorder(INNER_W);
                cout << "\n";
                resetColor();
                showCursor();
                return;
            default:
                showMessage("[!] Lua chon khong hop le!", CLR_LIGHT_RED);
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
        drawBoxLineCentered("QUAN LY CHU DE", INNER_W, CLR_LIGHT_CYAN);
        drawThinSeparator(INNER_W);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [1]  Tao chu de moi",      INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [2]  Doi ten chu de",       INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [3]  Quan ly Flash Card",   INNER_W, CLR_BRIGHT_WHITE);
        drawBoxLine("  [4]  Xoa chu de",           INNER_W, CLR_BRIGHT_WHITE);
        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [0]  Quay lai",             INNER_W, CLR_GRAY);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Nhap lua chon: ");

        switch (choice) {
            case 1: createDeck(root);      break;
            case 2: renameDeck(root);      break;
            case 3: manageCards(root);      break;
            case 4: deleteDeckMenu(root);  break;
            case 0: return;
            default:
                showMessage("[!] Lua chon khong hop le!", CLR_LIGHT_RED);
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
    drawBoxLineCentered("TAO CHU DE MOI", INNER_W, CLR_LIGHT_CYAN);
    drawEmptyBoxLine(INNER_W);
    drawBottomBorder(INNER_W);
    cout << "\n";

    string name = inputString("Nhap ten chu de (0 = Quay lai): ");

    if (name == "0") return;

    if (name.empty()) {
        showMessage("[!] Ten chu de khong duoc de trong!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    if (!isValidTopicName(name)) {
        showMessage("[!] Ten chu de chua ky tu khong hop le!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    if (searchTopic(root, name) != nullptr) {
        showMessage("[!] Chu de da ton tai!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    root = insertTopic(root, name);

    // Tao file rong cho chu de
    string cardFile = getCardFileName(name);
    ofstream f(cardFile);
    f.close();

    saveTopics("Topics.txt", root);
    showMessage("[OK] Da tao chu de: " + name, CLR_LIGHT_GREEN);

    // Hoi them Flash Card ngay
    cout << "\n";
    int ans = inputChoice("Ban co muon them Flash Card ngay? (1 = Co, 0 = Khong): ");
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
    cout << "  Nhap Flash Card (nhap 0 de ket thuc):\n";
    resetColor();

    while (true) {
        cout << "\n";
        string eng = inputString("  Tu tieng Anh (0 = Xong): ");
        if (eng == "0" || eng.empty()) break;

        string vie = inputString("  Nghia tieng Viet: ");
        if (vie == "0" || vie.empty()) {
            showMessage("[!] Nghia tieng Viet khong duoc de trong!", CLR_LIGHT_RED);
            continue;
        }

        addCard(cards, eng, vie);
        showMessage("  [+] Da them: " + eng + " - " + vie, CLR_GREEN);
    }

    saveCards(getCardFileName(topicName), cards);
    showMessage("[OK] Da luu " + to_string(cards.count) + " Flash Card.", CLR_LIGHT_GREEN);
    freeCardList(cards);
    pauseScreen();
}

// --- 1.2 Doi ten chu de ---

void renameDeck(TopicNode*& root) {
    clearScreen();
    drawTitle();
    drawMidBorder(INNER_W);
    drawBoxLineCentered("DOI TEN CHU DE", INNER_W, CLR_LIGHT_CYAN);
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
    cout << "  Chu de hien tai: " << selectedName << "\n";
    resetColor();

    string newName = inputString("Nhap ten moi: ");

    if (newName.empty() || !isValidTopicName(newName)) {
        showMessage("[!] Ten moi khong hop le!", CLR_LIGHT_RED);
        pauseScreen();
        return;
    }

    if (searchTopic(root, newName) != nullptr) {
        showMessage("[!] Ten chu de da ton tai!", CLR_LIGHT_RED);
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

    showMessage("[OK] Da doi ten: " + selectedName + " -> " + newName, CLR_LIGHT_GREEN);
    pauseScreen();
}

// --- 1.3 Quan ly Flash Card ---

static void displayCardList(CardList& cards) {
    if (cards.count == 0) {
        drawBoxLineCentered("(Chua co Flash Card nao)", INNER_W, CLR_GRAY);
        return;
    }

    // Tieu de bang
    string header = "  STT | Tieng Anh              | Tieng Viet";
    drawBoxLine(header, INNER_W, CLR_LIGHT_YELLOW);
    drawThinSeparator(INNER_W);

    CardNode* curr = cards.head;
    int idx = 1;
    while (curr) {
        // Dinh dang dong
        string stt = to_string(idx);
        while ((int)stt.length() < 3) stt = " " + stt;

        string eng = curr->english;
        string vie = curr->vietnamese;

        // Cat bot neu qua dai
        if (displayWidth(eng) > 22) eng = eng.substr(0, 20) + "..";
        if (displayWidth(vie) > 22) vie = vie.substr(0, 20) + "..";

        // Padding
        string engPad = eng;
        int engW = displayWidth(eng);
        for (int i = engW; i < 22; i++) engPad += " ";

        string viePad = vie;
        int vieW = displayWidth(vie);
        for (int i = vieW; i < 22; i++) viePad += " ";

        string line = "  " + stt + " | " + engPad + " | " + viePad;
        drawBoxLine(line, INNER_W, CLR_WHITE);

        curr = curr->next;
        idx++;
    }
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
        drawBoxLineCentered("QUAN LY FLASH CARD", INNER_W, CLR_LIGHT_CYAN);
        drawBoxLineCentered("Chu de: " + selectedName, INNER_W, CLR_YELLOW);
        drawThinSeparator(INNER_W);

        // Load va hien thi danh sach
        CardList cards;
        initCardList(cards);
        loadCards(getCardFileName(selectedName), cards);

        displayCardList(cards);

        drawEmptyBoxLine(INNER_W);
        drawBoxLine("  [1] Them  [2] Sua  [3] Xoa  [4] Xem the  [0] Quay lai", INNER_W, CLR_LIGHT_GREEN);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Nhap lua chon: ");

        if (choice == 0) {
            freeCardList(cards);
            return;
        }

        if (choice == 4) {
            // Xem the - duyet toi/lui su dung DSLK kep (prev/next)
            if (cards.count == 0) {
                showMessage("[!] Khong co the nao de xem!", CLR_LIGHT_RED);
            } else {
                CardNode* current = cards.head;
                int pos = 1;
                bool browsing = true;
                while (browsing) {
                    clearScreen();
                    drawTopBorder(INNER_W);
                    drawEmptyBoxLine(INNER_W);
                    string info = "XEM THE  [" + to_string(pos) + "/" + to_string(cards.count) + "]";
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
                    if (current->prev) nav += "[<-] Lui  ";
                    if (current->next) nav += "[->] Toi  ";
                    nav += "[ESC] Thoat";
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
                showMessage("[!] Khong co the nao de sua!", CLR_LIGHT_RED);
                freeCardList(cards);
                pauseScreen();
                continue;
            }
            int idx = inputChoice("Nhap STT can sua (1-" + to_string(cards.count) + "): ");
            CardNode* node = getCard(cards, idx);
            if (node == nullptr) {
                showMessage("[!] STT khong hop le!", CLR_LIGHT_RED);
            } else {
                cout << "\n";
                setColor(CLR_YELLOW);
                leftPad();
                cout << "  Hien tai: " << node->english << " - " << node->vietnamese << "\n";
                resetColor();

                string newEng = inputString("  Tu tieng Anh moi: ");
                string newVie = inputString("  Nghia tieng Viet moi: ");

                if (!newEng.empty() && !newVie.empty()) {
                    editCard(cards, idx, newEng, newVie);
                    saveCards(getCardFileName(selectedName), cards);
                    showMessage("[OK] Da cap nhat Flash Card!", CLR_LIGHT_GREEN);
                } else {
                    showMessage("[!] Khong duoc de trong!", CLR_LIGHT_RED);
                }
            }
            freeCardList(cards);
            pauseScreen();
        }
        else if (choice == 3) {
            // Xoa
            if (cards.count == 0) {
                showMessage("[!] Khong co the nao de xoa!", CLR_LIGHT_RED);
                freeCardList(cards);
                pauseScreen();
                continue;
            }
            int idx = inputChoice("Nhap STT can xoa (1-" + to_string(cards.count) + "): ");
            CardNode* node = getCard(cards, idx);
            if (node == nullptr) {
                showMessage("[!] STT khong hop le!", CLR_LIGHT_RED);
            } else {
                cout << "\n";
                setColor(CLR_YELLOW);
                leftPad();
                cout << "  Se xoa: " << node->english << " - " << node->vietnamese << "\n";
                resetColor();

                int confirm = inputChoice("  Xac nhan xoa? (1 = Co, 0 = Khong): ");
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
                    showMessage("[OK] Da xoa Flash Card!", CLR_LIGHT_GREEN);
                } else {
                    showMessage("[i] Da huy xoa.", CLR_GRAY);
                }
            }
            freeCardList(cards);
            pauseScreen();
        }
        else {
            freeCardList(cards);
            showMessage("[!] Lua chon khong hop le!", CLR_LIGHT_RED);
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
    cout << "  CANH BAO: Se xoa chu de \"" << selectedName << "\" va tat ca du lieu lien quan!\n";
    resetColor();

    int confirm = inputChoice("Xac nhan xoa? (1 = Co, 0 = Khong): ");
    if (confirm != 1) {
        showMessage("[i] Da huy xoa.", CLR_GRAY);
        pauseScreen();
        return;
    }

    // Xoa file
    remove(getCardFileName(selectedName).c_str());
    remove(getWrongFileName(selectedName).c_str());

    // Xoa khoi BST
    root = deleteTopic(root, selectedName);
    saveTopics("Topics.txt", root);

    showMessage("[OK] Da xoa chu de: " + selectedName, CLR_LIGHT_GREEN);
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
        drawBoxLineCentered("[Nhan ENTER de lat the]", INNER_W, CLR_GRAY);
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
        showMessage("[!] Chu de nay chua co Flash Card nao!", CLR_LIGHT_RED);
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
        drawFlashCardUI("HOC FLASH CARD - " + selectedName,
                        cardNum, totalCards,
                        qn->english, qn->vietnamese, false);
        drawBottomBorder(INNER_W);
        _getch(); // Doi nhan phim de lat the

        // Hien thi ca 2 mat
        drawFlashCardUI("HOC FLASH CARD - " + selectedName,
                        cardNum, totalCards,
                        qn->english, qn->vietnamese, true);

        drawMidBorder(INNER_W);
        drawBoxLine("  [1] Nho     [2] Chua nho     [0] Thoat", INNER_W, CLR_LIGHT_CYAN);
        drawBottomBorder(INNER_W);
        cout << "\n";

        int choice = inputChoice("Lua chon cua ban: ");

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

        showMessage("[i] Co " + to_string(wrongCards.count) + " tu chua nho da luu vao on tap.",
                    CLR_LIGHT_YELLOW);
    } else {
        showMessage("[OK] Tuyet voi! Ban da nho tat ca tu vung!", CLR_LIGHT_GREEN);
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
        showMessage("[!] Chu de nay chua co Flash Card nao!", CLR_LIGHT_RED);
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

        string info = "KIEM TRA - " + selectedName + "  [" + to_string(idx) + "/" + to_string(totalCards) + "]";
        drawBoxLineCentered(info, INNER_W, CLR_LIGHT_YELLOW);

        drawEmptyBoxLine(INNER_W);
        drawThinSeparator(INNER_W);
        drawEmptyBoxLine(INNER_W);
        drawBoxLineCentered(curr->english, INNER_W, CLR_BRIGHT_WHITE);
        drawEmptyBoxLine(INNER_W);
        drawBottomBorder(INNER_W);

        cout << "\n";
        string answer = inputString("Nhap nghia tieng Viet (0 = Thoat): ");

        if (answer == "0") {
            // Thoat giua chung - tinh so cau da lam
            totalCards = idx - 1;
            quitted = true;
            break;
        }

        if (compareAnswer(answer, curr->vietnamese)) {
            correctCount++;
            showMessage("[OK] DUNG ROI!", CLR_LIGHT_GREEN);
        } else {
            showMessage("[X] SAI! Dap an dung: " + curr->vietnamese, CLR_LIGHT_RED);
            addCard(wrongCards, curr->english, curr->vietnamese);
        }

        pauseScreen();
        curr = curr->next;
    }

    if (quitted && totalCards == 0) {
        showMessage("[i] Da thoat kiem tra.", CLR_GRAY);
        freeCardList(wrongCards);
        freeCardList(cards);
        pauseScreen();
        return;
    }

    // Hien thi ket qua
    clearScreen();
    drawTopBorder(INNER_W);
    drawEmptyBoxLine(INNER_W);
    drawBoxLineCentered("KET QUA KIEM TRA", INNER_W, CLR_LIGHT_YELLOW);
    drawBoxLineCentered("Chu de: " + selectedName, INNER_W, CLR_YELLOW);
    drawEmptyBoxLine(INNER_W);
    drawThinSeparator(INNER_W);
    drawEmptyBoxLine(INNER_W);

    string resultStr = "Diem: " + to_string(correctCount) + " / " + to_string(totalCards);
    int pct = (totalCards > 0) ? (correctCount * 100 / totalCards) : 0;
    string pctStr = "Ty le dung: " + to_string(pct) + "%";

    int resultColor = (pct >= 80) ? CLR_LIGHT_GREEN :
                      (pct >= 50) ? CLR_LIGHT_YELLOW : CLR_LIGHT_RED;

    drawBoxLineCentered(resultStr, INNER_W, resultColor);
    drawBoxLineCentered(pctStr,    INNER_W, resultColor);

    drawEmptyBoxLine(INNER_W);

    if (pct >= 80) {
        drawBoxLineCentered("Xuat sac!", INNER_W, CLR_LIGHT_GREEN);
    } else if (pct >= 50) {
        drawBoxLineCentered("Kha tot! Can on tap them!", INNER_W, CLR_LIGHT_YELLOW);
    } else {
        drawBoxLineCentered("Can co gang hon! Hay on tap lai!", INNER_W, CLR_LIGHT_RED);
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

