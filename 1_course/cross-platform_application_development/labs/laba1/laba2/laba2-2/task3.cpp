#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

using namespace std;

struct S1 {
    char Name[20];
    bool Red;
    int Price;
};

struct S2 {
    char Name[20];
    int Num;
    bool Red;
};

struct S3 {
    char Name[20];
    int Price;
    int Num;
};

bool operator==(const S1& a, const S2& b) {
    return strcmp(a.Name, b.Name) == 0;
}

bool operator==(const S1& a, const S1& b) {
    return strcmp(a.Name, b.Name) == 0;
}

bool operator==(const S2& a, const S2& b) {
    return strcmp(a.Name, b.Name) == 0;
}

ostream& operator<<(ostream& os, const S1& s) {
    os << s.Name << " " << (s.Red ? "true" : "false") << " " << s.Price;
    return os;
}

ostream& operator<<(ostream& os, const S2& s) {
    os << s.Name << " " << s.Num << " " << (s.Red ? "true" : "false");
    return os;
}

ostream& operator<<(ostream& os, const S3& s) {
    os << s.Name << " " << s.Price << " " << s.Num;
    return os;
}

class ABS_LIB {
public:
    virtual ~ABS_LIB() = default;
    virtual void ReadData(istream& in) = 0;
    virtual void WriteData(ostream& out) = 0;
    virtual void PutData(ostream& out) = 0;
    virtual ABS_LIB* Intersect(ABS_LIB* a, ABS_LIB* b) = 0;
    virtual void RemoveDuplicates() = 0;
    virtual void Sort() = 0;
    virtual void FindAndPrint(const char* name) = 0;
};

class Prod : public ABS_LIB {
    S1* data;
    int count;
public:
    Prod() : data(nullptr), count(0) {}
    ~Prod() override { delete[] data; }

    void ReadData(istream& in) override {
        in.read((char*)&count, sizeof(int));
        data = new S1[count];
        in.read((char*)data, count * sizeof(S1));
    }

    void WriteData(ostream& out) override {
        out.write((char*)&count, sizeof(int));
        out.write((char*)data, count * sizeof(S1));
    }

    void PutData(ostream& out) override {
        for (int i = 0; i < count; ++i)
            out << data[i] << endl;
    }

    ABS_LIB* Intersect(ABS_LIB* a, ABS_LIB* b) override { return nullptr; }

    void RemoveDuplicates() override {
        int newCount = 0;
        for (int i = 0; i < count; ++i) {
            bool found = false;
            for (int j = 0; j < newCount; ++j) {
                if (strcmp(data[i].Name, data[j].Name) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                data[newCount++] = data[i];
            }
        }
        count = newCount;
    }

    void Sort() override {
        sort(data, data + count, [](const S1& a, const S1& b) {
            return strcmp(a.Name, b.Name) < 0;
        });
    }

    void FindAndPrint(const char* name) override {
        for (int i = 0; i < count; ++i) {
            if (strcmp(data[i].Name, name) == 0) {
                cout << data[i] << endl;
                return;
            }
        }
        cout << "Product not found" << endl;
    }

    int getCount() const { return count; }
    const S1* getData() const { return data; }
};

class Bag : public ABS_LIB {
    S2* data;
    int count;
public:
    Bag() : data(nullptr), count(0) {}
    ~Bag() override { delete[] data; }

    void ReadData(istream& in) override {
        in.read((char*)&count, sizeof(int));
        data = new S2[count];
        in.read((char*)data, count * sizeof(S2));
    }

    void WriteData(ostream& out) override {
        out.write((char*)&count, sizeof(int));
        out.write((char*)data, count * sizeof(S2));
    }

    void PutData(ostream& out) override {
        for (int i = 0; i < count; ++i)
            out << data[i] << endl;
    }

    ABS_LIB* Intersect(ABS_LIB* a, ABS_LIB* b) override { return nullptr; }

    void RemoveDuplicates() override {
        int newCount = 0;
        for (int i = 0; i < count; ++i) {
            bool found = false;
            for (int j = 0; j < newCount; ++j) {
                if (strcmp(data[i].Name, data[j].Name) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                data[newCount++] = data[i];
            }
        }
        count = newCount;
    }

    void Sort() override {
        sort(data, data + count, [](const S2& a, const S2& b) {
            return strcmp(a.Name, b.Name) < 0;
        });
    }

    void FindAndPrint(const char* name) override {
        for (int i = 0; i < count; ++i) {
            if (strcmp(data[i].Name, name) == 0) {
                cout << data[i] << endl;
                return;
            }
        }
        cout << "Product not found" << endl;
    }

    int getCount() const { return count; }
    const S2* getData() const { return data; }
};

class NotRedPrice : public ABS_LIB {
    S3* data;
    int count;
public:
    NotRedPrice() : data(nullptr), count(0) {}
    ~NotRedPrice() override { delete[] data; }

    void ReadData(istream& in) override {
        in.read((char*)&count, sizeof(int));
        data = new S3[count];
        in.read((char*)data, count * sizeof(S3));
    }

    void WriteData(ostream& out) override {
        out.write((char*)&count, sizeof(int));
        out.write((char*)data, count * sizeof(S3));
    }

    void PutData(ostream& out) override {
        for (int i = 0; i < count; ++i)
            out << data[i] << endl;
    }

    ABS_LIB* Intersect(ABS_LIB* a, ABS_LIB* b) override {
        Prod* p = dynamic_cast<Prod*>(a);
        Bag* bg = dynamic_cast<Bag*>(b);
        if (!p || !bg) return this;

        int newCount = 0;
        S3* newData = new S3[100];
        const S1* prodData = p->getData();
        const S2* bagData = bg->getData();
        int prodCount = p->getCount();
        int bagCount = bg->getCount();

        for (int i = 0; i < bagCount; ++i) {
            for (int j = 0; j < prodCount; ++j) {
                if (strcmp(bagData[i].Name, prodData[j].Name) == 0 && !prodData[j].Red) {
                    strcpy(newData[newCount].Name, prodData[j].Name);
                    newData[newCount].Price = prodData[j].Price;
                    newData[newCount].Num = bagData[i].Num;
                    newCount++;
                    break;
                }
            }
        }
        delete[] data;
        data = newData;
        count = newCount;
        return this;
    }

    void RemoveDuplicates() override {}
    void Sort() override {
        sort(data, data + count, [](const S3& a, const S3& b) {
            return strcmp(a.Name, b.Name) < 0;
        });
    }

    void FindAndPrint(const char* name) override {
        for (int i = 0; i < count; ++i) {
            if (strcmp(data[i].Name, name) == 0) {
                cout << data[i] << endl;
                return;
            }
        }
        cout << "Product not found" << endl;
    }
};

void createTestFiles() {
    ofstream f1("F1.bin", ios::binary);
    int count1 = 3;
    S1 prodData[] = {
        {"Sugar", false, 7500},
        {"Apple", true, 10000},
        {"Orange", false, 20000}
    };
    f1.write((char*)&count1, sizeof(int));
    f1.write((char*)prodData, count1 * sizeof(S1));
    f1.close();

    ofstream f2("F2.bin", ios::binary);
    int count2 = 2;
    S2 bagData[] = {
        {"Sugar", 1001, false},
        {"Orange", 100, false}
    };
    f2.write((char*)&count2, sizeof(int));
    f2.write((char*)bagData, count2 * sizeof(S2));
    f2.close();
}

int main() {
    createTestFiles();

    Prod M1;
    Bag K1;
    NotRedPrice R;

    ifstream in1("F1.bin", ios::binary);
    M1.ReadData(in1);
    in1.close();

    ifstream in2("F2.bin", ios::binary);
    K1.ReadData(in2);
    in2.close();

    M1.RemoveDuplicates();
    K1.RemoveDuplicates();

    cout << "M1 after remove duplicates:" << endl;
    M1.PutData(cout);
    cout << "\nK1 after remove duplicates:" << endl;
    K1.PutData(cout);

    ABS_LIB* A1 = &M1;
    ABS_LIB* A2 = &K1;
    ABS_LIB* A3 = &R;

    A3->Intersect(A1, A2);

    ofstream out1("Res.bin", ios::binary);
    A3->WriteData(out1);
    out1.close();

    ofstream out3("Intersect.txt");
    A3->PutData(out3);
    out3.close();

    A3->Sort();

    cout << "\nIntersection sorted:" << endl;
    A3->PutData(cout);

    cout << "\nSearch 'Sugar' in R:" << endl;
    R.FindAndPrint("Sugar");

    cout << "\nSearch 'Apple' in R:" << endl;
    R.FindAndPrint("Apple");

    return 0;
}