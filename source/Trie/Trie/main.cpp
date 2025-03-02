#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>


#define ALPHABET_SIZE 26

using namespace std;

// Node trong Trie
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];  
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

// Ham clear man hinh sau moi lan chay chuc nang
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Ham in chuoi
void showMenu(const string& text) {
    int width = 60;
    int padding = (width - text.length()) / 2;
    cout << setw(padding) << "" << text << endl;
}

// In khung tieu de
void printHeader(const string& title) {
    int width = 60;
    cout << "\033[1;32m" << string(width, '*') << "\033[0m" << endl;
    showMenu("\033[1;34m" + title + "\033[0m");
    cout << "\033[1;32m" << string(width, '*') << "\033[0m" << endl;
}


//Them tu khoa
void insert(TrieNode* root, const string& word) {
    if (word.length() < 3) {
        cout << "Tu '" << word << "' qua ngan. Chi chap nhan tu co 3 chu cai tro len.\n";
        return;
    }

    TrieNode* node = root;
    for (char ch : word) {
        int index = ch - 'a';
        if (node->children[index] == nullptr) {
            node->children[index] = new TrieNode();
        }
        node = node->children[index];
    }
    node->isEndOfWord = true;
}

// Tim tu khoa
TrieNode* searchPrefix(TrieNode* root, const string& prefix) {
    TrieNode* node = root;
    for (char ch : prefix) {
        int index = ch - 'a';
        if (node->children[index] == nullptr) {
            return nullptr;
        }
        node = node->children[index];
    }
    return node;
}

// In cac tu khoa duoc suggest
void suggestWords(TrieNode* root, string currentPrefix, int& t) {
    if (t > 0) {
        if (root->isEndOfWord) {
            t--;
            cout << currentPrefix << endl;
        }
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (root->children[i] != nullptr) {
                suggestWords(root->children[i], currentPrefix + char(i + 'a'), t);
            }
        }
    }
}

// Goi y tu khoa
void autocomplete(TrieNode* root, const string& prefix) {
    TrieNode* node = searchPrefix(root, prefix);
    if (node == nullptr) {
        cout << "KHONG TIM THAY TU KHOA VOI TIEN TO: " << prefix << endl;
        return;
    }
    cout << "TU KHOA GOI Y:\n";
    int t = 7; //dat so luong goi y toi da la 7
    suggestWords(node, prefix, t);
}


//Ham check xem node co duoc xoa khong
bool shouldDeleteNode(TrieNode* node) {
    bool isNotEndOfWord = !node->isEndOfWord;
    bool hasNoChildren = true;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i] != nullptr) {
            hasNoChildren = false;
            break;
        }
    }
    return isNotEndOfWord && hasNoChildren;
}

//Ham xoa node
void deleteWord(TrieNode* root, const string& word, bool& isDeleted, int depth = 0) {
    if (!root) {
        isDeleted = false;
        return;
    }
    if (word.empty()) {
        isDeleted = false;
        return;
    }
    if (depth == word.length()) {
        if (root->isEndOfWord) {
            root->isEndOfWord = false;
            isDeleted = true;        
        }
        else {
            isDeleted = false;
        }
        return;
    }
    int index = word[depth] - 'a';
    if (index < 0 || index >= ALPHABET_SIZE || !root->children[index]) {
        isDeleted = false;
        return;
    }
    bool childDeleted = false;
    deleteWord(root->children[index], word, childDeleted, depth + 1);
    if (childDeleted && shouldDeleteNode(root->children[index])) {
        delete root->children[index];
        root->children[index] = nullptr;
    }
    isDeleted = isDeleted || childDeleted;
}


// Kiem tra tu co ton tai trong cay khong
bool searchWord(TrieNode* root, const string& word) {
    TrieNode* node = searchPrefix(root, word);
    return (node != nullptr && node->isEndOfWord);
}


// Doc tu dien va luu vao cay trie
void loadDictionary(TrieNode* root, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Khong the mo file tu dien: " << filename << endl;
        return;
    }
    string word;
    while (file >> word) {
        insert(root, word);
    }
    file.close();
    cout << "Da nap tu dien thanh cong tu file: " << filename << endl;
}

//Chay chuong trinh
void process(TrieNode* root) {
    clearScreen();
    printHeader("CHUONG TRINH TU DIEN VOI TRIE");
    cout << "\n";
    cout << "\t\t1. Goi y tu khoa\n";
    cout << "\t\t2. Them tu khoa\n";
    cout << "\t\t3. Xoa tu khoa\n";
    cout << "\t\t4. Thoat chuong trinh\n";
    cout << "\t\t5. Kiem tra tu khoa\n\n";
    cout << "\t\tChon mot so (1-5): ";
    string choiceS;
    string input;
    cin >> choiceS;
    if (choiceS == "1" || choiceS == "2" || choiceS == "3" || choiceS == "4" || choiceS == "5") {
        int choice = stoi(choiceS);
        bool isDeleted = false;
        clearScreen();
        printHeader("CHUONG TRINH TU DIEN VOI TRIE");
        switch (choice) {
        case 1:
            cout << "Nhap tien to: ";
            cin.ignore();
            getline(cin, input);
            autocomplete(root, input);
            break;
        case 2:
            cout << "Nhap tu can them: ";
            cin.ignore();
            getline(cin, input);
            insert(root, input);
            cout << "Da them tu '" << input << "' vao tu dien.\n";
            break;
        case 3:
            cout << "Nhap tu can xoa: ";
            cin.ignore();
            getline(cin, input);
            isDeleted = false;
            deleteWord(root, input, isDeleted);
            if (isDeleted) {
                cout << "Da xoa thanh cong." << endl;
            }
            else {
                cout << "Khong tim thay tu de xoa." << endl;
            }
            break;
        case 4:
            cout << "Thoat chuong trinh.\n";
            return;
        case 5:
            cout << "Nhap tu can kiem tra: ";
            cin.ignore();
            getline(cin, input);
            if (searchWord(root, input))
                cout << "Tu '" << input << "' co trong tu dien.\n";
            else
                cout << "Tu '" << input << "' khong co trong tu dien.\n";
            break;
        default:
            cout << "Lua chon khong hop le! Hay chon lai.\n";
            system("pause");
            break;
        }
        cout << "\n";
        system("pause");
        process(root);
    }
    else {
        clearScreen();
        printHeader("CHUONG TRINH TU DIEN VOI TRIE");
        cout << "Lua chon khong hop le! Hay chon lai.\n";
        system("pause");
        process(root);
    }
}


int main() {
    TrieNode* root = new TrieNode();
    string dictionaryFile = "basic_english_words.txt";
    loadDictionary(root, dictionaryFile);
    process(root);

    return 0;
}
