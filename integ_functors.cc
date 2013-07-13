#include "array_atom.h"
#include "context.h"
#include "integ_functors.h"
#include "number.h"
#include "prime.h"
#include "error.h"
#include <cmath>
#include <math.h>


/*
 * ROUND TO INTEGER
 */
atom* round_int::shallow_copy()
{
    return new round_int();
}
std::string round_int::get_keyword(int i)
{
    return "round";
}
bool round_int::same_class(atom* a)
{
    return dynamic_cast<round_int*>(a)!=0;
}
atom* round_int::evaluate(context* c)
{

    // evaluate arguments in numerical mode
    int prev_mode = c->get_mode();
    c->set_mode(context::NUMERICAL_MODE);
    atom* arg0 = get_arg(0)->evaluate(c);

    // return to previous mode
    c->set_mode(prev_mode);

    // exception (1)
    if(arg0==0)
    {
        delete arg0;
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        return arg0;
    }

    //
    if(number::is_number(arg0))
    {
        int i = static_cast<number*>(arg0)->get_int_value();
        delete arg0;
        return new inumber(i);
    }

    // do not allow round_int(round_int(x))
    if(same_class(arg0))
        return arg0;

    // default
    atom* retval = new round_int();
    retval->set_arg(0, arg0);
    return retval;
}

/*
 * ABSOLUTE VALUE
 */
atom* absolute_int::shallow_copy()
{
    return new absolute_int();
}
std::string absolute_int::get_keyword(int i)
{
    return "abs";
}
bool absolute_int::same_class(atom* a)
{
    return dynamic_cast<absolute_int*>(a)!=0;
}
atom* absolute_int::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);

    // exception (1)
    if(arg0==0)
    {
        delete arg0;
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        return arg0;
    }

    if(arg0->get_type()==atom::TYPE_INT_NUMBER)
    {
        int i = static_cast<number*>(arg0)->get_int_value();
        i = i>0?i:-i;
        delete arg0;
        return new inumber(i);
    }
    if(arg0->get_type()==atom::TYPE_DOUBLE_NUMBER)
    {
        double d = static_cast<number*>(arg0)->get_double_value();
        d = d>0?d:-d;
        delete arg0;
        return new dnumber(d);
    }
    if(arg0->get_type()==atom::TYPE_FRACTION)
    {
        fraction* frq = static_cast<fraction*>(arg0);
        int num = frq->get_numerator();
        int den = frq->get_denominator();
        num=num>0?num:-num;
        den=den>0?den:-den;
        number* retval = new fraction(num,den);
        delete frq;
        return retval;
    }

    // do not allow abs(abs(x))
    if(same_class(arg0))
        return arg0;

    // default
    atom* retval = new absolute_int();
    retval->set_arg(0, arg0);
    return retval;
}
std::string absolute_int::to_mathML()
{
    return "<apply><abs/>" + get_arg(0)->to_mathML() + "</apply>";
}

/*
 * PRIMALITY CHECK
 */
atom* is_prime::shallow_copy()
{
    return new is_prime();
}
std::string is_prime::get_keyword(int i)
{
    return "isPrime";
}
bool is_prime::same_class(atom* a)
{
    return dynamic_cast<is_prime*>(a)!=0;
}
atom* is_prime::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);

    // exception (1)
    if(arg0==0)
    {
        delete arg0;
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        return arg0;
    }

    if(number::is_number(arg0))
    {
        int i = static_cast<number*>(arg0)->get_int_value();
        delete arg0;
        return new inumber(prime::is_prime(i));
    }

    // default
    atom* retval = new is_prime();
    retval->set_arg(0,arg0);
    return retval;
}