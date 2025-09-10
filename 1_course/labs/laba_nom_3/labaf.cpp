#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "my_functions.h"

int main()
{
    srand(time(0));
    // createFile();
/////////////////////////////////////////////////////////////////////////
    std::ifstream input("Input.txt");

    std::vector<std::string> text;
    while(!input.eof())
    {
        std::string line;
        std::getline(input, line);
        text.push_back(line);
    }
    input.close();
/////////////////////////////////////////////////////////////////////////
    std::string s = text[0];

    std::vector<std::string> words = getVecLine(s);

    text = searchAndReplace(text, words);

    std::ofstream output ("Output.txt");
    for (int iter = 0; iter < text.size(); iter++ ) {
        output << text[iter] << "\n";
    }
    output.close ();
/////////////////////////////////////////////////////////////////////////
    fileSelSort(text);
/////////////////////////////////////////////////////////////////////////
    return 0;
}