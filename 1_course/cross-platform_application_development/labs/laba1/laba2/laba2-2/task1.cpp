#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstring>

using namespace std;

struct Comp {
    char Name[30];
    char Section[15];
    int Num;
};

vector<Comp> readFromFile(const string& filename) {
    vector<Comp> vec;
    ifstream in(filename);
    if (!in) {
        cerr << "Не удалось открыть файл " << filename << endl;
        return vec;
    }
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        Comp c;
        string name, section;
        if (getline(ss, name, '\t') && getline(ss, section, '\t') && (ss >> c.Num)) {
            strncpy(c.Name, name.c_str(), 29);
            c.Name[29] = '\0';
            strncpy(c.Section, section.c_str(), 14);
            c.Section[14] = '\0';
            vec.push_back(c);
        }
    }
    in.close();
    return vec;
}

void writeToFile(const string& filename, const vector<Comp>& vec) {
    ofstream out(filename);
    if (!out) {
        cerr << "Не удалось открыть файл " << filename << endl;
        return;
    }
    for (const auto& c : vec) {
        out << c.Name << "\t" << c.Section << "\t" << c.Num << endl;
    }
    out.close();
}

void display(const vector<Comp>& vec) {
    for (const auto& c : vec) {
        cout << c.Name << "\t" << c.Section << "\t" << c.Num << endl;
    }
}

bool equal(const Comp& a, const Comp& b) {
    return strcmp(a.Name, b.Name) == 0 && strcmp(a.Section, b.Section) == 0;
}

vector<Comp> intersection(const vector<Comp>& a, const vector<Comp>& b) {
    vector<Comp> result;
    for (const auto& itemA : a) {
        for (const auto& itemB : b) {
            if (equal(itemA, itemB)) {
                result.push_back(itemA);
                break;
            }
        }
    }
    return result;
}

bool compareByName(const Comp& a, const Comp& b) {
    return strcmp(a.Name, b.Name) < 0;
}

bool searchByName(const vector<Comp>& vec, const string& name, Comp& result) {
    for (const auto& c : vec) {
        if (strcmp(c.Name, name.c_str()) == 0) {
            result = c;
            return true;
        }
    }
    return false;
}

int main() {
    vector<Comp> M1 = readFromFile("Ob1.txt");
    cout << "M1:" << endl;
    display(M1);
    cout << endl;

    vector<Comp> M2 = readFromFile("Ob2.txt");
    cout << "M2:" << endl;
    display(M2);
    cout << endl;

    vector<Comp> M3 = intersection(M1, M2);
    cout << "Пересечение M3:" << endl;
    display(M3);
    cout << endl;

    writeToFile("TOb1.txt", M3);

    sort(M3.begin(), M3.end(), compareByName);
    cout << "M3, отсортированный по названию:" << endl;
    display(M3);
    cout << endl;

    writeToFile("TOb2.txt", M3);

    vector<Comp> K1 = readFromFile("TOb1.txt");
    cout << "K1 из TOb1.txt:" << endl;
    display(K1);
    cout << endl;

    vector<Comp> K2 = readFromFile("TOb2.txt");
    cout << "K2 из TOb2.txt:" << endl;
    display(K2);
    cout << endl;

    string searchName;
    cout << "Введите название товара для поиска в M3: ";
    cin >> searchName;
    Comp result;
    if (searchByName(M3, searchName, result)) {
        cout << "Найден: " << result.Name << " " << result.Section << " " << result.Num << endl;
    } else {
        cout << "Товар не найден." << endl;
    }

    return 0;
}
