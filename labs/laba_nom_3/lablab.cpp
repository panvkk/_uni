#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

map<string, int> months = { {"января", 1}, {"февраля", 2}, {"марта", 3}, {"апреля", 4}, {"мая", 5}, {"июня", 6}, {"июля", 7}, {"августа", 8}, {"сентября", 9}, {"октября", 10}, {"ноября", 11}, {"декабря", 12} };

class TPerson {
    string fullName;
    string birthDate;
    int day, month, year;
public:
    int CRS;
    vector<int> MAS;

    TPerson() : fullName("") , birthDate("") , day(0), month(0), year(0)
        {}

    TPerson(string fullName, string birthDate) : fullName(fullName), birthDate(birthDate) {
        parseBirthDate();
        calculateCRS();
        calculateMAS();
    }

    ~TPerson() {
        MAS.resize(0);
        MAS.shrink_to_fit();
        cout << "Объкт класса удалён.\n";
    }

    void setFullName(string s) {
        fullName = s;
    }

    string getFullName() const {
        return fullName;
    }

    void setbirthDate(string b) {
        birthDate = b;
    }
    string getbirthDate() const {
        return birthDate;
    }

    void parseBirthDate() {
        day = stoi(birthDate.substr(0, 2));

        auto firstLetter = birthDate.find_first_not_of("1234567890 ");
        auto lastLetter = birthDate.find_last_not_of("1234567890 ") - birthDate.find_first_not_of("1234567890 ") + 1;
        string m = birthDate.substr(firstLetter, lastLetter);

        month = months[m];

        year = stoi(birthDate.substr(birthDate.find_last_not_of("1234567890 ")+1, string::npos));
    }

    void calculateCRS() {
        long long Day = day*1000000 + month*10000 + year;
        int B = sumNumbers(Day);
        int C = sumNumbers(B);
        int D = B - 2 * ( (day < 10) ? day : day/10 );
        int E = sumNumbers(D);
        CRS = B * 10000 + C * 1000 + D * 10 + E;
    }

    void calculateMAS() {
        string digits = to_string(day*1000000 + month*10000 + year) + to_string(CRS);
        for (char digit : digits) {
            if (digit != '0' && find(MAS.begin(), MAS.end(), digit - '0') == MAS.end()) {
                MAS.push_back(digit - '0');
            }
        }
    }

    int sumNumbers(int number) {
        int sum = 0;
        while (number) {
            sum += number % 10;
            number /= 10;
        }
        return sum;
    }

    friend TPerson* binarySearch(vector<TPerson>&, int);
    friend void operator>>(ifstream&, TPerson&);
};

void result(vector<TPerson>&);

void operator>>(ifstream& f, TPerson& p) {
    string line;
    getline(f, line);

    int firstNumb = line.find_first_of("1234567890");
    auto pos = line.begin() + firstNumb;

    p.setFullName(line.substr(0, firstNumb));
    line.erase(line.begin(), pos);

    p.setbirthDate(line);

    p.parseBirthDate();
    p.calculateCRS();
    p.calculateMAS();
}

int main() {
    ifstream inputFile("Name.txt");
    ofstream outputFile("Task.txt");
    vector<TPerson> persons;
    TPerson pers;

    while (!inputFile.eof()) {
        inputFile >> pers;
        persons.push_back(pers);
        outputFile << pers.getFullName() << " " << pers.getbirthDate() << " CRS: " << pers.CRS << " MAS: ";
        for (int num : pers.MAS) {
            outputFile << num << " ";
        }
        outputFile << endl;
    }
    inputFile.close();
    outputFile.close();

    result(persons);
    return 0;
}

map<int, string> readMight() {
    ifstream file("Might.txt");
    map<int, string> mights;
    string line;
    int index = 1;

    while (getline(file, line)) {
        mights[index] = line;
        index++;
    }
    file.close();
    return mights;
}

void result(vector<TPerson>& p) {
    ofstream resultFile("Result.txt");
    map<int, string> mights = readMight();

    for (const auto& person : p) {
        resultFile << person.getFullName() << " ";
        for (int num : person.MAS) {
            resultFile << mights[num] << " ";
        }
        resultFile << endl;
    }
    resultFile.close();
}

TPerson* binarySearch(vector<TPerson>& persons, int year) {
    int left = 0, right = persons.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (persons[mid].year == year) {
            return &persons[mid];
        } else if (persons[mid].year < year) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return nullptr;
}