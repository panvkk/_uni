#include <iostream>
#include <cmath>
#include <iomanip>

double factorial(double), koren(double), log(double), cosinus(double), sinus(double), exponenta(double);
void stars()
{
    for(int j = 1; j < 130; j++)
        { std::cout << "*"; }
}
int main()
{
    std::cout << std::setw(6) << "x" ;
    std::cout << std::setw(35) << "Разложение корня";
    std::cout << std::setw(35) << "Разложение cos(x)";
    std::cout << std::setw(35) << "Разложение sin(x)";
    std::cout << std::setw(35) << "Разложение ln(x)";
    std::cout << std::setw(35) << "Разложение e^x" << std::endl;
    stars();
    std::cout << "\n";
    double x = 0.1;
    do {
        std::cout << std::setw(6) << x ;
        std::cout << std::setw(20) << koren(x);
        std::cout << std::setw(20) << cosinus(x);
        std::cout << std::setw(20) << sinus(x);
        std::cout << std::setw(30) << log(x);
        std::cout << std::setw(30) << exponenta(x) << std::endl;
        stars();
        std::cout << "\n";
        x+=0.1;
    } while( x < 2 );
    return 0;
}

double factorial(double n)
{   
    double k = 1;
    for( double j = 1; j <= n; ++j )
    {
        k *= j;
    }
    return k;
}
double exponenta(double x)
{
    double sum = 0, iter = 0;
    do{
        sum += pow( x, iter )  / factorial(iter);
        iter++;
    }while( pow( x, iter )  / factorial(iter) > 0.001 );
    return sum;
}
double log(double x)
{
    double sum = 0, iter = 1;
    x--;
    do{
        sum+=pow(-1, iter + 1) * pow(x , iter)/ iter;
        iter++;
    }while( fabs(pow(-1, iter + 1) * pow(x , iter) / iter) > 0.001 );
    return sum;
}