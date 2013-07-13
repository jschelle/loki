#include "prime.h"
#include <stdlib.h>

bool prime::miller_rabin(long x)
{
    // exceptions
    if(x<=1)
        return false;
    if(x==2)
        return true;
    // random number 0 < a < x
    long a = rand() % x;
    while(a==0)
        a = rand() % x;
    // log base 2 of x-1
    int n = 0;
    long x2 = x-1;
    while(x2!=1)
    {
        x2/=2;
        n++;
    }
    // square and multiply method
    long pot = 1;
    bool antecedent = false;
    for(int i=n; i>=0; i--)
    {
        int b = ((x-1) & (1<<i))!=0;
        antecedent = false;
        if(pot!=1 && pot!=x-1)
            antecedent = true;
        pot = ( pot * pot ) % x;
        if(antecedent && pot==1)
        {
            return false;
        }
        if(b)
            pot = ( pot * a );
        pot = pot % x;
    }
    if(pot!=1)
    {
        return false;
    }
    return true;
}
bool prime::is_prime(long x)
{
    for(int i=0; i<50; i++)
    {
        if(!miller_rabin(x))
        {
            return false;
        }
    }
    return true;
}
long prime::next_prime(long x)
{
    x++;
    while(!is_prime(x))
        x++;
    return x;
}
