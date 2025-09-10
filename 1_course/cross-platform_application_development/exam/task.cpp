#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <fstream>

struct S1 {
    std::string Name;
    std::string Section;
};

struct S2 {
    std::string Name;
    std::string Section;
    int quantity;
};


bool operator==(const S1& a, const S1& b) {
    return a.Name == b.Name;
}

bool operator==(const S2& a, const S2& b) {
    return a.Name == b.Name;
}

std::ostream& operator<<(std::ostream& os, const S1& s) {
    os << s.Name << " " << s.Section;
    return os;
}

std::ostream& operator<<(std::ostream& os, const S2& s) {
    os << s.Name << " " << s.Section << " " << s.quantity;
    return os;
}

std::istream& operator>>(std::istream& is, S1& s) {
    std::string line;
    if (!std::getline(is, line)) return is;

    size_t pos1 = line.find(';');
    if (pos1 == std::string::npos) return is;
    s.Name = line.substr(0, pos1);

    s.Section = line.substr(pos1 + 1, line.length());

    return is;
}

std::istream& operator>>(std::istream& is, S2& s) {
    std::string line;
    if (!std::getline(is, line)) return is;

    size_t pos1 = line.find(';');
    if (pos1 == std::string::npos) return is;
    s.Name = line.substr(0, pos1);

    size_t pos2 = line.find(';', pos1 + 1);
    if (pos2 == std::string::npos) return is;
    s.Section = line.substr(pos1 + 1, pos2 - pos1 - 1);

    try {
        s.quantity = stoi(line.substr(pos2 + 1));
    } catch (...) {
        s.quantity = 0;
    }
    return is;
}

class ABS_COMP {
public:
    virtual ~ABS_COMP() = default;
    virtual void WriteData(std::ostream& out) const = 0;
    virtual void PutData(std::istream& in) = 0;
    // virtual void Merge(ABS_COMP* a, ABS_COMP* b) = 0;
    virtual void Sort() = 0;
};

class Fruit : ABS_COMP {
    S1* data;
    int count;
public:
    Fruit() : data(nullptr), count(0) {}

    ~Fruit() override { delete[] data; }

    Fruit& operator=(const Fruit* other) {
        if(this->data != other->data) {
            delete[] data;
            count = other->count;
            data = new S1[count];
            for(int i = 0; i < count; i++) 
                data[i] = other->data[i];
        }
        return *this;
    }

    void WriteData(std::ostream& out) const override {
        for (int i = 0; i < count; ++i) {
            out << data[i] << '\n';
        }
    }

    void PutData(std::istream& in) override {
        std::vector<S1> items;
        S1 item;
        while (in >> item) {
            items.push_back(item);
        }
        
        delete[] data;
        count = static_cast<int>(items.size());
        data = new S1[count];
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    int GetCount() {
        return count;
    }

    S1 GetStruct(int& i) {
        if(i < count)
            return data[i];
        S1 mock;
        return mock;
    }

    void Sort() override {
        std::sort(data, data + count, [](const S1& a, const S1& b) {
            return a.Name > b.Name;
        });
    }

    void AlphabetSort() {
        std::sort(data, data + count, [](const S1& a, const S1& b) {
            return a.Name < b.Name;
        });
    }

    int findByName(std::string& query_name) {   
        for(int i = 0; i < count; i++) {
            if(data[i].Name == query_name)
                return i;
        }
        return -1;
    }
    
    void Merge(Fruit& a, Fruit& b)  {
        std::vector<S1> merging;
        for(int i = 0; i < a.count; i++) {
            if(b.findByName(a.data[i].Name) != -1 && 
                std::find(merging.begin(), merging.end(), a.data[i]) == merging.end()

            ) {
                merging.push_back(a.data[i]);
            }
        }
        delete[] data;
        count = static_cast<int>(merging.size());
        data = new S1[count];
        for (int i = 0; i < count; ++i) {
            data[i] = merging[i];
        }
    }
    
};

class Bag : ABS_COMP {
    S2* data;
    int count;
public:
    Bag() : data(nullptr), count(0) {}

    ~Bag() override { delete[] data; }

    Bag& operator=(const Bag* other) {
        if(this->data != other->data) {
            delete[] data;
            count = other->count;
            data = new S2[count];
            for(int i = 0; i < count; i++) 
                data[i] = other->data[i];
        }
        return *this;
    }

    void WriteData(std::ostream& out) const override {
        for (int i = 0; i < count; ++i) {
            out << data[i] << '\n';
        }
    }

    void PutData(std::istream& in) override {
        std::vector<S2> items;
        S2 item;
        while (in >> item) {
            items.push_back(item);
        }
        
        delete[] data;
        count = static_cast<int>(items.size());
        data = new S2[count];
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    int GetCount() {
        return count;
    }

    S2 GetStruct(int& i) {
        if(i < count)
            return data[i];
        S2 mock;
        return mock;
    }

    void Sort() override {
        std::sort(data, data + count, [](const S2& a, const S2& b) {
            return a.Name > b.Name;
        });
    }

    void AlphabetSort() {
        std::sort(data, data + count, [](const S2& a, const S2& b) {
            return a.Name < b.Name;
        });
    }

    int findByName(std::string& query_name) {   
        for(int i = 0; i < count; i++) {
            if(data[i].Name == query_name)
                return i;
        }
        return -1;
    }
    
    void Merge(Bag& a, Bag& b)  {
        std::vector<S2> merging;
        for(int i = 0; i < a.count; i++) {
            if(b.findByName(a.data[i].Name) != -1 && 
                std::find(merging.begin(), merging.end(), a.data[i]) == merging.end()

            ) {
                merging.push_back(a.data[i]);
            }
        }
        delete[] data;
        count = static_cast<int>(merging.size());
        data = new S2[count];
        for (int i = 0; i < count; ++i) {
            data[i] = merging[i];
        }
    }
    
};

void operationA(Fruit& M1, Fruit& M2) {
    std::ofstream outputFile("outputA.txt");
    Fruit M3;
    M3.Merge(M1, M2);

    M3.WriteData(outputFile);
    std::cout << "Пересечение M1 и M2:\n";
    M3.WriteData(std::cout);
}

void operationB(Bag& B1, Bag& B2) {
    std::ofstream outputFile("outputB.txt");
    Bag B3;
    B3.Merge(B1, B2);

    B3.WriteData(outputFile);
    std::cout << "Пересечение B1 и B2:\n";
    B3.WriteData(std::cout);
}

void operationC(Fruit& a) {
    ABS_COMP* X;
    X = &a;
    a.AlphabetSort();
    std::cout << "Упорядочкенный массив:\n";
    a.WriteData(std::cout);
}

void operationC(Bag& a) {
    a.AlphabetSort();
    std::cout << "Упорядочкенный массив:\n";
    a.WriteData(std::cout);
}

void operationD(Fruit& a, std::string query_name) {
    int it = a.findByName(query_name);
    if(it == -1) {
        std::cout << "Товар не найден.\n";
    } else {
        S1 item = a.GetStruct(it);
        std::cout << "Имя - " << item.Name
                    << "Секция - " << item.Section;
    }
}

void operationD(Bag& a, std::string query_name) {
    int it = a.findByName(query_name);
    if(it == -1) {
        std::cout << "Товар не найден.\n";
    } else {
        S2 item = a.GetStruct(it);
        std::cout << "Имя - " << item.Name
                    << "Секция - " << item.Section
                    << "Количество на складе - " << item.quantity;
    }
}

int main() {
    Fruit M1, M2;
    Bag B1, B2;

    std::ifstream file1("M1.txt");
    std::ifstream file2("M2.txt");
    std::ifstream file3("B1.txt");
    std::ifstream file4("B2.txt");

    std::cout << "M1:\n";
    M1.PutData(file1);
    M1.WriteData(std::cout);

    std::cout << "M2:\n";
    M2.PutData(file2);
    M2.WriteData(std::cout);

    std::cout << "B1:\n";
    B1.PutData(file3);
    B1.WriteData(std::cout);

    std::cout << "B2:\n";
    B2.PutData(file4);
    B2.WriteData(std::cout);


    char choice;
    do {
        std::cout << "\nМеню операций:\n"
             << "a - Работа с фруктами (Fruit)\n"
             << "b - Работа с сумками (Bag)\n"
             << "c - Упорядочить по алфавиту массив\n"
             << "d - Поиск по имени в массиве\n"
             << "q - Выход\n"
             << "Выберите операцию: ";
        std::cin >> choice;

        switch (choice) {
            case 'a': 
                operationA(M1, M2);
                break;
            case 'b': 
                operationB(B1, B2);
                break;
            case 'c': 
                std::cout << "Выберите массив (1 - M1, 2 - M2, 3 - B1, 4 - B2): ";
                int type; std::cin >> type;
                switch (type) {
                    case 1:
                        operationC(M1);
                        break;
                    case 2:
                        operationC(M2);
                        break;
                    case 3:
                        operationC(B1);
                        break;
                    case 4:
                        operationC(B2);
                        break;
                    default: std::cout << "Неверный выбор!\n";
                }
                break;
            case 'd': 
                // std::string name = "Apple";
                // std::cout << "Выберите массив (1 - M1, 2 - M2, 3 - B1, 4 - B2): ";
                // int type; std::cin >> type;
                // switch (type) {
                //     case 1:
                //         operationD(M1, name);
                //         break;
                //     case 2:
                //         operationD(M2, name);
                //         break;
                //     case 3:
                //         operationD(B1, name);
                //         break;
                //     case 4:
                //         operationD(B2, name);
                //         break;
                //     default: std::cout << "Неверный выбор!\n";
                // }
                break;
            case 'q': break;
            default: std::cout << "Неверный выбор!\n";
            }
        } while (choice != 'q');

    return 0;
}