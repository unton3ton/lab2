#include <stdio.h>
#include <math.h>
//#include <conio.h> // библиотека, необходимая для работы оператора getch();
#include <iostream.h>

const int count = 11000;
/* количество элементов в массиве */
const double h = 1e-2;
/* выбранная точность шага*/
const double a = 6.0;
const double q = 2.0;

double f( double x, double y ) { //функция правой части уравнения Матьё
    return ( 2 * q * cos( 2 * x ) - a ) * y;
    /* для уравнения Матьё y''= ( 2 * q * cos( 2 * x ) - a ) * y */
}

void eler( double *x, double *y, double *p ) { // процедура для метода Эйлера
    x[0] = 0.0; // x_0
    y[0] = 1.0; // НУ y(x_0)=y_0
    p[0] = 0.0; // НУ y'(x_0)=y'_0

     /*далее записываем систему уравнений */
    for ( int i = 1; i < count; i++ ) {
        y[i] = y[i-1] + h * p[i-1];
        p[i] = p[i-1] + h * f( x[i-1], y[i-1] );
        x[i] = x[i-1] + h;
    }
}

void eler_pro( double *x, double *y, double *p, double *y1, double *p1 ) {
     /* процедура для метода прогноза-коррекции*/
     x[0] = 0.0; // x_0
    y[0] = 1.0; // НУ y(x_0)=y_0
    p[0] = 0.0; // НУ y'(x_0)=y'_0

    for ( int i = 1; i < count; i++ ) {
        x[i] = x[i-1] + h;
        y1[i] = y[i-1] + h * p[i-1];
        p1[i] = p[i-1] + h * f( x[i-1], y[i-1] );
        y[i] = y[i-1] + 0.5 * h * ( p[i-1] + p1[i] );
        p[i]= p[i-1] + 0.5 * h * (f( x[i-1], y[i-1] )+f( x[i], y1[i] ));
    }
}

void runge(double *x, double *y, double *p ) {
     /*процедура для метода Рунге-Кутты 4-го порядка */
     double k1, k2, k3, k4;
     double m1, m2, m3, m4;

     x[0] = 0.0; // x_0
    y[0] = 1.0; // НУ y(x_0)=y_0
    p[0] = 0.0; // НУ y'(x_0)=y'_0

    for ( int i = 1; i < count; i++ ) {
    k1=p[i-1];
    m1=f( x[i-1], y[i-1] );
    k2=p[i-1]+0.5*h*m1;
    m2=f( x[i-1]+0.5*h, y[i-1]+0.5*h*k1 );
    k3=p[i-1]+0.5*h*m2;
    m3=f( x[i-1]+0.5*h, y[i-1]+0.5*h*k2 );
    k4=p[i-1]+h*m3;
    m4=f( x[i-1]+h, y[i-1]+h*k3 );
    x[i] = x[i-1] + h;
    y[i] = y[i-1] +h*(k1+2*k2+2*k3+k4)/6;
    p[i] = p[i-1] +h*(m1+2*m2+2*m3+m4)/6;
    }
}

int main( void ) { // функция реализующая процедуры
    double x[count], y[count], p[count], y1[count],y2[count],
    y3[count], p1[count]; // объявление массивов
    
    FILE *f;
    f = fopen("file.txt", "w"); // создаём файл для записи точек

    eler( x, y, p ); // вызов функции для решения методом Эйлера
     
     /*for ( int i = 1; i < count; i++ ) {
         fprintf(f, "%f %f\n", x[i], y[i],y2[i],y3[i] ); 
        // печатает таблицу данных x[i], y[i] на экране терминала
    }  
    cout << "for Eler" << endl;*/

    eler_pro(x, y2, p, y1, p1); 
    // вызов функции для решения методом прогноза-коррекции
    
    runge(x, y3, p );
    // вызов функции для решения методом Рунге-Кутты
    
    for ( int i = 1; i < count; i++ ) {
        fprintf(f, "%f %f %f %f\n", x[i],y[i], y2[i],y3[i] ); 
        /* печатает таблицу данных x[i], y[i],y2[i],y3[i] в файл file.txt*/
    }
    getchar(); // Команда задержки экрана
    return 0;
}
