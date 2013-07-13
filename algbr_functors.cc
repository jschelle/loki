/*
 * File:   algebr_functors.cc
 * Author: joris
 *
 */

#include "algbr_functors.h"
#include "elemt_functors.h"
#include "gonio_functors.h"
#include "power_functors.h"
#include "error.h"
#include <iostream>
#include <fstream>

/*
 * DERIVATIVE
 */
atom * derivative::shallow_copy()
{
    return new derivative();
}

std::string derivative::get_keyword(int i)
{
    if (i == 0)
    {
        return "Deriv";
    }

    return 0;
}

bool derivative::same_class(atom * a)
{
    return dynamic_cast<derivative *>(a) != 0;
}

atom * derivative::deriv(atom * expr, atom * var)
{
    // numbers
    if (number::is_number(expr))
    {
        return new inumber(0);
    }

    // variables
    else if (expr->get_type() == atom::TYPE_VARIABLE)
    {
        if (expr->equals(var))
        {
            return new inumber(1);
        }
        else
        {
            return new inumber(0);
        }
    }

    // +
    else if (plus().same_class(expr))
    {
        atom * ret = new plus();

        ret -> set_arg(0, deriv(expr -> get_arg(0), var));
        ret -> set_arg(1, deriv(expr -> get_arg(1), var));

        return ret;
    }

    // -
    else if (minus().same_class(expr))
    {
        atom * ret = new minus();

        ret -> set_arg(0, deriv(expr -> get_arg(0), var));
        ret -> set_arg(1, deriv(expr -> get_arg(1), var));
    }

    // *
    else if (multiply().same_class(expr))
    {
        atom * ret_part_0 = new multiply();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());
        ret_part_0 -> set_arg(1, deriv(expr -> get_arg(1), var));

        atom * ret_part_1 = new multiply();

        ret_part_1 -> set_arg(0, expr -> get_arg(1) -> deep_copy());
        ret_part_1 -> set_arg(1, deriv(expr -> get_arg(0), var));

        atom * ret = new plus();

        ret -> set_arg(0, ret_part_0);
        ret -> set_arg(1, ret_part_1);

        return ret;
    }

    // :
    else if (divide().same_class(expr))
    {
        atom * ret_part_0 = new multiply();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());
        ret_part_0 -> set_arg(1, deriv(expr -> get_arg(1), var));

        atom * ret_part_1 = new multiply();

        ret_part_1 -> set_arg(0, expr -> get_arg(1) -> deep_copy());
        ret_part_1 -> set_arg(1, deriv(expr -> get_arg(0), var));

        atom * ret_part_2 = new minus();

        ret_part_2 -> set_arg(0, ret_part_1);
        ret_part_2 -> set_arg(1, ret_part_0);

        atom * ret_part_3 = new power();

        ret_part_3 -> set_arg(0, new inumber(2));
        ret_part_3 -> set_arg(1, expr -> get_arg(0) -> deep_copy());

        atom * ret = new divide();

        ret -> set_arg(0, ret_part_3);
        ret -> set_arg(1, ret_part_2);

        return ret;
    }

    // ^
    else if (power().same_class(expr))
    {
        // f(x) ^ g(x)
        atom * g = expr -> get_arg(0);
        atom * f = expr -> get_arg(1);

        // calculate g(x)-1
        atom * ret_part_0 = new minus();

        ret_part_0 -> set_arg(0, new inumber(1));
        ret_part_0 -> set_arg(1, g -> deep_copy());

        // calculate f(x)^(g(x)-1)
        atom * ret_part_1 = new power();

        ret_part_1 -> set_arg(0, ret_part_0);
        ret_part_1 -> set_arg(1, f -> deep_copy());

        // calculate log(f(x))
        atom * ret_part_2 = new logarithm();

        ret_part_2 -> set_arg(0, f -> deep_copy());

        // calculate f(x) * log(f(x))
        atom * ret_part_3 = new multiply();

        ret_part_3 -> set_arg(0, ret_part_2);
        ret_part_3 -> set_arg(1, f -> deep_copy());

        // calculate f(x) * log(f(x)) * deriv(g(x),x)
        atom * ret_part_4 = new multiply();

        ret_part_4 -> set_arg(0, deriv(g, var));
        ret_part_4 -> set_arg(1, ret_part_3);

        // calculate g(x)*deriv(f(x),x)
        atom * ret_part_5 = new multiply();

        ret_part_5 -> set_arg(0, g -> deep_copy());
        ret_part_5 -> set_arg(1, deriv(f, var));

        // calculate (g(x)*deriv(f(x)) + f(x)*log(f(x)))
        atom * ret_part_6 = new plus();

        ret_part_6 -> set_arg(0, ret_part_5);
        ret_part_6 -> set_arg(1, ret_part_4);

        atom * ret = new multiply();

        ret -> set_arg(0, ret_part_6);
        ret -> set_arg(1, ret_part_1);

        return ret;
    }

    // logarithm
    else if (logarithm().same_class(expr))
    {
        atom * div = new divide();

        div -> set_arg(0, expr -> get_arg(0) -> deep_copy());
        div -> set_arg(1, deriv(expr -> get_arg(0), var));

        return div;
    }

    // sin
    else if (sine().same_class(expr))
    {
        atom * ret_part_0 = new cosine();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret = new multiply();

        ret -> set_arg(0, ret_part_0);
        ret -> set_arg(1, deriv(expr -> get_arg(0), var));

        return ret;
    }

    // cos
    else if (cosine().same_class(expr))
    {
        atom * ret_part_0 = new sine();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret_part_1 = new unary_minus();

        ret_part_1 -> set_arg(0, ret_part_0);

        atom * ret = new multiply();

        ret -> set_arg(0, ret_part_1);
        ret -> set_arg(1, deriv(expr -> get_arg(0), var));

        return ret;
    }

    // tan
    else if (tangent().same_class(expr))
    {
        atom * ret_part_0 = new secant();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret_part_1 = new power();

        ret_part_1 -> set_arg(0, new inumber(2));
        ret_part_1 -> set_arg(1, ret_part_0);

        atom * ret = new multiply();

        ret -> set_arg(0, ret_part_1);
        ret -> set_arg(1, deriv(expr -> get_arg(0), var));

        return ret;
    }

    // cot
    else if (cotangent().same_class(expr))
    {
        atom * ret_part_0 = new cosecant();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret_part_1 = new power();

        ret_part_1 -> set_arg(0, new inumber(2));
        ret_part_1 -> set_arg(1, ret_part_0);

        atom * ret_part_2 = new unary_minus();

        ret_part_2 -> set_arg(0, ret_part_1);

        atom * ret = new multiply();

        ret -> set_arg(0, deriv(expr -> get_arg(0), var));
        ret -> set_arg(1, ret_part_2);
    }

    // csc
    else if (cosecant().same_class(expr))
    {
        atom * ret_part_0 = new cosecant();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret_part_1 = new cotangent();

        ret_part_1 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret_part_2 = new multiply();

        ret_part_2 -> set_arg(0, ret_part_0);
        ret_part_2 -> set_arg(1, ret_part_1);

        atom * ret_part_3 = new unary_minus();

        ret_part_3 -> set_arg(0, ret_part_2);

        atom * ret = new multiply();

        ret -> set_arg(0, deriv(expr -> get_arg(0), var));
        ret -> set_arg(1, ret_part_3);

        return ret;
    }

    // sec
    else if (secant().same_class(expr))
    {
        atom * ret_part_0 = new tangent();

        ret_part_0 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret_part_1 = new secant();

        ret_part_1 -> set_arg(0, expr -> get_arg(0) -> deep_copy());

        atom * ret_part_2 = new multiply();

        ret_part_2 -> set_arg(0, ret_part_0);
        ret_part_2 -> set_arg(1, ret_part_1);

        atom * ret = new multiply();

        ret -> set_arg(0, ret_part_2);
        ret -> set_arg(1, deriv(expr -> get_arg(0), var));

        return ret;
    }

    // exception (1)
    else
    {
        return new invalid_argument();
    }
}

atom * derivative::evaluate(context * c)
{
    // evaluate arguments
    atom * arg0 = get_arg(0) -> evaluate(c);
    atom * arg1 = get_arg(1) -> evaluate(c);

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

    // calculate and evaluate derivative
    atom * d      = deriv(arg1, arg0);
    atom * retval = d -> evaluate(c);

    delete d;

    // return
    return retval;
}

/*
 * NTH DERIVATIVE
 */
bool nth_derivative::same_class(atom * a)
{
    return dynamic_cast<nth_derivative *>(a) != 0;
}

atom * nth_derivative::shallow_copy()
{
    return new nth_derivative();
}

std::string nth_derivative::get_keyword(int i)
{
    if (i == 0)
    {
        return "nthDeriv";
    }

    return 0;
}

int nth_derivative::count_keywords()
{
    return 1;
}

int nth_derivative::get_arity()
{
    return 3;
}

int nth_derivative::get_precedence()
{
    return 6;
}

bool nth_derivative::is_left_associative()
{
    return false;
}

bool nth_derivative::is_pre_operand()
{
    return true;
}

atom * nth_derivative::evaluate(context * c)
{
    // evaluate arguments
    atom*    f      = get_arg(2) -> evaluate(c);
    atom*    x      = get_arg(1) -> evaluate(c);
    atom*    n_temp = get_arg(0) -> evaluate(c);
    inumber* n_atom = 0;

    // order of derivative must evaluate to inumber
    if (n_temp -> get_type() == atom::TYPE_INT_NUMBER)
    {
        n_atom = static_cast<inumber *>(n_temp);
    }
    else
    {
        delete f;
        delete x;
        delete n_temp;
        return new invalid_argument();
    }

    // order of derivative must be larger than 0
    int n = n_atom -> get_int_value();
    if (n < 0)
    {
        delete f;
        delete x;
        delete n_temp;
        return new invalid_argument();
    }

    delete n_temp;

    // 0th derivative is function itself
    if (n == 0)
    {
        delete x;
        return f;
    }

    // calculate by nesting succesive derivatives
    atom * ret = new derivative();
    ret -> set_arg(0, x -> deep_copy());
    ret -> set_arg(1, f -> deep_copy());
    for (int i = 1; i < n; i++)
    {
        atom * temp = new derivative();
        temp -> set_arg(0, x -> deep_copy());
        temp -> set_arg(1, ret);
        ret = temp;
    }
    delete x;
    delete f;

    // evaluate nth derivative
    atom * ret_evl = ret -> evaluate(c);
    delete ret;

    // return
    return ret_evl;
}

std::string nth_derivative::to_mathML()
{
    // TODO
    return "";
}

/*
 * SUBSTITUTE
 */
atom* substitute::shallow_copy()
{
    return new substitute();
}
bool substitute::same_class(atom* a)
{
    return dynamic_cast<substitute*>(a)!=0;
}
std::string substitute::get_keyword(int i)
{
    return "subs";
}
int substitute::count_keywords()
{
    return 1;
}
int substitute::get_arity()
{
    return 3;
}
int substitute::get_precedence()
{
    return 6;
}
bool substitute::is_pre_operand()
{
    return true;
}
bool substitute::is_left_associative()
{
    return false;
}
atom* substitute::evaluate(context* c)
{

    // evaluate args
    atom* arg0 = get_arg(0)->evaluate(c);       // replacement
    atom* arg1 = get_arg(1)->evaluate(c);       // variables
    atom* arg2 = get_arg(2)->evaluate(c);       // function

    // exception (1)
    if(arg0==0 || arg1==0 || arg2==0)
    {
        delete arg0;
        delete arg1;
        delete arg2;
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        delete arg1;
        delete arg2;
        return arg0;
    }
    if(error::is_error(arg1))
    {
        delete arg0;
        delete arg2;
        return arg1;
    }
    if(error::is_error(arg2))
    {
        delete arg0;
        delete arg1;
        return arg2;
    }

    // enforce correct type
    if(arg0->get_type()!=atom::TYPE_ARRAY || arg1->get_type()!=atom::TYPE_ARRAY)
    {
        delete arg0;
        delete arg1;
        delete arg2;
        return new invalid_argument();
    }

    // array -> vector
    std::vector<atom*> repl;
    for(int i=0; i<arg0->count_args(); i++)
        repl.push_back(arg0->get_arg(i));
    std::vector<atom*> vars;
    for(int i=0; i<arg1->count_args(); i++)
        vars.push_back(arg1->get_arg(i));

    atom* subs = copy_and_replace(arg2,vars,repl);
    atom* retval = subs->evaluate(c);

    delete arg0;
    delete arg1;
    delete arg2;
    delete subs;

    return retval;
}
atom* substitute::copy_and_replace(atom* f, std::vector<atom*> vars, std::vector<atom*> replacement)
{
    for(int i=0; i<vars.size(); i++)
    {
        atom* a = vars[i];
        if(a->equals(f))
            return replacement[i]->deep_copy();
    }
    atom* fcopy = f->shallow_copy();
    for(int i=0; i<f->count_args(); i++)
    {
        fcopy->set_arg(i,copy_and_replace(f->get_arg(i),vars,replacement));
    }
    return fcopy;
}
std::string substitute::to_mathML()
{
    // TODO
    return "";
}