#include <iostream>

int main()
{
    srand(time(0));
    enum { MAX = 7, PEAK = 10 };
    int mass[MAX];
    for(int l = 0; l <= MAX-1; l++) {   
        mass[l] = rand()%PEAK;
    }
    for(int l = 0; l <= MAX-1; l++)
    {   std::cout << mass[l] << " ";    }
    std::cout << "\n";
    int peakSuit;
    int peakAmount;
    for ( int i = 0; i < MAX; i++) {
        int suit = mass[i];
        int amount = 0;
        for (int j = 0; j < MAX; j++) {
            if(mass[j] == suit)
                amount++;
        }
        if (peakAmount < amount) {
            peakAmount = amount; peakSuit = suit;
        }
    }
    std::cout << "Кол-во повторений: " << peakAmount << "\nЭлемент: " << peakSuit;
    return 0;
} 