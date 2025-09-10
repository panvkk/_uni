#include <iostream>

int main()
{
    int ;
    return 0;
}

int Function::nod(int a, int b) {
    if (a < b) {
        std::swap(a, b);
    }
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}
int Function::nok(int x, int y)
{
    for(int j = 1; j!=0; j++)
    {
        if(j%x==0 && j%y==0)
        {
            return j;
        }
    }
}
void Function::palindrom(int a)
{
    int b=0;
    int i = a;
    while(i>=1)
    {
        b*=10;
        b+=i%10;
        i/=10;
    }
    if (b==a)
        std::cout << "Число является палиндромом \n";
    else
        std::cout << "Число не является палиндромом \n";
}
bool prost(int prosto)
{
    for(int j = 2; j < prosto; j++)
    {
        if(prosto%j==0)
        {
            return 0;
        }
    }
    return 1;
}

int Function::sum(int a)
{
    int s=0;
    while(a>0)
    {
        s+=a%10;
        a/=10;
    }
    return s;
}
int Function::kolv(int a)
{
    int k =0;
    while(a > 0)
    {
        a/=10;
        k++;
    }
    return k;
}
void Function::stepen(int a,int b)
{
    for(int i = 1; i <= b; i++)
    {
        a*=a;
    }
    std::cout << a;
}
void Function::fibon(int n)
{
    int a=1,b,c;
    for(int i = 1; i != n-1; i++)
    {
        c=b;
        b=a;
        a = b+c;
    }
    std::cout << a;
}
void razl(int k)
{
    int sch=0;
    for(int j = 0; j <= 9; j++)
    {
        int a = k,m;
        while(a>0)
        {
            m=a%10;
            a/=10;
            if(m==j)
            {
                sch++;
                break;
            }
        }
    }
    std::cout << std::endl << sch;
}
void naib(int k)
{
    for(int j = 9; j >= 0; j--)
    {
        int a = k,m;
        while(a>0)
        {
            m=a%10;
            a/=10;
            if(m==j)
            {
                std::cout << std::endl << "Наибольшая цифра: " << j;
                return;
            }
        }
    }
}
void delit(int k)
{
    std::cout << "\nПростые делители: ";
    for(int i = 1; i <= k; i++)
    {
        if((k%i)==0)
        {
            if(prost(i) == 1)
            {
                std::cout << i << " ";
                k/=i;
            }
        }
    }
}