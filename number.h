#ifndef NUMBER_H
#define NUMBER_H

#include "atom.h"

class number:public atom
{
public:
    // meta-information
    virtual int get_type() = 0;
    // copy
    virtual atom* shallow_copy() = 0;
    // conversion
    virtual double get_double_value() = 0;
    virtual int get_int_value() = 0;
    virtual long get_long_value() = 0;
    // equality
    virtual bool same_class(atom* a) = 0;
    // evaluation
    virtual atom* evaluate(context* c) = 0;
    // mathML
    virtual std::string to_mathML() = 0;
    // UTIL
    static bool is_number(atom* a);
    static number* add(number* n0, number* n1);
    static number* sub(number* n0, number* n1);
    static number* neg(number* n0);
    static number* mul(number* n0, number* n1);
    static number* div(number* n0, number* n1);
    static number* pow(number* n0, number* n1);
    static number* inv(number* n0);
private:
    static number* pow_double(number* base, number* exp);
    static number* int_pow_int(number* base, number* exp);
    number* int_pow_frac(number* base, number* exp);
    static number* frac_pow_int(number* base, number* exp);
    number* frac_pow_frac(number* base, number* exp);
};

class fraction:public number
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    fraction(int n,int d);
    // conversion
    double get_double_value();
    int get_int_value();
    long get_long_value();
    // equality
    bool same_class(atom* a);
    bool equals(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // member access
    int get_numerator();
    int get_denominator();
    // mathML
    std::string to_mathML();
private:
    long GCD(long a, long b);
    long m_num;
    long m_den;
};

class inumber:public number
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    inumber(int i);
    // conversion
    double get_double_value();
    int get_int_value();
    long get_long_value();
    // equality
    bool same_class(atom* a);
    bool equals(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
    int m_value;
};

class dnumber:public number
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    dnumber(double d);
    // conversion
    double get_double_value();
    int get_int_value();
    long get_long_value();
    // equality
    bool same_class(atom* a);
    bool equals(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
    double m_value;
};

#endif
