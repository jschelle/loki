
#ifndef PRIME_H
#define PRIME_H

class prime
{
public:
    static bool is_prime(long l);
    static long next_prime(long l);
private:
    static bool miller_rabin(long l);
};

#endif
