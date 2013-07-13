#include "context.h"
#include "elemt_functors.h"
#include "number.h"
#include "power_functors.h"
#include "error.h"
#include <cmath>

/*
 * POWER
 */
atom* power::shallow_copy()
{
    return new power();
}
std::string power::get_keyword(int i)
{
    return "^";
}
int power::get_precedence()
{
    return 5;
}
bool power::is_left_associative()
{
    return false;
}
bool power::same_class(atom* a)
{
    return dynamic_cast<power*>(a)!=0;
}
atom* power::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        delete arg1;
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        delete arg1;
        return arg0;
    }
    if(error::is_error(arg1))
    {
        delete arg0;
        return arg1;
    }

    // x ^ 1
    if(number::is_number(arg0) && static_cast<number*>(arg0)->get_double_value()==1.0)
    {
        delete arg0;
        return arg1;
    }

    // x ^ 0
    if(number::is_number(arg0) && static_cast<number*>(arg0)->get_double_value()==0.0)
    {
        delete arg0;
        delete arg1;
        return new inumber(1);
    }

    // power ^ x
    if(same_class(arg1))
    {
        multiply* mul1 = new multiply();
        mul1->set_arg(0,arg1->get_arg(0)->deep_copy());
        mul1->set_arg(1,arg0);
        power* newpow = new power();
        newpow->set_arg(0,mul1);
        newpow->set_arg(1, arg1->get_arg(1)->deep_copy());
        delete arg1;
        atom* rv = newpow->evaluate(c);
        delete newpow;
        return rv;
    }

    // ¬<number> ^ . OR . ^ ¬<number>
    if(!number::is_number(arg0) || !number::is_number(arg1))
    {
        atom* v = new power();
        v->set_arg(0,arg0);
        v->set_arg(1,arg1);
        return v;
    }

    // numbers
    atom* retval;
    if(c->get_mode()==context::SYMBOLIC_MODE)
        retval = num_pow_symb(static_cast<number*>(arg1),static_cast<number*>(arg0));
    else
        retval = number::pow(static_cast<number*>(arg1),static_cast<number*>(arg0));
    delete arg0;
    delete arg1;
    return retval;
}
std::string power::to_mathML()
{
    if(get_arg(0)->get_type()==atom::TYPE_FRACTION)
    {
        fraction* f = static_cast<fraction*>(get_arg(0));
        if(f->get_numerator()==1)
            return "<apply><root/><degree>" + inumber(f->get_denominator()).to_mathML() + "</degree>" +  get_arg(1)->to_mathML() + "</apply>";
        else
            return "<apply><root/><apply><power/>" + get_arg(1)->to_mathML() + inumber(f->get_numerator()).to_mathML() + "</apply>"+inumber(f->get_denominator()).to_mathML()+"</apply>";
    }
    return "<apply><power/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() + "</apply>";
}
atom* power::pow_int_frac(number* base, number* exp)
{

    // handle negative exponents
    if(exp->get_double_value()<0)
    {
        number* newbase = new fraction(1,base->get_int_value());
        number* newexp = number::neg(exp);
        atom* retval = pow_frac_frac(newbase,newexp);
        delete newbase;
        delete newexp;
        return retval;
    }

    // fetch numerator and denominator
    int n = static_cast<fraction*>(exp)->get_numerator();
    int d = static_cast<fraction*>(exp)->get_denominator();

    int b = base->get_int_value();

    b = std::pow(b,n);

    // if power evaluates to exact int, return that int
    int estim = (int) std::pow(b,1.0/d);
    if(std::pow(estim,d)==b)
        return new inumber(estim);

    // find largest power of numerator that fits in base
    // keep track of factor
    // eg: 32^(1/2) => hipow = 16, facto = 4
    int hipow = 1;
    int facto = 1;
    for(int i=estim; i>=0; i--)
    {
        int p = std::pow(i,d);
        if(b % p == 0)
        {
            hipow = p;
            facto = i;
            break;
        }
    }

    // rewrite power
    // eg: 32^(1/2) => facto * ((32/hipow)^(1/2)) => 4*(2^(1/2))
    if(facto!=1)
    {

        b/=hipow;
        atom* retpow = new power();
        retpow->set_arg(0, new fraction(1,d));
        retpow->set_arg(1, new inumber(b));

        atom* retval = new multiply();
        retval->set_arg(0,retpow);
        retval->set_arg(1, new inumber(facto));
        return retval;
    }

    // default
    atom* retpow = new power();
    retpow->set_arg(0, new fraction(1,d));
    retpow->set_arg(1, new inumber(b));
    return retpow;
}
atom* power::pow_frac_frac(number* base, number* exp)
{

    number* n = new inumber(static_cast<fraction*>(base)->get_numerator());
    number* d = new inumber(static_cast<fraction*>(base)->get_denominator());

    atom* npow = pow_int_frac(n,exp);
    atom* dpow = pow_int_frac(d,exp);

    delete n;
    delete d;

    atom* retval = new divide();
    retval->set_arg(0, dpow);
    retval->set_arg(1, npow);
    return retval;
}
atom* power::num_pow_symb(number* base, number* exp)
{

    // int ^ fraction
    if(base->get_type()==atom::TYPE_INT_NUMBER && exp->get_type()==atom::TYPE_FRACTION)
        return pow_int_frac(base, exp);

    // fraction ^ fraction
    if(base->get_type()==atom::TYPE_FRACTION && exp->get_type()==atom::TYPE_FRACTION)
        return pow_frac_frac(base,exp);

    // default
    return number::pow(base,exp);
}

/*
 * .²
 */

atom* second_power::shallow_copy()
{
    return new second_power();
}
std::string second_power::get_keyword(int i)
{
    return "²";
}
int second_power::get_precedence()
{
    return 5;
}
bool second_power::is_left_associative()
{
    return false;
}
bool second_power::same_class(atom* a)
{
    return dynamic_cast<second_power*>(a)!=0;
}
atom* second_power::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);

    // exception (1)
    if(arg0==0)
    {
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        return arg0;
    }

    // default
    power* p = new power();
    p->set_arg(0,new inumber(2));
    p->set_arg(1,arg0);
    atom* retval = p->evaluate(c);
    delete p;
    return retval;
}
std::string second_power::to_mathML()
{
    return "<apply><power/>" + get_arg(0)->to_mathML() + "<cn>2</cn></apply>";
}

/*
 * .³
 */

atom* third_power::shallow_copy()
{
    return new third_power();
}
std::string third_power::get_keyword(int i)
{
    return "³";
}
int third_power::get_precedence()
{
    return 5;
}
bool third_power::is_left_associative()
{
    return false;
}
bool third_power::same_class(atom* a)
{
    return dynamic_cast<third_power*>(a)!=0;
}
atom* third_power::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);

    // exception (1)
    if(arg0==0)
    {
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        return arg0;
    }

    // default
    power* p = new power();
    p->set_arg(0,new inumber(3));
    p->set_arg(1,arg0);
    atom* retval = p->evaluate(c);
    delete p;
    return retval;
}
std::string third_power::to_mathML()
{
    return "<apply><power/>" + get_arg(0)->to_mathML() + "<cn>3</cn></apply>";
}

/*
 * LOGARITHM
 */
atom* logarithm::shallow_copy()
{
    return new logarithm();
}
std::string logarithm::get_keyword(int i)
{
    return "log";
}
int logarithm::get_precedence()
{
    return 5;
}
bool logarithm::is_left_associative()
{
    return false;
}
bool logarithm::same_class(atom* a)
{
    return dynamic_cast<logarithm*>(a)!=0;
}
atom* logarithm::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);

    // exception (1)
    if(arg0==0)
    {
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        return arg0;
    }

    // numbers
    if(number::is_number(arg0))
    {
        atom* retval = new dnumber(std::log(static_cast<number*>(arg0)->get_double_value()));
        delete arg0;
        return retval;
    }

    // default
    atom* retval = new logarithm();
    retval->set_arg(0,arg0);
    return retval;
}
std::string logarithm::to_mathML()
{
    return "<apply><ln/>" + get_arg(0)->to_mathML() + "</apply>";
}