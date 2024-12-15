#include <iostream>
#include <fstream>
#include <string>
#include <ostream>

const int MAX = 20;

bool wordMacth(char*, char*);

void searchWord(char*);

int main()
{
    std::ofstream oufile ("Input.txt");
    oufile << "ява.ложь,ива,вёдра.волшба.исподлобья,конъюнктура\n"
            << "изгородь.лев,волшба\n"
            << "локомотив ива.вёдра\n"
            << "конъюнктура исподлобья\n";
    oufile.close ();
    
    // std::ifstream infile ("input.txt");
    // while (!infile.eof())
    // {
    //     char c;
    //     infile >> c;
    //     ofile << c;
    // }
    // infile.seekg (0, infile.beg);
    std::ifstream infile( "Input.txt");

    while (infile)
    {
        int i = 0;
        bool indicator = false;
        char word[MAX];
        while (infile.get(word[i]))
        {
            // switch(word[i])
            // {
            //     case ' ' || ',' || '.':
            //         word[i] = '\0';
            //         break;
            //     case '\n':
            //         indicator = 1;
            //         word[i] = '\0';
            //         break;
            // }
            if (word[i] == ' ' || word[i] == '.' || word[i] == ',' || word[i] == '\n')
            {
                if(word[i] == '\n')
                    indicator = true;
                word[i] = '\0';
                break;
            }
            i++;
        }
        searchWord(word);
        if(indicator)
            return 0;
    }
    // std::ofstream ofile ("Output.txt", std::ios::app);
    // ofile << "ява.ложь,ива,вёдра.волшба.исподлобья,конъюнктура\n"
    //         << "изгородь.лев,волшба\n"
    //         << "локомотив ива.вёдра\n"
    //         << "конъюнктура исподлобья\n";
    // ofile.close ();
}

bool wordMatch(char* s1, char* s2)
{
    for(int j = 0; s1[j] != '\0' || s2[j] != '\0' ; j++)
    {
        if(s1[j] == s2[j])
            continue;
        else
            return false;
    }
    return true;
}
void searchWord (char* W1)
{
    std::string dummy;
    std::fstream file ("Output.txt", std::ios::in | std::ios::out /*| std::ios::app */ );

    std::ifstream ifile ("Input.txt");
    std::getline (ifile, dummy);
    std::getline (file, dummy);

    bool well = false;
    int counter = 1;
    while (ifile)
    {
        if(well)
            counter++;
        int n = 0;
        char W2[MAX];
        while (ifile.get(W2[n]))
        {
            if ( W2[n] ==  ' ' || W2[n] == ','  ||  W2[n] == '.' || W2[n] == '\n' )
            {
                W2[n] == '\n' ? well = true : well = false;
                W2[n] = '\0';
                break;
            }
            n++;
        }
        if (wordMatch(W1, W2))
        {   
            for (int m = 0; m < counter; m++)
            {
                std::getline(file, dummy);
            }
            long pos = file.tellg();
            file.seekp(pos);
            for (int s = 0; W1[s] != '\0' ; s++)
                file << W1[s];
            file << " ";
        }
    }
    file.close();
    ifile.close();
}