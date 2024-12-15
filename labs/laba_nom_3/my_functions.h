#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ostream>
#include <cstring>
#include <algorithm>

const char seporators[3] {' ', ',', '.'};
char randomSeparator() {
    return seporators[rand()%3];
}
std::vector<std::string> searchAndReplace (std::vector<std::string>, std::vector<std::string>);
std::vector<std::string> getVecLine (std::string);
void fileSelSort (std::vector<std::string>);
 
std::vector<std::string> searchAndReplace (std::vector<std::string> txt, std::vector<std::string> wrd) {
    std::vector<std::string> result = txt;
    for(int index = 1; index < txt.size(); index++) {
        std::string s = txt[index];
        std::vector<std::string> newWords = getVecLine(s);
        for (int k = 0; k < newWords.size(); k++) {
            for (int m = 0; m < wrd.size(); m++) {
                if (newWords[k] == wrd[m]) {
                    std::string temp = result[index];
                    temp.push_back(randomSeparator());
                    for (int curr = 0; curr < newWords[k].size(); curr++) {
                        temp.push_back(newWords[k][curr]);
                        }
                    result[index] = temp;
                }
            }
        }
        newWords.clear();
    }
    return result;
}
void fileSelSort (std::vector<std::string> txt) {
    std::ofstream FILE ("Out_sort.txt");
    for (int i = 0; i < txt.size(); i++) {
        std::string s = txt[i];
        std::vector<std::string> currLine = getVecLine (s);
        for (int j = 0; j < currLine.size(); j++) {
            std::string min = currLine[j];
            int counter = j;
            for (int m = j; m < currLine.size(); m++) {
                if (min.compare(currLine[m]) > 0 ) {
                    min = currLine[m];
                    counter = m;
                }
            }
            std::swap (currLine[j], currLine[counter]);
        }
        for (int k = 0; k < currLine.size(); k++) { 
            FILE << currLine[k] << randomSeparator();
        }
        FILE << "\n";
        currLine.clear();
    }
    // for(int i = 0; i < txt.size(); i++) {
    //     std::vector<std::string> current_line = getVecLine(txt[i]);
    //     std::sort(current_line.begin(), current_line.end());
    //     for(int j = 0; j < current_line.size(); j++) {
    //         FILE << current_line[j] << " ";
    //     }
    //     FILE << "\n";
    // }
}
std::vector<std::string> getVecLine (std::string str) {
    std::string w;
    std::vector<std::string> newLine;
    for (int l = 0; l <= str.size(); ++l) {
        if (str[l] == ' ' || str[l] == '.' || str[l] == ',' || str[l] == '\0') {
            if (!w.empty()) {
                newLine.push_back(w);
                w.clear(); 
            }
        } else {
            w.push_back(str[l]);
        }
    }
    return newLine;
}