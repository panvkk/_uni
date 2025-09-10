#include <iostream>
/*
С клавиатуры вводится целое число n, необходимо используя С осуществить перевод числа в шестнадцатиричную.
Получить как массив символов.
64 = 4 0
63 = 3 F ...
*/
int main()
{
    int n;
    int rest, counter;
    unsigned int shest = 16;
    char array[10];
    /*for (auto& i : array)
        i = '\0';*/
    std::cout << "Введите цеоле число.\n";
    std::cin >> n;
    /*counter = 9;
    while (n > 15) {
        rest = n % 16;
        n /= 16;
        rest < 10 ? rest += 48 : rest += 55;
        array[counter] = char(rest);
        counter--;
    }
    array[counter] = char(n);
    std::cout << "Шестнадцатиричная система: \n";
    for (int i : array) {
        std::cout << i;
    }*/
    //Решаем на ассемблере
    __asm {
        mov ecx, 0
        mov eax, n
        mov esi, 0
        povt:
        cmp eax, 0
            je vihod
            mov edx, 0
            div shest
            cmp edx, 9
            jg bolshe
            add edx, 48
            jmp obhod
            bolshe: 
                add edx, 55
            obhod:
                mov [array + esi], edx 
                inc esi
                jmp povt
         vihod: 
         /*mov esi, 0
         zuzu:
            pop edx
            cmp dl, 9
            jg bolshe
            add dl, 48
            jmp obhod 
            bolshe: 
                add dl, 55
            obhod: 
                mov [array + esi], dl
                inc esi 
                loop zuzu*/
        mov [array + esi], 0
    }
    /*
    Далее необходимо с asm в этом же массиве получить число задом наперёд, т е:
    65 - 1 4 
    64 - 0 4 
    */
    std::cout << "Результат в шестнадцатеричной: \n";
    for (int i = 0; array[i] != '\0'; i++)
        std::cout << array[i];
    return 0;
}
/*
ДЗ - оформить в виде процедуры на asm преобразование целого числа в 16-систему исчисления
входные параметры: исходное число, адрес массива, где должен быть получен результат
*/