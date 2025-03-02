#include "quick_sort.h"

void loadFileToVector(const string& filename, vector<string>& dictionary) {
    ifstream file(filename);
    if (!file) {
        cout << "Khong the mo file: " << filename << endl;
        return;
    }
    string word;
    while (file >> word) {
        if (word.length() >= 3) {
            dictionary.push_back(word);
        }
    }
    file.close();
}


int partition(vector<string>& a, int low, int high,int& c) {
    string pivot = a[high];  
    int left = low;
    int right = high - 1;  

    while (left <= right) {
        while (left <= right && a[left] < pivot) {
            c++;
            left++;
        }
        while (left <= right && a[right] > pivot) {
            c++;
            right--;
        }
        if (left <= right) {
            swap(a[left], a[right]);
            left++;
            right--;
        }
    }
    swap(a[left], a[high]);
    return left;
}

void quickSort(vector<string>& a, int low, int high,int& c) {
    if (low < high) {
        int pivotIndex = partition(a, low, high,c);  
        quickSort(a, low, pivotIndex - 1,++c); 
        quickSort(a, pivotIndex + 1, high,++c); 
    }
}

vector<string> quicksortAutocomplete(vector<string>& dictionary, const string& prefix, int maxSuggestions, int& comparisons) {
    vector<string> filteredWords;
    for (const auto& word : dictionary) {
        comparisons++;  
        if (word.rfind(prefix, 0) == 0) {        
            filteredWords.push_back(word);
            if (filteredWords.size() == maxSuggestions) {
                break;  
            }
        }
    }
    if (filteredWords.empty()) {      
        return filteredWords;
    }
    int quickSortComparisons = 0;
    if (filteredWords.size() >1) {
        quickSort(filteredWords, 0, filteredWords.size()-1 , quickSortComparisons); 
        comparisons += quickSortComparisons;
    }
    return filteredWords;
}