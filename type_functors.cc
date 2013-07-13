/*
 * File:   type_functors.cpp
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#include "type_functors.h"
#include "context.h"
#include "number.h"
#include "error.h"

/*
 * EVALUATE TO FLOATING POINT
 */
atom* evalf::shallow_copy()
{
    return new evalf();
}
std::string evalf::get_keyword(int i)
{
    return "evalf";
}
int evalf::get_precedence()
{
    return 6;
}
bool evalf::same_class(atom* a)
{
    return dynamic_cast<evalf*>(a)!=0;
}
atom* evalf::evaluate(context* c)
{
    c->set_mode(context::NUMERICAL_MODE);
    atom* retval = get_arg(0)->evaluate(c);
    if(number::is_number(retval))
    {
        atom* temp = new dnumber(static_cast<number*>(retval)->get_double_value());
        delete retval;
        retval = temp;
    }
    c->set_mode(context::SYMBOLIC_MODE);
    return retval;
}
std::string evalf::to_mathML()
{
    return "<apply><csymbol>evalf</csymbol>" + get_arg(0)->to_mathML() +"</apply>";
}

/*
 * CHECK FOR NUMBER
 */
atom* is_number::shallow_copy()
{
    return new is_number();
}
std::string is_number::get_keyword(int i)
{
    return "isNumber";
}
int is_number::get_precedence()
{
    return 6;
}
bool is_number::same_class(atom* a)
{
    return dynamic_cast<is_number*>(a)!=0;
}
atom* is_number::evaluate(context* c)
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
    int rv = number::is_number(arg0);
    delete arg0;
    return new inumber(rv);
}
std::string is_number::to_mathML()
{
    return "<apply><csymbol>isNumber</csymbol>" + get_arg(0)->to_mathML() +"</apply>";
}

/*
 * CHECK FOR  DOUBLE
 */
atom* is_double::shallow_copy()
{
    return new is_double();
}
std::string is_double::get_keyword(int i)
{
    return "isDouble";
}
int is_double::get_precedence()
{
    return 6;
}
bool is_double::same_class(atom* a)
{
    return dynamic_cast<is_double*>(a)!=0;
}
atom* is_double::evaluate(context* c)
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
    int rv = arg0->get_type()==atom::TYPE_DOUBLE_NUMBER;
    delete arg0;
    return new inumber(rv);
}
std::string is_double::to_mathML()
{
    return "<apply><csymbol>isDouble</csymbol>" + get_arg(0)->to_mathML() +"</apply>";
}

/*
 * CHECK FOR INTEGER
 */
atom* is_integer::shallow_copy()
{
    return new is_integer();
}
std::string is_integer::get_keyword(int i)
{
    return "isInteger";
}
int is_integer::get_precedence()
{
    return 6;
}
bool is_integer::same_class(atom* a)
{
    return dynamic_cast<is_integer*>(a)!=0;
}
atom* is_integer::evaluate(context* c)
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
    int rv = arg0->get_type()==atom::TYPE_INT_NUMBER;
    delete arg0;
    return new inumber(rv);
}
std::string is_integer::to_mathML()
{
    return "<apply><csymbol>isInteger</csymbol>" + get_arg(0)->to_mathML() +"</apply>";
}

/*
 * CHECK FOR TEXT
 */
atom* is_text::shallow_copy()
{
    return new is_text();
}
std::string is_text::get_keyword(int i)
{
    return "isText";
}
int is_text::get_precedence()
{
    return 6;
}
bool is_text::same_class(atom* a)
{
    return dynamic_cast<is_text*>(a)!=0;
}
atom* is_text::evaluate(context* c)
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
    int rv = arg0->get_type()==atom::TYPE_TEXT;
    delete arg0;
    return new inumber(rv);
}
std::string is_text::to_mathML()
{
    return "<apply><csymbol>isText</csymbol>" + get_arg(0)->to_mathML() +"</apply>";
}

/*
 * CHECK FOR ARRAY
 */
atom* is_array::shallow_copy()
{
    return new is_array();
}
std::string is_array::get_keyword(int i)
{
    return "isArray";
}
int is_array::get_precedence()
{
    return 6;
}
bool is_array::same_class(atom* a)
{
    return dynamic_cast<is_array*>(a)!=0;
}
atom* is_array::evaluate(context* c)
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
    int rv = arg0->get_type()==atom::TYPE_ARRAY;
    delete arg0;
    return new inumber(rv);
}
std::string is_array::to_mathML()
{
    return "<apply><csymbol>isArray</csymbol>" + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * CHECK FOR OPERATOR
 */
atom* is_operator::shallow_copy()
{
    return new is_operator();
}
std::string is_operator::get_keyword(int i)
{
    return "isOperator";
}
int is_operator::get_precedence()
{
    return 6;
}
bool is_operator::same_class(atom* a)
{
    return dynamic_cast<is_operator*>(a)!=0;
}
atom* is_operator::evaluate(context* c)
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
    int rv = arg0->get_type()==atom::TYPE_OPERATOR;
    delete arg0;
    return new inumber(rv);
}
std::string is_operator::to_mathML()
{
    return "<apply><csymbol>isOperator</csymbol>" + get_arg(0)->to_mathML() +"</apply>";
}
