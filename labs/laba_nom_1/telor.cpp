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
    } while( x <= 2.1);
    return 0;
}

double factorial(double n)
{   
    if(n>1)
        return n*factorial(n-1);
    else
        return 1;
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
double sinus(double x)
{
    double sum = 0, iter = 0;
    do{
        sum+=pow(-1, iter)*(pow(x , 2*iter + 1) / factorial(2*iter + 1));
        iter++;
    }while( fabs(pow(-1, iter)*pow(x , 2*iter + 1) / factorial(2*iter + 1)) > 0.001 );
    return sum;
}
double cosinus(double x)
{
    double sum = 0, iter = 0;
    do{
        sum+=pow(-1, iter)*(pow(x , 2*iter) / factorial(2*iter));
        iter++;
    }while( fabs(pow(-1, iter)*pow(x , 2*iter) / factorial(2*iter)) > 0.001 );
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
double koren(double x)
{
    double sum = 0, iter = 0;
    double term;
    x--;
    do{
        term = (pow(-1, iter) * factorial(2*iter) * pow( x, iter))/ ((1-2*iter) * (pow(factorial(iter), 2))* (pow( 4, iter)));
        sum+=term;
        iter++;
    }while( fabs(term) > 0.001 );
    return sum;
}