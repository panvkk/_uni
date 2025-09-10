#include <iostream>
using namespace std;

  int main()
  {
    setlocale(LC_ALL, "Russian");
    int  counter, fnumber, lnumber, сумма = 0, сумми = 0;
    counter = 0;
    cin >> fnumber;
    cin >> lnumber;
    while (fnumber != 0)
    {
      сумма += fnumber % 10;
      fnumber /= 10;
      
    }
    while (lnumber != 0)
    {
      сумми += lnumber % 10;
      lnumber /= 10;
    }
    if (сумма == сумми) {
      counter++;
    }
    cout << counter;
  }