#include "context.h"
#include "gonio_functors.h"
#include "gonio_rounder.h"
#include "number.h"
#include "error.h"
#include "elemt_functors.h"
#include <math.h>

/**
 * PI CONSTANT
 **/
atom* pi_constant::shallow_copy()
{
    return new pi_constant();
}
bool pi_constant::same_class(atom* a)
{
    return dynamic_cast<pi_constant*>(a)!=0;
}
std::string pi_constant::get_keyword(int i)
{
    return "Pi";
}
atom* pi_constant::evaluate(context* c)
{
    if(c->get_mode()==context::SYMBOLIC_MODE)
        return new pi_constant();
    else
        return new dnumber(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196);
}
std::string pi_constant::to_mathML()
{
    return "<pi/>";
}

/*
 * TO RADIANS
 */
atom* to_rad::shallow_copy()
{
    return new to_rad();
}
std::string to_rad::get_keyword(int i)
{
    return "toRad";
}
bool to_rad::same_class(atom* a)
{
    return dynamic_cast<to_rad*>(a)!=0;
}
atom* to_rad::evaluate(context* c)
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

    // numeric mode
    if(c->get_mode()==context::NUMERICAL_MODE)
    {
        if(!number::is_number(arg0))
            return 0;
        double f = static_cast<number*>(arg0)->get_double_value() / (float) 180.0;
        double pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196;
        delete arg0;
        return new dnumber(f*pi);
    }

    // symbolic mode
    atom* part01 = new divide();
    part01->set_arg(0, new inumber(180));
    part01->set_arg(1, arg0);

    number* part01_ev = static_cast<number*>(part01->evaluate(c));
    delete part01;

    // exception for 1 and 0
    if(part01_ev->get_type()==atom::TYPE_INT_NUMBER)
    {
        int v = part01_ev->get_int_value();
        if(v==0)
        {
            delete part01_ev;
            return new inumber(0);
        }
        else if(v==1)
        {
            delete part01_ev;
            return new pi_constant();
        }
    }

    atom* prod = new multiply();
    prod->set_arg(0, part01_ev);
    prod->set_arg(1, new pi_constant());

    atom* retval = prod->evaluate(c);
    delete prod;

    return retval;
}

/*
 * TO DEGREES
 */
atom* to_degrees::shallow_copy()
{
    return new to_degrees();
}
std::string to_degrees::get_keyword(int i)
{
    return "toDeg";
}
bool to_degrees::same_class(atom* a)
{
    return dynamic_cast<to_degrees*>(a)!=0;
}
atom* to_degrees::evaluate(context* c)
{
    double pi = 3.14159265358979323846264338327950288419716939937510;
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

    atom* retval = 0;
    if(number::is_number(arg0))
    {
        double d = static_cast<number*>(arg0)->get_double_value();
        return new dnumber(d*(180.0/pi));
    }

    // default
    if(retval==0)
    {
        retval = new to_degrees();
        retval->set_arg(0,arg0);
    }
    else
    {
        delete arg0;
    }
    return retval;
}

/*
 * SINE
 */
atom* sine::shallow_copy()
{
    return new sine();
}
std::string sine::get_keyword(int i)
{
    return "sin";
}
bool sine::same_class(atom* a)
{
    return dynamic_cast<sine*>(a)!=0;
}
atom* sine::evaluate(context* c)
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
        double d = static_cast<number*>(arg0)->get_double_value();
        delete arg0;
        if(c->get_mode()==context::SYMBOLIC_MODE)
            return gonio_rounder::round((double)sin(d));
        else
            return new dnumber(sin(d));
    }

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        return new invalid_argument();
    }

    // default
    atom* retval = new sine();
    retval->set_arg(0, arg0);
    return retval;
}

/*
 * COSINE
 */
atom* cosine::shallow_copy()
{
    return new cosine();
}
std::string cosine::get_keyword(int i)
{
    return "cos";
}
bool cosine::same_class(atom* a)
{
    return dynamic_cast<cosine*>(a)!=0;
}
atom* cosine::evaluate(context* c)
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
        double d = static_cast<number*>(arg0)->get_double_value();
        delete arg0;
        if(c->get_mode()==context::SYMBOLIC_MODE)
            return gonio_rounder::round((double)cos(d));
        else
            return new dnumber(cos(d));
    }

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        return new invalid_argument();
    }

    // default
    atom* retval = new cosine();
    retval->set_arg(0, arg0);
    return retval;
}

/*
 * TANGENT
 */
atom* tangent::shallow_copy()
{
    return new tangent();
}
std::string tangent::get_keyword(int i)
{
    return "tan";
}
bool tangent::same_class(atom* a)
{
    return dynamic_cast<tangent*>(a)!=0;
}
atom* tangent::evaluate(context* c)
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
        double d = static_cast<number*>(arg0)->get_double_value();
        delete arg0;
        if(c->get_mode()==context::SYMBOLIC_MODE)
            return gonio_rounder::round((double)tan(d));
        else
            return new dnumber(tan(d));
    }

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        return new invalid_argument();
    }

    // default
    atom* retval = new tangent();
    retval->set_arg(0, arg0);
    return retval;
}

/*
 * COTANGENT
 */
atom* cotangent::shallow_copy()
{
    return new cotangent();
}
std::string cotangent::get_keyword(int i)
{
    return "cot";
}
bool cotangent::same_class(atom* a)
{
    return dynamic_cast<cotangent*>(a)!=0;
}
atom* cotangent::evaluate(context* c)
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
        double d = static_cast<number*>(arg0)->get_double_value();
        delete arg0;
        if(c->get_mode()==context::SYMBOLIC_MODE)
            return gonio_rounder::round((double)(1.0/tan(d)));
        else
            return new dnumber(1.0/tan(d));
    }

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        return new invalid_argument();
    }
    // default
    atom* retval = new cotangent();
    retval->set_arg(0, arg0);
    return retval;
}

/*
 * COSECANT
 */
atom* cosecant::shallow_copy()
{
    return new cosecant();
}
std::string cosecant::get_keyword(int i)
{
    return "csc";
}
bool cosecant::same_class(atom* a)
{
    return dynamic_cast<cosecant*>(a)!=0;
}
atom* cosecant::evaluate(context* c)
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
        double d = static_cast<number*>(arg0)->get_double_value();
        delete arg0;
        if(c->get_mode()==context::SYMBOLIC_MODE)
            return gonio_rounder::round((double)(1.0/sin(d)));
        else
            return new dnumber(1.0/sin(d));
    }

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        return new invalid_argument();
    }

    // default
    atom* retval = new cosecant();
    retval->set_arg(0, arg0);
    return retval;
}

/*
 * SECANT
 */
atom* secant::shallow_copy()
{
    return new secant();
}
std::string secant::get_keyword(int i)
{
    return "sec";
}
bool secant::same_class(atom* a)
{
    return dynamic_cast<secant*>(a)!=0;
}
atom* secant::evaluate(context* c)
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
        double d = static_cast<number*>(arg0)->get_double_value();
        delete arg0;
        if(c->get_mode()==context::SYMBOLIC_MODE)
            return gonio_rounder::round((double)(1.0/cos(d)));
        else
            return new dnumber(1.0/cos(d));
    }

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        return new invalid_argument();
    }

    // default
    atom* retval = new secant();
    retval->set_arg(0, arg0);
    return retval;
}