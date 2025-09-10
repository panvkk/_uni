#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

class wheel {
        std::string material;
        float radius;
        int needle_amount;
    public: 
        wheel() : material("none") , radius(0.0) , needle_amount(0)
            {}
        wheel(std::string mat , float rad, int ne_am): material(mat) , radius(rad) , needle_amount(ne_am)
            {}
        void setMaterial(std::string line) {
            material = line;
        }
        void setRadius(float value) {
            material = value;
        }
        void setNeedle_Amount(int quantity) {
            material = quantity;
        }
        std::string getMaterial() const     { return material; }
        float getRadius() const     { return radius; }
        int getNeedle_Amount() const     { return needle_amount; }
};

std::ostream& operator << (std::ostream& os, wheel& wh) {
    return os << "Радиус: " << wh.getRadius() << "\nМатериал изготовления: " << wh.getMaterial() << "\nКоличество спиц: " << wh.getNeedle_Amount() << "\n";
}

void findStatistics(std::vector<wheel>&);


int main() {
    std::vector<wheel> few_wheels;
    char ch;
    std::cout << "Ввод данных\n\n";
    while(ch != 'n') {
        std::string mat;
        float rad;
        int ne_am;
        std::cout << "Введите материал колеса: ";
        std::cin >> mat;
        std::cout << "Введите количество спиц колеса: ";
        std::cin >> ne_am;
        std::cout << "Введите радиус колеса: ";
        std::cin >> rad;

        wheel current(mat, rad, ne_am);
        few_wheels.push_back(current);
        std::cout << "Продолжить?(y,n) \n";
        std::cin >> ch;
    }
    findStatistics(few_wheels);
    return 0;
}

void findStatistics(std::vector<wheel>& wh) {
    float average_radius = 0;
    int average_needle_amount = 0;
    
    int max_radius_element;
    float max_radius = 0;

    for(int i = 0; i < wh.size(); i++) {
        if(wh[i].getRadius() > max_radius) {
            max_radius = wh[i].getRadius();
            max_radius_element = i;
        }
        average_needle_amount += wh[i].getNeedle_Amount();
        average_radius += wh[i].getRadius();
    }
    std::cout << "\nСредний радиус колеса: " << average_radius/wh.size();
    std::cout << "\nСреднее количество спиц колеса: " << average_needle_amount/wh.size();
    std::cout << "\n\nСамое больше колесо: \n" << wh[max_radius_element]; 
}