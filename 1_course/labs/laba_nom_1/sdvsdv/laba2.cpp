#include <iostream>
using namespace std;

int main()
{
    int m;
    cout << "Введите число: ";
    cin >> m;
    for(int j = 100; j<=999; j++)
    {
        int a,b,c;
        int i=j;
        c=(i%10);
        i/=10;
        b=(i%10);
        i/=10;
        a=(i%10);
        i/=10;
        if(((a*a*a)+(b*b*b)+(c*c*c))==m)
        {
            cout << endl << "Наименьшее: " << j; 
            break;
        }
    }
    for(int j = 999; j>=100; j--)
    {
        int a,b,c;
        int i=j;
        c=(i%10);
        i/=10;
        b=(i%10);
        i/=10;
        a=(i%10);
        i/=10;
        if(((a*a*a)+(b*b*b)+(c*c*c))==m)
        {
            cout << endl << "Наибольшее: " << j; 
            break;
        }
    }
    return 0; 
}