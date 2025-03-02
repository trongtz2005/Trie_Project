#pragma once
#include "lib.h"

// Ham quicksort va cac ham lien quan
int partition(vector<string>& a, int low, int high, int& c);
void quickSort(vector<string>& a, int low, int high, int& c);
void loadFileToVector(const string& filename, vector<string>& dictionary);
vector<string> quicksortAutocomplete(vector<string>& dictionary, const string& prefix, int maxSuggestions, int& comparisons);


