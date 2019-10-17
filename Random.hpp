#ifndef RANDOM
#define RANDOM

#include<cmath>

/*
contains two functions - one returns ints, other returns doubles
both modify the seed during execution
*/


//returns a scaled pseudorandom I4 between a and b (inclusive)
int i4_uniform_ab ( int a, int b, int &seed ){

    int c;
    const int i4_huge = 2147483647;
    int k;
    float r;
    int value;
  
    //Guarantee A <= B
    if ( b < a ){    c = a;    a = b;    b = c;}

    k = seed / 127773;
    seed = 16807 * ( seed - k * 127773 ) - k * 2836;
    if ( seed < 0 ){    seed = seed + i4_huge;}
    r = ( float ) ( seed ) * 4.656612875E-10;

    //Scale R to lie between A-0.5 and B+0.5.

    r = (1.0 - r) * (( float )a - 0.5) + r * (( float )b + 0.5);
    value = round(r);
    if ( value < a ){    value = a;}
    if ( b < value ){    value = b;}
    return value;
}

//returns a scaled pseudorandom R8 between a and b (inclusive)
double r8_uniform_ab ( double a, double b, int &seed ){

    int i4_huge = 2147483647;
    int k;
    double value;
    k = seed / 127773;
    seed = 16807 * ( seed - k * 127773 ) - k * 2836;
    if ( seed < 0 ){    seed = seed + i4_huge;}
    value = ( double ) ( seed ) * 4.656612875E-10;
    value = a + ( b - a ) * value;
    return value;
}

#endif