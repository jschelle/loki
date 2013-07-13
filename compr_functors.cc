/*
 * File:   compr_functors.cc
 * Author: joris
 *
 */

#include "compr_functors.h"
#include "number.h"
#include "text_atom.h"
#include "error.h"
#include "context.h"

/**
 * EQUIVALENCE
 **/

atom* equivalence::shallow_copy()
{
    return new equivalence();
}
std::string equivalence::get_keyword(int i)
{
    return "==";
}
int equivalence::get_precedence()
{
    return 5;
}
bool equivalence::same_class(atom* a)
{
    return dynamic_cast<equivalence*>(a)!=0;
}
atom* equivalence::evaluate(context* c)
{
    int prev_mode = c->get_mode();
    c->set_mode(context::NUMERICAL_MODE);
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);
    c->set_mode(prev_mode);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        delete arg1;
        return 0;
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

    // numbers
    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        double i0 = static_cast<number*>(arg0)->get_double_value();
        double i1 = static_cast<number*>(arg1)->get_double_value();
        int rv = i1==i0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // text
    if(arg0->get_type()==atom::TYPE_TEXT && arg1->get_type()==atom::TYPE_TEXT)
    {
        std::string s0 = static_cast<text_atom*>(arg0)->get_text();
        std::string s1 = static_cast<text_atom*>(arg1)->get_text();
        int rv = s0.compare(s1)==0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // default
    if(retval==0)
    {
        retval = new equivalence();
        retval->set_arg(0, arg0);
        retval->set_arg(1, arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }

    return retval;
}
std::string equivalence::to_mathML()
{
    return "<apply><eq/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * INEQUIVALENCE
 **/

atom* inequivalence::shallow_copy()
{
    return new inequivalence();
}
std::string inequivalence::get_keyword(int i)
{
    return "!=";
}
int inequivalence::get_precedence()
{
    return 5;
}
bool inequivalence::same_class(atom* a)
{
    return dynamic_cast<inequivalence*>(a)!=0;
}
atom* inequivalence::evaluate(context* c)
{
    int prev_mode = c->get_mode();
    c->set_mode(context::NUMERICAL_MODE);
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);
    c->set_mode(prev_mode);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        delete arg1;
        return 0;
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

    // numbers
    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        double i0 = static_cast<number*>(arg0)->get_double_value();
        double i1 = static_cast<number*>(arg1)->get_double_value();
        int rv = i1!=i0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // text
    if(arg0->get_type()==atom::TYPE_TEXT && arg1->get_type()==atom::TYPE_TEXT)
    {
        std::string s0 = static_cast<text_atom*>(arg0)->get_text();
        std::string s1 = static_cast<text_atom*>(arg1)->get_text();
        int rv = s0.compare(s1)!=0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // default
    if(retval==0)
    {
        retval = new inequivalence();
        retval->set_arg(0, arg0);
        retval->set_arg(1, arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }

    return retval;
}
std::string inequivalence::to_mathML()
{
    return "<apply><neq/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * LESS THAN
 **/

atom* less_than::shallow_copy()
{
    return new less_than();
}
std::string less_than::get_keyword(int i)
{
    return "<";
}
int less_than::get_precedence()
{
    return 5;
}
bool less_than::same_class(atom* a)
{
    return dynamic_cast<less_than*>(a)!=0;
}
atom* less_than::evaluate(context* c)
{
    int prev_mode = c->get_mode();
    c->set_mode(context::NUMERICAL_MODE);
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);
    c->set_mode(prev_mode);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        delete arg1;
        return 0;
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

    // numbers
    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        double i0 = static_cast<number*>(arg0)->get_double_value();
        double i1 = static_cast<number*>(arg1)->get_double_value();
        int rv = i1<i0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // default
    if(retval==0)
    {
        retval = new less_than();
        retval->set_arg(0, arg0);
        retval->set_arg(1, arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }
    return retval;
}
std::string less_than::to_mathML()
{
    return "<apply><lt/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * LESS THAN OR EQUAL
 **/

atom* less_than_or_equal::shallow_copy()
{
    return new less_than_or_equal();
}
std::string less_than_or_equal::get_keyword(int i)
{
    return "<=";
}
int less_than_or_equal::get_precedence()
{
    return 5;
}
bool less_than_or_equal::same_class(atom* a)
{
    return dynamic_cast<less_than_or_equal*>(a)!=0;
}
atom* less_than_or_equal::evaluate(context* c)
{
    int prev_mode = c->get_mode();
    c->set_mode(context::NUMERICAL_MODE);
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);
    c->set_mode(prev_mode);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        delete arg1;
        return 0;
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

    // numbers
    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        double i0 = static_cast<number*>(arg0)->get_double_value();
        double i1 = static_cast<number*>(arg1)->get_double_value();
        int rv = i1<=i0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // default
    if(retval==0)
    {
        retval = new less_than_or_equal();
        retval->set_arg(0, arg0);
        retval->set_arg(1, arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }
    return retval;
}
std::string less_than_or_equal::to_mathML()
{
    return "<apply><leq/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * GREATER THAN
 **/

atom* greater_than::shallow_copy()
{
    return new greater_than();
}
std::string greater_than::get_keyword(int i)
{
    return ">";
}
int greater_than::get_precedence()
{
    return 5;
}
bool greater_than::same_class(atom* a)
{
    return dynamic_cast<greater_than*>(a)!=0;
}
atom* greater_than::evaluate(context* c)
{
    int prev_mode = c->get_mode();
    c->set_mode(context::NUMERICAL_MODE);
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);
    c->set_mode(prev_mode);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        delete arg1;
        return 0;
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

    // numbers
    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        double i0 = static_cast<number*>(arg0)->get_double_value();
        double i1 = static_cast<number*>(arg1)->get_double_value();
        int rv = i1>i0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // default
    if(retval==0)
    {
        retval = new greater_than();
        retval->set_arg(0, arg0);
        retval->set_arg(1, arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }
    return retval;
}
std::string greater_than::to_mathML()
{
    return "<apply><gt/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * GREATER THAN OR EQUAL
 **/

atom* greater_than_or_equal::shallow_copy()
{
    return new greater_than_or_equal();
}
std::string greater_than_or_equal::get_keyword(int i)
{
    return ">=";
}
int greater_than_or_equal::get_precedence()
{
    return 5;
}
bool greater_than_or_equal::same_class(atom* a)
{
    return dynamic_cast<greater_than_or_equal*>(a)!=0;
}
atom* greater_than_or_equal::evaluate(context* c)
{
    int prev_mode = c->get_mode();
    c->set_mode(context::NUMERICAL_MODE);
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);
    c->set_mode(prev_mode);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        delete arg1;
        return 0;
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

    // numbers
    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        double i0 = static_cast<number*>(arg0)->get_double_value();
        double i1 = static_cast<number*>(arg1)->get_double_value();
        int rv = i1>=i0 ? 1 : 0;
        retval = new inumber(rv);
    }
    // default
    if(retval==0)
    {
        retval = new greater_than_or_equal();
        retval->set_arg(0, arg0);
        retval->set_arg(1, arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }
    return retval;
}
std::string greater_than_or_equal::to_mathML()
{
    return "<apply><geq/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}