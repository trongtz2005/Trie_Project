#include "Trie.h"
#include "quick_sort.h"


template <typename Func, typename... Args>
double measureExecutionTime(Func func, Args&&... args) {
    auto start = chrono::high_resolution_clock::now();
    func(forward<Args>(args)...);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    return elapsed.count() * 1000; // Tinh theo don vi ms
}

void compareTrieVsQuicksort(TrieNode* root, vector<string>& dictionary, int maxSuggestions, int length) {
    const int iterations = 50000;  // Lay ket qua trung binh cua 50000 lan chay
    double totalTrieTime = 0.0;
    double totalQuicksortTime = 0.0;
    int totalTrieComparisons = 0;
    int totalQuicksortComparisons = 0;

    for (int i = 0; i < iterations; ++i) {
        string prefix = randomPrefix(root, length);

        // Do thoi gian va so phep so sanh cho TRIE
        int trieComp = 0;
        vector<string> trieResults;
        totalTrieTime += measureExecutionTime([&]() {
            trieResults = autocompleteTrie(root, prefix, maxSuggestions, trieComp);
            });
        totalTrieComparisons += trieComp;

        // Do thoi gian va so phep so sanh cho QuickSort
        int quicksortComp = 0;
        vector<string> quicksortResults;
        totalQuicksortTime += measureExecutionTime([&]() {
            quicksortResults = quicksortAutocomplete(dictionary, prefix, maxSuggestions, quicksortComp);
            });
        totalQuicksortComparisons += quicksortComp;

    }

    // Tinh trung binh
    double avgTrieTime = totalTrieTime / iterations;
    double avgQuicksortTime = totalQuicksortTime / iterations;
    int avgTrieComparisons = totalTrieComparisons / iterations;
    int avgQuicksortComparisons = totalQuicksortComparisons / iterations;

    cout << fixed << setprecision(6);
    cout << "So sanh ket qua sau " << iterations << " lan chay:\n";
    cout << "- Trie:\n";
    cout << "  + Thoi gian trung binh: " << avgTrieTime << " ms\n";
    cout << "  + So phep so sanh trung binh: " << avgTrieComparisons << " phep\n";
    cout << "- QuickSort:\n";
    cout << "  + Thoi gian trung binh: " << avgQuicksortTime << " ms\n";
    cout << "  + So phep so sanh trung binh: " << avgQuicksortComparisons << " phep\n";
}

int main() {
    srand(time(0));
    TrieNode* root = new TrieNode();

    string dictionaryFile = "words.txt";
    loadDictionary(root, dictionaryFile); 

    vector<string> dictionary;
    loadFileToVector(dictionaryFile, dictionary);  

    // So ssanh 
    int prefixLength = 5;   // Do dai tien to
    int maxSuggestions =15; // So tu goi y toi da
    compareTrieVsQuicksort(root, dictionary, maxSuggestions, prefixLength);

    delete root;
    return 0;
}
