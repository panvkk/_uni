/*
Шаблонированные типы данных. 
Условия задач с MyList. 
Определить класс шаблонированный майлист и используя этот класс решить следующие 3 задачи: 

1. Ввести с клавиатуры последовательность целых чисел, которая оканчивается вводом двух следующих друг за другом чисел - 
0 и 1, причём числа 0 и 1 не являются членами последовательности. Создать список, в информационной части которого целые 
числа и опеределить максимальное значение в этом списке. Вывести на экран максимальное значение и сам список,
после чего удалить из этого списка все значения, совпадающие с максимальным.

2. Ввести с клавиатуры последовательность действительных чисел, используя средства того же класса. Организовать список с действительной
информационной частью, но таким образом, чтобы этот список формировался уже отсортированным по невозрастанию, после чего
удалить из этого списка все элементы, включая самый первый, которые равны этому первому. Вывести на экран

3. Ввести с клавиатуры последовательность символов, которая оканчивается вводом символа #. Организовать список, в информацинной
части которой находится 1 символ. Полученный список вывести на экран, после чего в этом списке подсчитать так называемую 
частоту повторения символов, которые встречаются в этой последовательности. Например, вводится мама мыла раму #. Вывести информацию:
Символ пробел - 2 раза
а - 4 раза
... 


Решение этих 3 задач в одном .cpp и .exe прислать на почту до конца дня.


Замечание. Класс это абстрактный тип данных, определяющийся множествов значений и функций над ними. Классы определяются для того,
чтобы любой кодировщик мог используя эти функции и значения, составлять свои тексты на языке записи алгоритмов, будь то С, будь
гадюка и прочие. При этом, любой класс должен быть совершенен, то есть замкнут и вполне ограничен. Никаких лишних 
фукнций и данных в этом классе не должно быть. Нельзя класс MyList добавлять функции, например, сортировки, либо фукнции 
вставки в нужное место некоторого значения. Поэтому необходимо описать полностью класс и дальше решая первую задачу, а также
вторую и третью, нужно определять переменные соответствующего класса и над ними выполнять соответствующие действия.
В частности для решения первой задачи, должна быть переменная типа MyList, с указанием типа int, и дальше нужно осуществлять ввод
значений, путём добавления либо в голову, либо в хвост, вводимых с клавиатуры значений.

Замечание. В условии для while никогда нельзя использовать использовать операцию || потому что такое условие с большой 
вероятностью будет равно истине и цикл будет бесконечен. Там, где используется while нужно использовать только &&(операцию И)
Замечание. Никогда не начинать состовление текста с операции while. 

Замечание. При решении 3 задачи после организации списка символов, необходимо для подсчёта частоты повторяемости завести 
массив, состоящий из 256 элементов, равных 0, после чего, взяв очередной элемент из списка, в ячейку с номером равным коду символа
нужно постоянно добавлять единицу. После обработки всех символов элементы массива не равные нулю нужно вывести на экран, 
с указанием "символ <> встречается <> раз" и тд. При этом, для решения этой задачни нужно будет от класса MyList наследовать новый
класс, в котором добавить функцию getValueByPointer().

Замечание. Для решения 1 и 3 задач в файле необходимо в пункте 4 изменить функцию нахождения значения по указателю, чтобы возвра
щаемое значение было типа T. 
*/
#include <iostream>


template<typename T>
class MyList {
    public:
        struct node {
            T data;
            node* next;
        };
    protected:
        node* head;
    public:

        MyList() : head(NULL) { }

        bool isEmpty() {
            return head ? false : true;
        }

        node* getHead() {
            return head;
        }

        void push_head(T data) {
            node* temp;
            temp = new node;
            if(isEmpty()) {
                head = temp;
            } else {
                temp->next = head;
                head = temp;
            }
        }
        void push_tail(T data) {
            node* temp;
            if (isEmpty()) { 
                head = new node( ); 
                head->next = NULL; 
                head->data = data; 
            } else { 
                node* temp = head; 
                while (temp->next) { 
                    temp = temp->next; 
                }
                temp->next = new node;
                temp->next->next = NULL;
                temp->next->data = data; 
            }
        } 
        void showList() {
            node* temp = head;
            if (isEmpty()) 
                std::cout << "out: <пусто>" << std::endl;
            else { 
                std::cout << "out: [ " << temp->data;
                temp = temp->next;
                while (temp) { 
                    std::cout << ", " << temp->data; 
                    temp = temp->next; 
                }
                std::cout << " ]" << std::endl; 
            }    
        }
        node* findValueByData(T data){
            node* temp = head;
            while (temp != NULL) { 
                if (temp->data == data) return temp; temp = temp->next; 
            }
            return NULL; 
        }
        void deleteElement(T data) {
            node* temp = findValueByData(data);
            node* save = head;
            if (!isEmpty() && temp != NULL) {
                if (temp == head) { 
                    node* temp = head; 
                    temp = temp->next; 
                    delete head; 
                    head = temp; 
                }
                else { 
                    while (save->next != temp) save = save->next; save->next = temp->next; delete temp; 
                }
            }
        }
        void deleteElement(node* value) {
            node* curr = head;
            
            if(value != head) {
                while(curr->next != value) {
                    curr = curr->next;
                }
                if(value->next == NULL) {
                    curr->next = NULL;
                    delete value;
                } else {
                    curr->next = value->next;
                    delete value;
                }
            } 
            else {
                head = head->next;
                delete value;
            }
        }
        int findMaxValue(node*& pointer_to_max) {
            int max_element;
            node* curr;
            max_element = 0;
            curr = head;
            while(curr) {
                if(curr->data > max_element) {
                    max_element = curr->data;
                    pointer_to_max = curr;
                } else {
                    curr = curr->next;
                }
            }
            return max_element;
        }
        int findMaxValue() {
            int max_element;
            node* curr;
            max_element = 0;
            curr = head;
            while(curr) {
                if(curr->data > max_element) {
                    max_element = curr->data;
                } else {
                    curr = curr->next;
                }
            }
            return max_element;
        }
        
};

template<typename T>
void getList(MyList<T>&, bool);

template<typename T>
void sortList(MyList<T>& List);


int main() {
    setlocale(LC_ALL, "Russian");


    // Задача 1
    std::cout << "Task 1:\n";

    MyList<int> pyataya_kolonna;
    getList<int>(pyataya_kolonna, false);
    int max_value = pyataya_kolonna.findMaxValue();
    pyataya_kolonna.deleteElement(max_value);
    pyataya_kolonna.showList();

    // Задача 2
    std::cout << "Task 2:\n";

    MyList<float> task_two_list;
    getList<float>(task_two_list, true);
    task_two_list.showList();

    // Задача 3

    std::cout << "Task 3 (Only English):\n";

    MyList<char> task_three_list;
    int symbols[256];
    for(int i = 1; i < 256; i++) {
        symbols[i] = 0;
    }

    bool induk = true;
    char ch;
    while(std::cin.get(ch) && ch != '#') {
        task_three_list.push_tail(ch);
    }
    
    auto curr = task_three_list.getHead();
    while(curr) {
        symbols[curr->data] = symbols[curr->data] + 1;
        curr = curr->next;
    }

    for(int i = 1; i < 256; i++) {
        if(symbols[i] != 0) {
            char character = static_cast<char>(i);
            std::cout << character << " - " << symbols[i] << " times\n"; 
        }
    }

    system("pause");
}

template<typename T>
void getList(MyList<T>& List, bool needToSort) {
    T data, previous;
    data = 1;
    previous = 1;
    bool induk = true;
    while(induk) {
        std::cin >> data;
        if(data == 1 && previous == 0) {
            induk = false;
        } else {
            List.push_tail(data);
            previous = data;
            if(needToSort) {
                sortList<T>(List);
            }
        }
    }
    auto curr = List.getHead();
    while(curr->next) {
        curr = curr->next;
    }
    List.deleteElement(curr);
}

template<typename T>
void sortList(MyList<T>& List) {
    auto curr = List.getHead();
    while(curr) {
        T max = curr->data;
        auto reffer_to_max = List.getHead();
        auto local_curr = List.getHead();

        reffer_to_max = curr;
        local_curr = curr;
        while(local_curr) {
            if(max < local_curr->data) {
                max = local_curr->data;
                reffer_to_max = local_curr;
            } 
            local_curr = local_curr->next;
        }
        std::swap(curr->data, reffer_to_max->data);
        curr = curr->next;
    }
}
