/*
Подсчитатаь слова:
Мама мыла раму#
На КР будет похожая задача. КР на следующей неделе.
Решение, которое есть исопльзуется с юникод кодировка.

*/
#include <iostream>
#include <string>

int getStat(int, char*);

extern "C" int __cdecl getWordsCount(int lenth, char* line);

int main()
{
    char line[80];
    int count, len;
    unsigned int wordsCount;
    char curr, prev;
    count = 0;
    for (count; std::cin.get(curr) && curr != '#'; count++) {
        line[count] = curr;
    }
    line[count] = '\0';
    len = (int)std::strlen(line);
    
    count = getWordsCount(len, line);

    std::cout << count << "\n";
    system("pause");
}

int getStat(int lenth, char* line) {
    int wordsCount = 0;
    bool isWasWord = false;
    for (lenth; lenth >= 0; lenth--) {
        if (!isalnum(line[lenth])) {
            if (isWasWord) {
                wordsCount++;
                isWasWord = false;
            }
        }
        else{
            isWasWord = true;
        }
    }
    if (isWasWord)
        wordsCount++;
    return wordsCount;
}

//мама мыла раму 