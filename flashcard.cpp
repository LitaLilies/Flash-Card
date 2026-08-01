/*
 *  FLASH CARD - Hoc Tu Vung Tieng Anh
 *  File: flashcard.cpp
 *  Mo ta: Cai dat cac cau truc du lieu va ham xu ly
 */

#include "flashcard.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <cwctype>
using namespace std;

// =====================================================================
//  DOUBLY LINKED LIST - CardList
// =====================================================================

void initCardList(CardList& list) {
    list.head = nullptr;
    list.tail = nullptr;
    list.count = 0;
}

CardNode* createCardNode(const string& eng, const string& vie) {
    CardNode* node = new CardNode;
    node->english = eng;
    node->vietnamese = vie;
    node->prev = nullptr;
    node->next = nullptr;
    return node;
}

void addCard(CardList& list, const string& eng, const string& vie) {
    CardNode* node = createCardNode(eng, vie);
    if (list.head == nullptr) {
        list.head = list.tail = node;
    } else {
        list.tail->next = node;
        node->prev = list.tail;
        list.tail = node;
    }
    list.count++;
}

void deleteCard(CardList& list, int index) {
    if (index < 1 || index > list.count) return;

    CardNode* curr = list.head;
    for (int i = 1; i < index; i++)
        curr = curr->next;

    if (curr->prev)
        curr->prev->next = curr->next;
    else
        list.head = curr->next;

    if (curr->next)
        curr->next->prev = curr->prev;
    else
        list.tail = curr->prev;

    delete curr;
    list.count--;
}

void editCard(CardList& list, int index, const string& eng, const string& vie) {
    CardNode* node = getCard(list, index);
    if (node) {
        node->english = eng;
        node->vietnamese = vie;
    }
}

CardNode* getCard(CardList& list, int index) {
    if (index < 1 || index > list.count) return nullptr;
    CardNode* curr = list.head;
    for (int i = 1; i < index; i++)
        curr = curr->next;
    return curr;
}

void freeCardList(CardList& list) {
    CardNode* curr = list.head;
    while (curr) {
        CardNode* next = curr->next;
        delete curr;
        curr = next;
    }
    list.head = list.tail = nullptr;
    list.count = 0;
}

// =====================================================================
//  QUEUE - CardQueue
// =====================================================================

void initQueue(CardQueue& q) {
    q.front = nullptr;
    q.rear = nullptr;
    q.count = 0;
}

void enqueue(CardQueue& q, const string& eng, const string& vie) {
    QueueNode* node = new QueueNode;
    node->english = eng;
    node->vietnamese = vie;
    node->next = nullptr;

    if (q.rear == nullptr) {
        q.front = q.rear = node;
    } else {
        q.rear->next = node;
        q.rear = node;
    }
    q.count++;
}

QueueNode* dequeue(CardQueue& q) {
    if (q.front == nullptr) return nullptr;
    QueueNode* node = q.front;
    q.front = q.front->next;
    if (q.front == nullptr) q.rear = nullptr;
    q.count--;
    node->next = nullptr;
    return node;
}

bool isQueueEmpty(const CardQueue& q) {
    return q.front == nullptr;
}

void freeQueue(CardQueue& q) {
    while (!isQueueEmpty(q)) {
        QueueNode* node = dequeue(q);
        delete node;
    }
}

// =====================================================================
//  BST - TopicNode
// =====================================================================

TopicNode* createTopicNode(const string& name) {
    TopicNode* node = new TopicNode;
    node->name = name;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

TopicNode* insertTopic(TopicNode* root, const string& name) {
    if (root == nullptr) return createTopicNode(name);
    if (name < root->name)
        root->left = insertTopic(root->left, name);
    else if (name > root->name)
        root->right = insertTopic(root->right, name);
    // Trung ten thi khong them
    return root;
}

TopicNode* searchTopic(TopicNode* root, const string& name) {
    if (root == nullptr || root->name == name)
        return root;
    if (name < root->name)
        return searchTopic(root->left, name);
    else
        return searchTopic(root->right, name);
}

TopicNode* findMinTopic(TopicNode* node) {
    while (node && node->left)
        node = node->left;
    return node;
}

TopicNode* deleteTopic(TopicNode* root, const string& name) {
    if (root == nullptr) return nullptr;

    if (name < root->name)
        root->left = deleteTopic(root->left, name);
    else if (name > root->name)
        root->right = deleteTopic(root->right, name);
    else {
        // Tim thay node can xoa
        if (root->left == nullptr) {
            TopicNode* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == nullptr) {
            TopicNode* temp = root->left;
            delete root;
            return temp;
        }
        // Co 2 con: thay bang node nho nhat cua cay con phai
        TopicNode* temp = findMinTopic(root->right);
        root->name = temp->name;
        root->right = deleteTopic(root->right, temp->name);
    }
    return root;
}

void inorderTopics(TopicNode* root, string names[], int& count) {
    if (root == nullptr) return;
    inorderTopics(root->left, names, count);
    names[count++] = root->name;
    inorderTopics(root->right, names, count);
}

int countTopics(TopicNode* root) {
    if (root == nullptr) return 0;
    return 1 + countTopics(root->left) + countTopics(root->right);
}

void freeBST(TopicNode* root) {
    if (root == nullptr) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}

// =====================================================================
//  HISTORY - Singly Linked List
// =====================================================================

void initHistory(HistoryList& list) {
    list.head = nullptr;
    list.count = 0;
}

void addHistory(HistoryList& list, const string& date, const string& topic, int correct, int total) {
    HistoryNode* node = new HistoryNode;
    node->date = date;
    node->topic = topic;
    node->correct = correct;
    node->total = total;
    node->next = list.head;
    list.head = node;
    list.count++;
}

void deleteHistory(HistoryList& list, int index) {
    if (index < 1 || index > list.count) return;

    if (index == 1) {
        HistoryNode* temp = list.head;
        list.head = list.head->next;
        delete temp;
    } else {
        HistoryNode* prev = list.head;
        for (int i = 1; i < index - 1; i++)
            prev = prev->next;
        HistoryNode* temp = prev->next;
        prev->next = temp->next;
        delete temp;
    }
    list.count--;
}

void clearHistory(HistoryList& list) {
    freeHistory(list);
}

void freeHistory(HistoryList& list) {
    HistoryNode* curr = list.head;
    while (curr) {
        HistoryNode* next = curr->next;
        delete curr;
        curr = next;
    }
    list.head = nullptr;
    list.count = 0;
}

// =====================================================================
//  FILE I/O
// =====================================================================

string getCardFileName(const string& topicName) {
    return topicName + ".txt";
}

string getWrongFileName(const string& topicName) {
    return "Wrong_" + topicName + ".txt";
}

// --- Topics ---

static void saveTopicsHelper(ofstream& f, TopicNode* root) {
    if (root == nullptr) return;
    saveTopicsHelper(f, root->left);
    f << root->name << "\n";
    saveTopicsHelper(f, root->right);
}

void loadTopics(const string& filename, TopicNode*& root) {
    ifstream f(filename);
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (!line.empty())
            root = insertTopic(root, line);
    }
    f.close();
}

void saveTopics(const string& filename, TopicNode* root) {
    ofstream f(filename);
    if (!f.is_open()) return;
    saveTopicsHelper(f, root);
    f.close();
}

// --- Cards ---

void loadCards(const string& filename, CardList& list) {
    ifstream f(filename);
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        size_t pos = line.find('|');
        if (pos != string::npos) {
            string eng = trim(line.substr(0, pos));
            string vie = trim(line.substr(pos + 1));
            if (!eng.empty())
                addCard(list, eng, vie);
        }
    }
    f.close();
}

void saveCards(const string& filename, CardList& list) {
    ofstream f(filename);
    if (!f.is_open()) return;
    CardNode* curr = list.head;
    while (curr) {
        f << curr->english << "|" << curr->vietnamese << "\n";
        curr = curr->next;
    }
    f.close();
}

// --- History ---

// Luu theo thu tu: duyet nguoc (cu nhat truoc, moi nhat sau)
static void saveHistoryReverse(ofstream& f, HistoryNode* node) {
    if (node == nullptr) return;
    saveHistoryReverse(f, node->next);
    f << node->date << "|" << node->topic << "|" << node->correct << "|" << node->total << "\n";
}

void loadHistory(const string& filename, HistoryList& list) {
    ifstream f(filename);
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string date, topic, correctStr, totalStr;
        getline(ss, date, '|');
        getline(ss, topic, '|');
        getline(ss, correctStr, '|');
        getline(ss, totalStr, '|');

        if (!date.empty() && !topic.empty()) {
            int correct = atoi(correctStr.c_str());
            int total   = atoi(totalStr.c_str());
            addHistory(list, trim(date), trim(topic), correct, total);
        }
    }
    f.close();
}

void saveHistory(const string& filename, HistoryList& list) {
    ofstream f(filename);
    if (!f.is_open()) return;
    saveHistoryReverse(f, list.head);
    f.close();
}

// =====================================================================
//  UTILITY
// =====================================================================

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string normalizeSpaces(const string& s) {
    string result;
    bool lastSpace = true;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == ' ' || s[i] == '\t') {
            if (!lastSpace) {
                result += ' ';
                lastSpace = true;
            }
        } else {
            result += s[i];
            lastSpace = false;
        }
    }
    if (!result.empty() && result.back() == ' ')
        result.pop_back();
    return result;
}

static wstring utf8ToWide(const string& s) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(s);
}

static string wideToUtf8(const wstring& s) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(s);
}

static wstring toLowerWide(const wstring& s) {
    wstring result = s;
    transform(result.begin(), result.end(), result.begin(), [](wchar_t ch) {
        return static_cast<wchar_t>(std::towlower(ch));
    });
    return result;
}

bool compareAnswer(const string& input, const string& answer) {
    string a = normalizeSpaces(trim(input));
    string b = normalizeSpaces(trim(answer));

    if (a.empty() && b.empty()) return true;
    if (a.empty() || b.empty()) return false;

    wstring wa = toLowerWide(utf8ToWide(a));
    wstring wb = toLowerWide(utf8ToWide(b));
    return wa == wb;
}

string getCurrentDate() {
    time_t now = time(0);
    struct tm* ts = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", ts);
    return string(buf);
}

bool isValidTopicName(const string& name) {
    if (name.empty()) return false;
    string invalid = "\\/:*?\"<>|";
    for (size_t i = 0; i < name.length(); i++) {
        if (invalid.find(name[i]) != string::npos)
            return false;
    }
    return true;
}

int displayWidth(const string& s) {
    int width = 0;
    for (size_t i = 0; i < s.length(); ) {
        unsigned char c = (unsigned char)s[i];
        if (c < 0x80)       { width++; i += 1; }
        else if (c < 0xE0)  { width++; i += 2; }
        else if (c < 0xF0)  { width++; i += 3; }
        else                 { width += 2; i += 4; }
    }
    return width;
}

// =====================================================================
//  VIETNAMESE ACCENT REMOVAL - Bo dau tieng Viet
// =====================================================================

// Bang chuyen doi Unicode tieng Viet co dau -> khong dau
static const wchar_t* VIET_ACCENTS[] = {
    L"áàảãạăắằẳẵặâấầẩẫậ", L"a",
    L"ÁÀẢÃẠĂẮẰẲẴẶÂẤẦẨẪẬ", L"A",
    L"éèẻẽẹêếềểễệ", L"e",
    L"ÉÈẺẼẸÊẾỀỂỄỆ", L"E",
    L"íìỉĩị", L"i",
    L"ÍÌỈĨỊ", L"I",
    L"óòỏõọôốồổỗộơớờởỡợ", L"o",
    L"ÓÒỎÕỌÔỐỒỔỖỘƠỚỜỞỠỢ", L"O",
    L"úùủũụưứừửữự", L"u",
    L"ÚÙỦŨỤƯỨỪỬỮỰ", L"U",
    L"ýỳỷỹỵ", L"y",
    L"ÝỲỶỸỴ", L"Y",
    L"đ", L"d",
    L"Đ", L"D",
    nullptr, nullptr
};

string removeVietnameseAccents(const string& s) {
    if (s.empty()) return "";

    wstring inputWide = utf8ToWide(s);
    wstring result;
    result.reserve(inputWide.size());

    for (wchar_t ch : inputWide) {
        bool found = false;
        for (int j = 0; VIET_ACCENTS[j] != nullptr; j += 2) {
            const wchar_t* accents = VIET_ACCENTS[j];
            const wchar_t* replacement = VIET_ACCENTS[j + 1];
            for (int k = 0; accents[k] != L'\0'; ++k) {
                if (ch == accents[k]) {
                    if (replacement != nullptr && replacement[0] != L'\0') {
                        result += replacement[0];
                    }
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            result += ch;
        }
    }

    return wideToUtf8(result);
}

// So sanh linh hoat: chap nhan ca co dau va khong dau
bool compareAnswerFlexible(const string& input, const string& answer) {
    if (compareAnswer(input, answer)) return true;

    string inputNoAccent = removeVietnameseAccents(normalizeSpaces(trim(input)));
    string answerNoAccent = removeVietnameseAccents(normalizeSpaces(trim(answer)));
    return compareAnswer(inputNoAccent, answerNoAccent);
}

// =====================================================================
//  AI FEEDBACK - Loi khen / choc
// =====================================================================

string getCorrectFeedback() {
    static const string messages[] = {
        "Tuyet voi! Ban thong minh qua!",
        "Xuat sac! Tiep tuc nhu vay!",
        "Chinh xac! Ban dang lam rat tot!",
        "Perfect! Ban la thien tai!",
        "Dung roi! Tri oc cua ban qua nhon ben!",
        "Wow! Ban nho ma nhanh the!",
        "Gioi lam! Keep it up!",
        "Amazing! Ban dang on fire day!",
        "That's right! Ban qua dang cap!",
        "Brilliant! IQ 200 day!",
        "Dung 100%! Ban la sieu nhan!",
        "Excellent! Bo nao ban hoat dong tot qua!",
        "Chinh xac tuyet doi! Ban la hien tuong!",
        "Hay lam! Ban sinh ra de lam dieu nay!",
        "Outstanding! Einstein phai ganh ti!",
        "Hoan hao! Ban la huyen thoai!",
        "Incredible! Khong ai lam tot bang ban!",
        "Spot on! Ban dang bung no day!",
        "Magnificient! Tu dien song la day!",
        "Flawless! Ban la bac thay!"
    };
    int count = sizeof(messages) / sizeof(messages[0]);
    int idx = rand() % count;
    return messages[idx];
}

string getWrongFeedback() {
    static const string messages[] = {
        "Oops! Bo nao can ngu them day!",
        "Sai roi! On lai di ban ei!",
        "Chua dung! Doc sach them di!",
        "Khong phai roi! Tap trung len nao!",
        "Sai be bet! Can co gang hon!",
        "Tro lai lop 1 di! Hehe!",
        "Not quite! Bo nao dang ngu say roi!",
        "Nope! Hay thu suy nghi them!",
        "Sai mat ti roi! Dung buon, thu lai!",
        "Wrong! IQ hom nay thap the nhi?",
        "Chua chuan! Tap trung vao nao!",
        "Sai cmnr! On bai di ban!",
        "Try again! Ban dang ngu a?",
        "That's wrong! Doc ky hon di!",
        "Incorrect! Tra google di ban!",
        "Sai roi ban oi! Thuc day di!",
        "Not even close! Hoc bai chua day!",
        "Nah! Bo nao ban di nghi phe roi!",
        "False! Can uong ca phe khong day?",
        "Missed! Tap trung len si quan!"
    };
    int count = sizeof(messages) / sizeof(messages[0]);
    int idx = rand() % count;
    return messages[idx];
}

// =====================================================================
//  SHUFFLE - Tron ngau nhien mang (Fisher-Yates)
// =====================================================================

void shuffleCards(CardNode** arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        CardNode* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// =====================================================================
//  QUICK SORT - Sap xep Flash Card theo ten tieng Anh (A -> Z)
//
//  Thuat toan:
//    - Chon pivot la phan tu cuoi (phan hoach Lomuto)
//    - Chia mang thanh 2 phan: <= pivot va > pivot
//    - De quy sap xep tung phan
//    - Do phuc tap: O(n log n) trung binh, O(n^2) xau nhat
// =====================================================================

// So sanh 2 CardNode theo english, khong phan biet hoa/thuong
bool cardLessOrEqual(CardNode* a, CardNode* b) {
    // Chuyen ve chu thuong de so sanh
    string sa = a->english;
    string sb = b->english;
    for (size_t i = 0; i < sa.size(); i++)
        if (sa[i] >= 'A' && sa[i] <= 'Z') sa[i] += 32;
    for (size_t i = 0; i < sb.size(); i++)
        if (sb[i] >= 'A' && sb[i] <= 'Z') sb[i] += 32;
    return sa <= sb;
}

// Phan hoach Lomuto: tra ve vi tri cuoi cung cua pivot
int partitionCards(CardNode** arr, int low, int high) {
    CardNode* pivot = arr[high];   // Chon pivot la phan tu cuoi
    int i = low - 1;               // i tro den phan tu cuoi cua vung <= pivot
    for (int j = low; j < high; j++) {
        if (cardLessOrEqual(arr[j], pivot)) {
            i++;
            // Hoan vi arr[i] va arr[j]
            CardNode* tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    // Dat pivot vao dung vi tri
    CardNode* tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return i + 1;
}

// Ham de quy Quick Sort
void quickSortCards(CardNode** arr, int low, int high) {
    if (low < high) {
        int pi = partitionCards(arr, low, high);  // Vi tri pivot sau phan hoach
        quickSortCards(arr, low, pi - 1);         // Sap xep phan trai
        quickSortCards(arr, pi + 1, high);        // Sap xep phan phai
    }
}

// Ham tien ich: chuyen CardList sang mang con tro, ap dung Quick Sort
// Tra ve mang da sap xep (caller phai delete[])
CardNode** cardListToSortedArray(CardList& list) {
    if (list.count == 0) return nullptr;
    CardNode** arr = new CardNode*[list.count];
    CardNode* curr = list.head;
    for (int i = 0; i < list.count; i++) {
        arr[i] = curr;
        curr = curr->next;
    }
    quickSortCards(arr, 0, list.count - 1);
    return arr;
}

// =====================================================================
//  MERGE SORT - Sap xep lich su kiem tra theo diem % (cao -> thap)
//
//  Thuat toan:
//    - Chia doi mang lien tuc cho den khi con 1 phan tu
//    - Gop (merge) 2 nua da sap xep thanh 1 mang sap xep
//    - On dinh (stable): ket qua cung diem giu nguyen thu tu nhap vao
//    - Do phuc tap: O(n log n) moi truong hop, ton O(n) bo nho phu
// =====================================================================

// Gop 2 nua arr[left..mid] va arr[mid+1..right] da duoc sap xep giam dan
void mergeHistory(HistoryNode** arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Tao 2 mang tam
    HistoryNode** L = new HistoryNode*[n1];
    HistoryNode** R = new HistoryNode*[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Gop: lay phan tu lon hon truoc (giam dan theo %)
    while (i < n1 && j < n2) {
        int pctL = (L[i]->total > 0) ? (L[i]->correct * 100 / L[i]->total) : 0;
        int pctR = (R[j]->total > 0) ? (R[j]->correct * 100 / R[j]->total) : 0;
        if (pctL >= pctR)
            arr[k++] = L[i++];   // Giu tinh on dinh: bang nhau uu tien nua trai
        else
            arr[k++] = R[j++];
    }
    // Sao chep phan con lai
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Ham de quy Merge Sort
void mergeSortHistory(HistoryNode** arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;   // Tranh tran so khi left+right lon
        mergeSortHistory(arr, left, mid);       // Sap xep nua trai
        mergeSortHistory(arr, mid + 1, right);  // Sap xep nua phai
        mergeHistory(arr, left, mid, right);    // Gop 2 nua
    }
}

// Ham tien ich: chuyen HistoryList sang mang, ap dung Merge Sort giam dan theo %
// Tra ve mang da sap xep (caller phai delete[])
HistoryNode** historyToSortedArray(HistoryList& list) {
    if (list.count == 0) return nullptr;
    HistoryNode** arr = new HistoryNode*[list.count];
    HistoryNode* curr = list.head;
    for (int i = 0; i < list.count; i++) {
        arr[i] = curr;
        curr = curr->next;
    }
    mergeSortHistory(arr, 0, list.count - 1);
    return arr;
}
