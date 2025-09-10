#include <iostream>
                                    // Вариант 3.
int num(int);
bool prost(int);

int main()
{
    int a,b,c,x;
    std::cout << "Vvedite 3 natural'nih chisla: \n";
    std::cin >> a >> b>> c;
    std::cout << std::endl;
    if(a<1 or b<1 or c<1)
    {
        std::cout << "Vvedeni nevernie dannie";
        exit(0);
    }
    int s = num(a), y = num(b), z = num(c);
    if(s>y && s>z)
    {
        std::cout << a << std::endl;
        x=a;
    }
        else if(z>s && z>y)
        {
            std::cout << c << std::endl;
            x=c;
        }
            else if(y>s && y>z)
            {
                std::cout << b << std::endl;
                x=b;
            }
                else if(s==z)
                    {
                        std::cout << a << std::endl;
                        x = a;
                    }
                        else if(s==y)
                        {
                            std::cout << b << std::endl;
                            x = a;
                        }
                            else if(y==z)
                            {
                                std::cout << c << std::endl;
                                x = b;
                            }
    if(prost(x) == 1)
        std::cout << "Chislo prostoe\n";
    else
        std::cout << "Chislo ne prostoe\n";
    std::cout << "1/x = " << 1/(double)x << std::endl;
    return 0;
}
int num(int h)
{
    int j = 0;
    do{
        h/=10;
        j++;
    }while(h>=1);
    return j;
}
bool prost(int m)
{
    for(int l = 2; l<m; l++)
    {
        if(m%l==0)
            return 0;
        else    
            continue;
    }
    return 1;
}