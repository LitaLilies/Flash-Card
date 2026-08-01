/*
 *  FLASH CARD - Hoc Tu Vung Tieng Anh
 *  File: flashcard.h
 *  Mo ta: Khai bao cau truc du lieu va cac ham xu ly chinh
 *
 *  Cau truc du lieu su dung:
 *    1. Danh sach lien ket doi (Doubly Linked List) - Luu tru Flash Card trong moi chu de
 *    2. Cay nhi phan tim kiem (BST) - Quan ly danh sach chu de theo thu tu
 *    3. Hang doi (Queue) - Xu ly hoc va on tap Flash Card
 */

#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>
using std::string;

// =====================================================================
//  1. DOUBLY LINKED LIST - Luu tru Flash Card
//     Cho phep duyet tien/lui, them, xoa, sua the theo vi tri
// =====================================================================
struct CardNode {
    string english;
    string vietnamese;
    CardNode* prev;
    CardNode* next;
};

struct CardList {
    CardNode* head;
    CardNode* tail;
    int count;
};

// Khoi tao danh sach rong
void initCardList(CardList& list);
// Tao node moi
CardNode* createCardNode(const string& eng, const string& vie);
// Them the vao cuoi danh sach
void addCard(CardList& list, const string& eng, const string& vie);
// Xoa the theo STT (1-based)
void deleteCard(CardList& list, int index);
// Sua the theo STT (1-based)
void editCard(CardList& list, int index, const string& eng, const string& vie);
// Lay the theo STT (1-based)
CardNode* getCard(CardList& list, int index);
// Giai phong bo nho
void freeCardList(CardList& list);

// =====================================================================
//  2. QUEUE - Hoc va on tap Flash Card
//     FIFO: the chua nho duoc dua lai cuoi hang doi de on tap lai
// =====================================================================
struct QueueNode {
    string english;
    string vietnamese;
    QueueNode* next;
};

struct CardQueue {
    QueueNode* front;
    QueueNode* rear;
    int count;
};

void initQueue(CardQueue& q);
void enqueue(CardQueue& q, const string& eng, const string& vie);
QueueNode* dequeue(CardQueue& q);
bool isQueueEmpty(const CardQueue& q);
void freeQueue(CardQueue& q);

// =====================================================================
//  3. BST - Quan ly chu de (Topic)
//     Tim kiem nhanh theo ten, duyet in-order cho danh sach sap xep
// =====================================================================
struct TopicNode {
    string name;
    TopicNode* left;
    TopicNode* right;
};

TopicNode* createTopicNode(const string& name);
TopicNode* insertTopic(TopicNode* root, const string& name);
TopicNode* searchTopic(TopicNode* root, const string& name);
TopicNode* deleteTopic(TopicNode* root, const string& name);
TopicNode* findMinTopic(TopicNode* node);
// Duyet in-order, luu ten vao mang names[]
void inorderTopics(TopicNode* root, string names[], int& count);
int  countTopics(TopicNode* root);
void freeBST(TopicNode* root);

// =====================================================================
//  4. SINGLY LINKED LIST - Lich su kiem tra
// =====================================================================
struct HistoryNode {
    string date;
    string topic;
    int correct;
    int total;
    HistoryNode* next;
};

struct HistoryList {
    HistoryNode* head;
    int count;
};

void initHistory(HistoryList& list);
// Them vao dau (moi nhat o dau)
void addHistory(HistoryList& list, const string& date, const string& topic, int correct, int total);
// Xoa 1 muc theo STT (1-based)
void deleteHistory(HistoryList& list, int index);
// Xoa toan bo lich su
void clearHistory(HistoryList& list);
void freeHistory(HistoryList& list);

// =====================================================================
//  5. FILE I/O
// =====================================================================
void loadTopics(const string& filename, TopicNode*& root);
void saveTopics(const string& filename, TopicNode* root);

void loadCards(const string& filename, CardList& list);
void saveCards(const string& filename, CardList& list);

void loadHistory(const string& filename, HistoryList& list);
void saveHistory(const string& filename, HistoryList& list);

string getCardFileName(const string& topicName);
string getWrongFileName(const string& topicName);

// =====================================================================
//  6. UTILITY
// =====================================================================
string trim(const string& s);
string normalizeSpaces(const string& s);
bool   compareAnswer(const string& input, const string& answer);
string getCurrentDate();
bool   isValidTopicName(const string& name);
int    displayWidth(const string& s);

// =====================================================================
//  6.5. VIETNAMESE ACCENT REMOVAL - Bo dau tieng Viet
// =====================================================================
string removeVietnameseAccents(const string& s);
bool   compareAnswerFlexible(const string& input, const string& answer);

// =====================================================================
//  7. AI FEEDBACK - Loi khen / choc khi tra loi
// =====================================================================
string getCorrectFeedback();
string getWrongFeedback();

// =====================================================================
//  8. SHUFFLE - Tron ngau nhien cho tro choi
// =====================================================================
void shuffleCards(CardNode** arr, int n);

// =====================================================================
//  9. QUICK SORT - Sap xep Flash Card theo ten tieng Anh (A->Z)
//     Su dung de hien thi danh sach the theo thu tu alphabet
//     Pivot = phan tu cuoi, phan hoach Lomuto
// =====================================================================
// So sanh khong phan biet hoa/thuong va khong dau tieng Viet
bool cardLessOrEqual(CardNode* a, CardNode* b);
// Phan hoach mang [low..high], tra ve vi tri pivot sau phan hoach
int  partitionCards(CardNode** arr, int low, int high);
// De quy chinh: sap xep arr[low..high]
void quickSortCards(CardNode** arr, int low, int high);
// Ham tien ich: chuyen CardList -> mang, sort, tra ve mang da sap xep
// Goi xong phai delete[] ket qua
CardNode** cardListToSortedArray(CardList& list);

// =====================================================================
//  10. MERGE SORT - Sap xep lich su kiem tra theo diem % (cao -> thap)
//      Dam bao on dinh (stable): ket qua cung diem giu nguyen thu tu goc
// =====================================================================
// Gop 2 nua da sap xep cua mang arr[left..mid] va arr[mid+1..right]
void mergeHistory(HistoryNode** arr, int left, int mid, int right);
// De quy chinh
void mergeSortHistory(HistoryNode** arr, int left, int right);
// Ham tien ich: chuyen HistoryList -> mang, sort giam dan theo %, tra ve mang
// Goi xong phai delete[] ket qua
HistoryNode** historyToSortedArray(HistoryList& list);

#endif // FLASHCARD_H
