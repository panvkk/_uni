#include <iostream>
#include <string>

class cryptoCurrency {
        float value;
        std::string cyrrency_name; 
    public:
        cryptoCurrency() : value(0.0) , cyrrency_name("cyrrency")
            {}
        cryptoCurrency(float v, std::string cn) : value(v) , cyrrency_name(cn)
            {   std::cout << "Новая валюта создана!\n";   }
        float getValue() {
            return value;
        }
        std::string getCurrencyName() {
            return cyrrency_name;
        }
};

class mainActivity {
    
    public:
        int main() {
            cryptoCurrency Bitcoin(93890.8, "bitcoin");
            std::cout << "Валюта: "  << Bitcoin.getCurrencyName;
            std::cout << "\nСтоимость монеты: "  << Bitcoin.getValue;
            return 0;
        }
};