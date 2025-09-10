#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream cin("input.txt"); ofstream cout("output.txt"); ofstream coutsort("out_sort.txt");
    string s1, curs = "", prevch = "";
    getline(cin,s1);
    vector<string> words;
    for (long long i = 0; i < s1.size(); i++) {
        if (s1[i] == ' ') {
            if (curs != "")
                words.push_back(curs);
            curs = "";
        }
        else
            curs.push_back(s1[i]);
    }
    if (curs != "")
        words.push_back(curs);
    reverse(words.begin(), words.end());
    for (long long i = 0; i < words.size(); i++) {
        int check = 1;
        for (long long j = 0; j < words[i].size(); j++)
            if (words[i][j] != '2' && words[i][j] != '4' && words[i][j] != '6' && words[i][j] != '8' && words[i][j] != '0')
                check = 0;
        if (check) {
            if (prevch != "")
                swap(prevch, words[i]);
            else
                prevch = words[i];
        }
    }
    reverse(words.begin(), words.end());
    for (long long i = 0; i < words.size(); i++) {
        cout << words[i] << ' ';
    }
    sort(words.begin(), words.end());
    for (long long i = 0; i < words.size(); i++) {
        coutsort << words[i] << ' ';
    }
}
