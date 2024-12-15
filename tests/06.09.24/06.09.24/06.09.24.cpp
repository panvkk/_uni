#include <iostream>
using namespace std;

int n1,n2;




int main()
{
    cout << "Input 1 and 2 numbers: \n";
    cin >> n1 >> n2;
    if(n1>n2)
        cout << n1 << endl;
    if (n2 > n1)
        cout << n2 << endl;
    else
        cout << n1 << "=" << n2 << endl;
    return 0;
}