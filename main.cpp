/*
 *  ================================================================
 *  FLASH CARD - Chuong Trinh Hoc Tu Vung Tieng Anh
 *  ================================================================
 *  
 *  Cau truc du lieu su dung:
 *    1. Danh sach lien ket doi (Doubly Linked List) - Quan ly Flash Card
 *    2. Cay nhi phan tim kiem (BST) - Quan ly chu de
 *    3. Hang doi (Queue) - Xu ly hoc va on tap
 *
 *  Bien dich (g++):
 *    g++ -o flashcard main.cpp flashcard.cpp ui.cpp -std=c++17 -finput-charset=UTF-8
 *
 *  Bien dich (MSVC):
 *    cl /EHsc /utf-8 main.cpp flashcard.cpp ui.cpp
 *
 *  Cac file du lieu:
 *    - Topics.txt              : Danh sach chu de
 *    - <Ten chu de>.txt        : Flash Card cua chu de
 *    - Wrong_<Ten chu de>.txt  : Tu chua nho / lam sai
 *    - History.txt             : Lich su kiem tra
 */

#include "ui.h"
#include <cstdlib>
#include <ctime>

int main() {
    // Khoi tao random seed
    srand((unsigned int)time(0));
    
    // Khoi tao giao dien console
    initConsole();

    // Khoi tao cau truc du lieu
    TopicNode* topicRoot = nullptr;
    HistoryList history;
    initHistory(history);

    // Doc du lieu tu file
    loadTopics("Topics.txt", topicRoot);
    loadHistory("History.txt", history);

    // Hien thi menu chinh
    showMainMenu(topicRoot, history);

    // Giai phong bo nho
    freeBST(topicRoot);
    freeHistory(history);

    return 0;
}
