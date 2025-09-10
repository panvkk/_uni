#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

struct BaseItem {
    string Name;
    virtual ~BaseItem() = default;
    virtual vector<string> getFields() const = 0;
};

struct CompItem : BaseItem {
    string Section;
    double Price = 0.0;
    
    vector<string> getFields() const override {
        return {Name, Section, to_string(Price)};
    }
};

istream& operator>>(istream& is, CompItem& item) {
    string line;
    if (!getline(is, line)) return is;

    size_t pos1 = line.find(';');
    if (pos1 == string::npos) return is;
    item.Name = line.substr(0, pos1);

    size_t pos2 = line.find(';', pos1 + 1);
    if (pos2 == string::npos) return is;
    item.Section = line.substr(pos1 + 1, pos2 - pos1 - 1);

    try {
        item.Price = stod(line.substr(pos2 + 1));
    } catch (...) {
        item.Price = 0.0;
    }
    return is;
}

struct BagItem : BaseItem {
    string Section;
    double Weight = 0.0;
    
    vector<string> getFields() const override {
        return {Name, Section, to_string(Weight)};
    }
};

istream& operator>>(istream& is, BagItem& item) {
    string line;
    if (!getline(is, line)) return is;

    size_t pos1 = line.find(';');
    if (pos1 == string::npos) return is;
    item.Name = line.substr(0, pos1);

    size_t pos2 = line.find(';', pos1 + 1);
    if (pos2 == string::npos) return is;
    item.Section = line.substr(pos1 + 1, pos2 - pos1 - 1);

    try {
        item.Weight = stod(line.substr(pos2 + 1));
    } catch (...) {
        item.Weight = 0.0;
    }
    return is;
}

template <typename T>
class COMP {
private:
    int count = 0;
    T* data = nullptr;
    static vector<string> headers;
    static vector<int> colWidths;

    void calculateColumnWidths() const {
        colWidths = vector<int>(headers.size(), 0);
        
        for (int i = 0; i < headers.size(); i++) {
            if (headers[i].length() > colWidths[i]) {
                colWidths[i] = headers[i].length();
            }
        }
        
        for (int i = 0; i < count; i++) {
            vector<string> fields = data[i].getFields();
            for (int j = 0; j < fields.size() && j < colWidths.size(); j++) {
                if (fields[j].length() > colWidths[j]) {
                    colWidths[j] = fields[j].length();
                }
            }
        }
        
        for (int& width : colWidths) {
            width += 2;
        }
    }

public:
    COMP() = default;
    
    COMP(const COMP& other) : count(other.count), data(new T[other.count]) {
        for (int i = 0; i < count; ++i)
            data[i] = other.data[i];
    }
    
    ~COMP() {
        delete[] data;
    }

    COMP& operator=(const COMP& other) {
        if (this != &other) {
            delete[] data;
            count = other.count;
            data = new T[count];
            for (int i = 0; i < count; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    void ReadFromStream(istream& is) {
        vector<T> items;
        T item;
        while (is >> item) {
            items.push_back(item);
        }
        
        delete[] data;
        count = static_cast<int>(items.size());
        data = new T[count];
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    void WriteToStream(ostream& os) const {
        for (int i = 0; i < count; ++i) {
            vector<string> fields = data[i].getFields();
            for (int j = 0; j < fields.size(); j++) {
                os << fields[j];
                if (j < fields.size() - 1) os << ";";
            }
            os << '\n';
        }
    }

    void PrintTable(ostream& os) const {
        calculateColumnWidths();
        
        for (int i = 0; i < headers.size(); i++) {
            os << left << setw(colWidths[i]) << headers[i];
        }
        os << "\n";
        
        for (int width : colWidths) {
            os << string(width, '-') << " ";
        }
        os << "\n";
        
        for (int i = 0; i < count; i++) {
            vector<string> fields = data[i].getFields();
            for (int j = 0; j < fields.size() && j < headers.size(); j++) {
                os << left << setw(colWidths[j]) << fields[j];
            }
            os << "\n";
        }
    }

    COMP operator+(const COMP& other) const {
        vector<T> result;
        
        for (int i = 0; i < count; ++i) {
            bool found = false;
            for (const auto& item : result) {
                if (item.Name == data[i].Name) {
                    found = true;
                    break;
                }
            }
            if (!found) result.push_back(data[i]);
        }
        
        for (int i = 0; i < other.count; ++i) {
            bool found = false;
            for (const auto& item : result) {
                if (item.Name == other.data[i].Name) {
                    found = true;
                    break;
                }
            }
            if (!found) result.push_back(other.data[i]);
        }
        
        COMP newComp;
        newComp.count = static_cast<int>(result.size());
        newComp.data = new T[newComp.count];
        for (int i = 0; i < newComp.count; ++i) {
            newComp.data[i] = result[i];
        }
        return newComp;
    }

    int GetCount() const {
        return count;
    }

    void SortByName() {
        sort(data, data + count, [](const T& a, const T& b) {
            return a.Name < b.Name;
        });
    }

    void SortByNameReverse() {
        sort(data, data + count, [](const T& a, const T& b) {
            return a.Name > b.Name;
        });
    }

    void SortBySectionAndNameReverse() {
        sort(data, data + count, [](const T& a, const T& b) {
            if (a.Section != b.Section)
                return a.Section < b.Section;
            return a.Name > b.Name;
        });
    }

    T* FindByName(const string& name) {
        for (int i = 0; i < count; ++i) {
            if (data[i].Name == name) {
                return &data[i];
            }
        }
        return nullptr;
    }

    static void SetHeaders(const vector<string>& h) {
        headers = h;
    }
};

template<> vector<string> COMP<CompItem>::headers = {"Наименование товара", "Секция", "Цена"};
template<> vector<int> COMP<CompItem>::colWidths = {0, 0, 0};

template<> vector<string> COMP<BagItem>::headers = {"Наименование товара", "Секция", "Вес"};
template<> vector<int> COMP<BagItem>::colWidths = {0, 0, 0};

void operationA(COMP<CompItem>& compObj) {
    COMP<CompItem> comp1, comp2;
    
    ifstream file1("comp1.txt");
    ifstream file2("comp2.txt");
    
    comp1.ReadFromStream(file1);
    comp2.ReadFromStream(file2);
    
    cout << "\nМассив 1 (Comp):\n";
    comp1.PrintTable(cout);
    cout << "\nМассив 2 (Comp):\n";
    comp2.PrintTable(cout);
    
    compObj = comp1 + comp2;
    
    ofstream outFile("comp_union.txt");
    compObj.WriteToStream(outFile);
    
    cout << "\nОбъединенный массив (Comp):\n";
    compObj.PrintTable(cout);
}

void operationB(COMP<BagItem>& bagObj) {
    COMP<BagItem> bag1, bag2;
    
    ifstream file1("bag1.txt");
    ifstream file2("bag2.txt");
    
    bag1.ReadFromStream(file1);
    bag2.ReadFromStream(file2);
    
    cout << "\nМассив 1 (Bag):\n";
    bag1.PrintTable(cout);
    cout << "\nМассив 2 (Bag):\n";
    bag2.PrintTable(cout);
    
    bagObj = bag1 + bag2;
    
    ofstream outFile("bag_union.txt");
    bagObj.WriteToStream(outFile);
    
    cout << "\nОбъединенный массив (Bag):\n";
    bagObj.PrintTable(cout);
}

template <typename T>
void operationC(COMP<T>& obj) {
    obj.SortByNameReverse();
    cout << "\nМассив после обратной сортировки:\n";
    obj.PrintTable(cout);
}

template <typename T>
void operationD(COMP<T>& obj) {
    obj.SortBySectionAndNameReverse();
    cout << "\nМассив после сложной сортировки:\n";
    obj.PrintTable(cout);
}

template <typename T>
void operationE(COMP<T>& obj) {
    string name;
    cout << "Введите наименование товара: ";
    cin.ignore();
    getline(cin, name);
    
    T* item = obj.FindByName(name);
    if (item) {
        cout << "\nТовар найден:\n";
        vector<string> fields = item->getFields();
        string line;
        for (size_t j = 0; j < fields.size(); j++) {
            if (j > 0) line += ';';
            line += fields[j];
        }
        COMP<T> tempComp;
        istringstream iss(line);
        tempComp.ReadFromStream(iss);
        tempComp.PrintTable(cout);
    } else {
        cout << "\nТовар не найден.\n";
    }
}

int main() {
    char choice;
    COMP<CompItem> compObj;
    COMP<BagItem> bagObj;
    bool compLoaded = false, bagLoaded = false;

    do {
        cout << "\nМеню операций:\n"
             << "a - Работа с электроникой (Comp)\n"
             << "b - Работа с сумками (Bag)\n"
             << "c - Обратная сортировка по имени\n"
             << "d - Сложная сортировка\n"
             << "e - Поиск по имени\n"
             << "q - Выход\n"
             << "Выберите операцию: ";
        cin >> choice;

        switch (choice) {
            case 'a': 
                operationA(compObj); 
                compLoaded = true;
                break;
            case 'b': 
                operationB(bagObj); 
                bagLoaded = true;
                break;
            case 'c': {
                cout << "Выберите тип (1 - Comp, 2 - Bag): ";
                int type; cin >> type;
                if (type == 1 && compLoaded) operationC(compObj);
                else if (type == 2 && bagLoaded) operationC(bagObj);
                else cout << "Ошибка: данные не загружены или неверный тип\n";
                break;
            }
            case 'd': {
                cout << "Выберите тип (1 - Comp, 2 - Bag): ";
                int type; cin >> type;
                if (type == 1 && compLoaded) operationD(compObj);
                else if (type == 2 && bagLoaded) operationD(bagObj);
                else cout << "Ошибка: данные не загружены или неверный тип\n";
                break;
            }
            case 'e': {
                cout << "Выберите тип (1 - Comp, 2 - Bag): ";
                int type; cin >> type;
                if (type == 1 && compLoaded) operationE(compObj);
                else if (type == 2 && bagLoaded) operationE(bagObj);
                else cout << "Ошибка: данные не загружены или неверный тип\n";
                break;
            }
            case 'q': break;
            default: cout << "Неверный выбор!\n";
        }
    } while (choice != 'q');

    return 0;
}