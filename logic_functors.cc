/*
 * File:   logic_functors.cpp
 * Author: joris
 *
 * Created on March 16, 2012, 10:49 PM
 */

#include "logic_functors.h"
#include "number.h"
#include "error.h"

/*
 * LOGICAL AND
 */
atom* logical_and::shallow_copy()
{
    return new logical_and();
}
std::string logical_and::get_keyword(int i)
{
    return "&&";
}
int logical_and::get_precedence()
{
    return 2;
}
bool logical_and::same_class(atom* a)
{
    return dynamic_cast<logical_and*>(a)!=0;
}
atom* logical_and::evaluate(context* c)
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

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT || arg1->get_type()==atom::TYPE_ARRAY || arg1->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        delete arg1;
        return new invalid_argument();
    }

    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        int i0 = static_cast<number*>(arg0)->get_int_value();
        int i1 = static_cast<number*>(arg1)->get_int_value();
        int rv = (i0==1 && i1==1) ? 1 : 0;
        retval = new inumber(rv);
    }

    // default
    if(retval==0)
    {
        retval = new logical_and();
        retval->set_arg(0,arg0);
        retval->set_arg(1,arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }

    return retval;
}
std::string logical_and::to_mathML()
{
    return "<apply><and/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/*
 * LOGICAL OR
 */
atom* logical_or::shallow_copy()
{
    return new logical_or();
}
std::string logical_or::get_keyword(int i)
{
    return "||";
}
int logical_or::get_precedence()
{
    return 3;
}
bool logical_or::same_class(atom* a)
{
    return dynamic_cast<logical_or*>(a)!=0;
}
atom* logical_or::evaluate(context* c)
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

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT || arg1->get_type()==atom::TYPE_ARRAY || arg1->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        delete arg1;
        return new invalid_argument();
    }

    atom* retval = 0;
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        int i0 = static_cast<number*>(arg0)->get_int_value();
        int i1 = static_cast<number*>(arg1)->get_int_value();
        int rv = (i0==1 || i1==1) ? 1 : 0;
        retval = new inumber(rv);
    }

    if(retval==0 && number::is_number(arg0))
    {
        int i0 = static_cast<number*>(arg0)->get_int_value();
        if(i0!=0)
            retval = new inumber(1);
    }
    if(retval==0 && number::is_number(arg1))
    {
        int i1 = static_cast<number*>(arg1)->get_int_value();
        if(i1!=0)
            retval = new inumber(1);
    }

    // default
    if(retval==0)
    {
        retval = new logical_or();
        retval->set_arg(0,arg0);
        retval->set_arg(1,arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }

    return retval;
}
std::string logical_or::to_mathML()
{
    return "<apply><or/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/*
 * LOGICAL NOT
 */
atom* logical_not::shallow_copy()
{
    return new logical_not();
}
std::string logical_not::get_keyword(int i)
{
    return "¬";
}
int logical_not::get_precedence()
{
    return 3;
}
bool logical_not::is_pre_operand()
{
    return true;
}
bool logical_not::same_class(atom* a)
{
    return dynamic_cast<logical_not*>(a)!=0;
}
atom* logical_not::evaluate(context* c)
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

    // numbers
    if(number::is_number(arg0))
    {
        int i0 = static_cast<number*>(arg0)->get_int_value();
        int rv = (i0==1) ? 0 : 1;
        delete arg0;
        return new inumber(rv);
    }

    // don't evalulate [] or ''
    if(arg0->get_type()==atom::TYPE_ARRAY || arg0->get_type()==atom::TYPE_TEXT)
    {
        delete arg0;
        return new invalid_argument();
    }

    // default
    atom* retval = new logical_not();
    retval->set_arg(0,arg0);
    return retval;

}
std::string logical_not::to_mathML()
{
    return "<apply><not/>" + get_arg(0)->to_mathML() +"</apply>";
}