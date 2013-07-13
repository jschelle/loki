#include "context.h"
#include "number.h"
#include <cmath>
#include <sstream>
#include <iostream>

bool number::is_number(atom* a)
{
    if(a==0)
        return false;
    int t = a->get_type();
    return t==atom::TYPE_DOUBLE_NUMBER || t==atom::TYPE_INT_NUMBER || t==atom::TYPE_FRACTION;
}
number* number::add(number* c0, number* c1)
{

    // double + . OR . + double
    if(c0->get_type()==atom::TYPE_DOUBLE_NUMBER || c1->get_type()==atom::TYPE_DOUBLE_NUMBER)
        return new dnumber(c0->get_double_value()+c1->get_double_value());

    // int + int
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        return new inumber(c0->get_int_value()+c1->get_int_value());
    }

    // fraction + int
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        fraction* frq = static_cast<fraction*>(c0);
        int num = frq->get_numerator() + c1->get_int_value()*frq->get_denominator();
        int den = frq->get_denominator();
        fraction* ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }
    // int + fraction
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* frq = static_cast<fraction*>(c1);
        int num = frq->get_numerator() + c0->get_int_value()*frq->get_denominator();
        int den = frq->get_denominator();
        fraction* ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }
    // fraction + fraction
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* f0 = static_cast<fraction*>(c0);
        fraction* f1 = static_cast<fraction*>(c1);
        int num = f0->get_numerator()*f1->get_denominator() + f1->get_numerator()*f0->get_denominator();
        int den = f0->get_denominator()*f1->get_denominator();
        fraction* ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }

}
number* number::sub(number* c0, number* c1)
{

    // double - . OR . - double
    if(c0->get_type()==atom::TYPE_DOUBLE_NUMBER || c1->get_type()==atom::TYPE_DOUBLE_NUMBER)
        return new dnumber(c0->get_double_value()-c1->get_double_value());

    // int - int
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        return new inumber(c0->get_int_value()-c1->get_int_value());
    }

    // fraction - int
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        fraction* frq = static_cast<fraction*>(c0);
        int num = frq->get_numerator() - c1->get_int_value()*frq->get_denominator();
        int den = frq->get_denominator();
        fraction* ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }
    // int - fraction
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* frq = static_cast<fraction*>(c1);
        int num = c0->get_int_value()*frq->get_denominator() - frq->get_numerator();
        int den = frq->get_denominator();
        fraction* ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }

    // fraction - fraction
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* f0 = static_cast<fraction*>(c0);
        fraction* f1 = static_cast<fraction*>(c1);
        int num = f0->get_numerator()*f1->get_denominator() - f1->get_numerator()*f0->get_denominator();
        int den = f0->get_denominator()*f1->get_denominator();
        fraction* ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }

}
number* number::neg(number* c0)
{
    number* zero = new inumber(0);
    number* retval = sub(zero,c0);
    delete zero;
    return retval;
}
number* number::mul(number* c0, number* c1)
{

    // double * . OR . * double
    if(c0->get_type()==atom::TYPE_DOUBLE_NUMBER || c1->get_type()==atom::TYPE_DOUBLE_NUMBER)
        return new dnumber(c0->get_double_value()*c1->get_double_value());

    // int * int
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        return new inumber(c0->get_int_value()*c1->get_int_value());
    }

    // fraction * int
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        fraction* frq = static_cast<fraction*>(c0);
        int num = frq->get_numerator()*c1->get_int_value();
        int den = frq->get_denominator();
        fraction * ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }
    // int * fraction
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* frq = static_cast<fraction*>(c1);
        int num = c0->get_int_value()*frq->get_numerator();
        int den = frq->get_denominator();
        fraction * ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }

    // fraction * fraction
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* f0 = static_cast<fraction*>(c0);
        fraction* f1 = static_cast<fraction*>(c1);
        int num = f0->get_numerator()*f1->get_numerator();
        int den = f0->get_denominator()*f1->get_denominator();
        fraction * ret = new fraction(num,den);
        if(ret->get_denominator()==1)
        {
            int i = ret->get_numerator();
            delete ret;
            return new inumber(i);
        }
        return ret;
    }

}
number* number::pow(number* n0, number* n1)
{

    // pow with at least one double
    if(n0->get_type()==atom::TYPE_DOUBLE_NUMBER || n1->get_type()==atom::TYPE_DOUBLE_NUMBER)
        return pow_double(n0,n1);

    // int ^ int
    if(n0->get_type()==atom::TYPE_INT_NUMBER && n1->get_type()==atom::TYPE_INT_NUMBER)
    {
        return int_pow_int(n0,n1);
    }
    // frac ^ int
    if(n0->get_type()==atom::TYPE_FRACTION && n1->get_type()==atom::TYPE_INT_NUMBER)
        return frac_pow_int(n0,n1);

    // default
    return new dnumber(std::pow(n0->get_double_value(),n1->get_double_value()));

}
number* number::pow_double(number* base, number* exp)
{
    return new dnumber(std::pow(base->get_double_value(),exp->get_double_value()));
}
number* number::int_pow_int(number* base, number* exp)
{
    int e = exp->get_int_value();
    int b = base->get_int_value();
    if(e<0)
        return new fraction(1,std::pow(b,-e));
    return new inumber(std::pow(b,e));
}
number* number::frac_pow_int(number* base, number* exp)
{
    int e = exp->get_int_value();
    if(e==0)
        return new inumber(1);
    if(e==1)
        return static_cast<number*>(base->deep_copy());

    fraction* f = static_cast<fraction*>(base);
    if(e<0)
    {
        return new fraction(std::pow(f->get_denominator(),e),std::pow(f->get_numerator(),e));
    }
    else
    {
        return new fraction(std::pow(f->get_numerator(),e),std::pow(f->get_denominator(),e));
    }
}

number* number::div(number* c0, number* c1)
{

    // double : . OR . : double
    if(c0->get_type()==atom::TYPE_DOUBLE_NUMBER || c1->get_type()==atom::TYPE_DOUBLE_NUMBER)
        return new dnumber(c0->get_double_value()/c1->get_double_value());

    // int : int
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        fraction* f  = new fraction(c0->get_int_value(),c1->get_int_value());
        if(f->get_denominator()==1)
        {
            inumber* retval = new inumber(f->get_numerator());
            delete f;
            return retval;
        }
        return f;
    }

    // fraction : int
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_INT_NUMBER)
    {
        fraction* frq = static_cast<fraction*>(c0);
        int num = frq->get_numerator();
        int den = frq->get_denominator()*c1->get_int_value();
        return new fraction(num,den);
    }
    // int : fraction
    if(c0->get_type()==atom::TYPE_INT_NUMBER && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* frq = static_cast<fraction*>(c1);
        int num = c0->get_int_value()*frq->get_denominator();
        int den = frq->get_numerator();
        return new fraction(num,den);
    }

    // fraction : fraction
    if(c0->get_type()==atom::TYPE_FRACTION && c1->get_type()==atom::TYPE_FRACTION)
    {
        fraction* f0 = static_cast<fraction*>(c0);
        fraction* f1 = static_cast<fraction*>(c1);
        int num = f0->get_numerator()*f1->get_denominator();
        int den = f0->get_denominator()*f1->get_numerator();
        return new fraction(num,den);
    }

}
number* number::inv(number* n0)
{

    // double
    if(n0->get_type()==number::TYPE_DOUBLE_NUMBER)
        return new dnumber(1.0 / n0->get_double_value());

    // integer
    if(n0->get_type()==number::TYPE_INT_NUMBER)
    {
        int i = n0->get_int_value();
        if(i==1)
            return new inumber(1);
        return new fraction(1,i);
    }
    // fraction
    if(n0->get_type()==number::TYPE_FRACTION)
    {
        fraction* f = static_cast<fraction*>(n0);
        return new fraction(f->get_denominator(),f->get_numerator());
    }
}
// fraction
fraction::fraction(int n, int d)
{
    long gcd = GCD(n,d);
    this->m_num = n/gcd;
    this->m_den = d/gcd;
    // -5:-7 => 5/7
    if(m_num<0 && m_den<0)
    {
        m_num=-m_num;
        m_den=-m_den;
    }
    // 5:-7 => -5:7
    if(m_den<0)
    {
        m_num=-m_num;
        m_den=-m_den;
    }
}
int fraction::get_type()
{
    return atom::TYPE_FRACTION;
}
atom* fraction::shallow_copy()
{
    return new fraction(m_num,m_den);
}
double fraction::get_double_value()
{
    return ((double)m_num)/((double)m_den);
}
int fraction::get_int_value()
{
    return m_num/m_den;
}
long fraction::get_long_value()
{
    return (long) m_num/m_den;
}
bool fraction::same_class(atom* a)
{
    return dynamic_cast<fraction*>(a)!=0;
}
bool fraction::equals(atom* a)
{
    if(!same_class(a))
        return false;
    fraction* other = static_cast<fraction*>(a);
    return other->m_num==m_num && other->m_den==m_den;
}
atom* fraction::evaluate(context* c)
{
    // numeric
    if(c->get_mode()==context::NUMERICAL_MODE)
        return new dnumber(get_double_value());
    // symbolic
    double v = ((double)m_num)/((double)m_den);
    bool is_int = v==(int)v;
    if(is_int)
        return new inumber((int)v);
    return deep_copy();
}
int fraction::get_numerator()
{
    return m_num;
}
int fraction::get_denominator()
{
    return m_den;
}
std::string fraction::to_mathML()
{
    std::ostringstream ns;
    ns << m_num;
    std::ostringstream ds;
    ds << m_den;
    return "<apply><divide/><cn>" + ns.str() + "</cn><cn>" + ds.str() + "</cn></apply>";
}
long fraction::GCD(long a, long b)
{
    while(1)
    {
        a = a % b;
        if(a==0)
            return b;
        b=b%a;
        if(b==0)
            return a;
    }
    return 1;
}

// inumber
int inumber::get_type()
{
    return atom::TYPE_INT_NUMBER;
}
atom* inumber::shallow_copy()
{
    return new inumber(m_value);
}
inumber::inumber(int i)
{
    this->m_value = i;
}
double inumber::get_double_value()
{
    return (double) m_value;
}
int inumber::get_int_value()
{
    return m_value;
}
long inumber::get_long_value()
{
    return (long) m_value;
}
bool inumber::same_class(atom* a)
{
    return dynamic_cast<inumber*>(a)!=0;
}
bool inumber::equals(atom* a)
{
    if(!same_class(a))
        return false;
    inumber* other = static_cast<inumber*>(a);
    return other->m_value==m_value;
}
atom* inumber::evaluate(context* c)
{
    return deep_copy();
}
std::string inumber::to_mathML()
{
    std::ostringstream ns;
    ns << m_value;
    return "<cn>" + ns.str() + "</cn>";
}

// dnumber
int dnumber::get_type()
{
    return atom::TYPE_DOUBLE_NUMBER;
}
atom* dnumber::shallow_copy()
{
    return new dnumber(m_value);
}
dnumber::dnumber(double d)
{
    this->m_value = d;
}
double dnumber::get_double_value()
{
    return m_value;
}
int dnumber::get_int_value()
{
    return (int) m_value;
}
long dnumber::get_long_value()
{
    return (long) m_value;
}
bool dnumber::same_class(atom* a)
{
    return dynamic_cast<dnumber*>(a)!=0;
}
bool dnumber::equals(atom* a)
{
    if(!same_class(a))
        return false;
    dnumber* other = static_cast<dnumber*>(a);
    return other->m_value==m_value;
}
atom* dnumber::evaluate(context* c)
{
    return deep_copy();
}
std::string dnumber::to_mathML()
{
    std::ostringstream ns;
    ns << m_value;
    return "<cn>" + ns.str() + "</cn>";
}