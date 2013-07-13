/*
 * File:   array_functors.cc
 * Author: joris
 *
 */

#include "array_atom.h"
#include "array_functors.h"
#include "context.h"
#include "number.h"
#include "text_atom.h"
#include "variable.h"
#include "error.h"
#include "elemt_functors.h"
#include "power_functors.h"
#include <vector>

/*
 * CONCATENATE ARRAYS/STRINGS
 */

atom* concat_array::shallow_copy()
{
    return new concat_array();
}
std::string concat_array::get_keyword(int i)
{
    return "concat";
}
bool concat_array::same_class(atom* a)
{
    return dynamic_cast<concat_array*>(a)!=0;
}
atom* concat_array::evaluate(context* c)
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

    // concatenate arrays
    atom* retval = 0;
    if(arg0->get_type()==atom::TYPE_ARRAY && arg1->get_type()==atom::TYPE_ARRAY)
    {
        std::vector<atom*> v;
        for(int i=0; i<arg1->count_args(); i++)
            v.push_back(arg1->get_arg(i)->deep_copy());
        for(int i=0; i<arg0->count_args(); i++)
            v.push_back(arg0->get_arg(i)->deep_copy());
        // return
        retval = new array_atom(v,false);
    }
    // concatenate strings
    if(arg0->get_type()==atom::TYPE_TEXT && arg1->get_type()==atom::TYPE_TEXT)
    {
        retval =  new text_atom(static_cast<text_atom*>(arg1)->get_text()+static_cast<text_atom*>(arg0)->get_text(),false);
    }

    // default
    if(retval==0)
    {
        retval = new concat_array();
        retval->set_arg(0,arg0);
        retval->set_arg(1, arg1);
    }
    else
    {
        delete arg0;
        delete arg1;
    }

    return retval;
}

/*
 * CONCATENATE ARRAYS/STRINGS IN PLACE
 */
atom* concat_array_in_place::shallow_copy()
{
    return new concat_array_in_place();
}
std::string concat_array_in_place::get_keyword(int i)
{
    return "concat$";
}
bool concat_array_in_place::same_class(atom* a)
{
    return dynamic_cast<concat_array_in_place*>(a)!=0;
}
atom* concat_array_in_place::evaluate(context* c)
{

    atom* arg0 = get_arg(0)->evaluate(c);       // array
    atom* arg1 = get_arg(1)->deep_copy();       // variable

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

    // arg1 must be variable
    if(arg1->get_type()!=atom::TYPE_VARIABLE)
    {
        delete arg0;
        delete arg1;
        return new invalid_argument();
    }

    std::string identifier = static_cast<variable*>(arg1)->get_identifier();
    atom* ref = c->get_var(identifier);

    // reference may not be 0
    if(ref==0)
    {
        delete arg0;
        delete arg1;
        return new invalid_argument();
    }

    // concatenate arrays
    if(arg0->get_type()==atom::TYPE_ARRAY && ref->get_type()==atom::TYPE_ARRAY)
    {
        for(int i=0; i<arg0->count_args(); i++)
            ref->set_arg(ref->count_args(),arg0->get_arg(i)->deep_copy());
        delete arg0;
        delete arg1;
        return 0;
    }

    // concatenate strings
    if(arg0->get_type()==atom::TYPE_TEXT && ref->get_type()==atom::TYPE_TEXT)
    {
        text_atom* ta = static_cast<text_atom*>(ref);
        ta->set_text(ta->get_text()+static_cast<text_atom*>(arg0)->get_text());
        delete arg0;
        delete arg1;
        return 0;
    }

    // default
    atom* retval = new concat_array_in_place();
    retval->set_arg(0, arg0);
    retval->set_arg(1, arg1);
    return retval;
}

/*
 * CARTESIAN PRODUCT
 */

atom* cartprod_array::shallow_copy()
{
    return new cartprod_array();
}
std::string cartprod_array::get_keyword(int i)
{
    return "cartprod";
}
bool cartprod_array::same_class(atom* a)
{
    return dynamic_cast<cartprod_array*>(a)!=0;
}
atom* cartprod_array::evaluate(context* c)
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

    if(arg0->get_type()==atom::TYPE_ARRAY && arg1->get_type()==atom::TYPE_ARRAY)
    {
        std::vector<atom*> v;
        for(int i=0; i<arg1->count_args(); i++)
            for(int j=0; j<arg0->count_args(); j++)
            {
                std::vector<atom*> v2;
                v2.push_back(arg0->get_arg(j)->deep_copy());
                v2.push_back(arg1->get_arg(i)->deep_copy());
                v.push_back(new array_atom(v2,false));
            }
        delete arg0;
        delete arg1;
        return new array_atom(v,false);
    }

    atom* retval = new cartprod_array();
    retval->set_arg(0, arg0);
    retval->set_arg(1, arg1);
    return retval;
}

/*
 * LENGTH OF ARRAY/STRING
 */

atom* array_length::shallow_copy()
{
    return new array_length();
}
std::string array_length::get_keyword(int i)
{
    return "length";
}
bool array_length::is_pre_operand()
{
    return true;
}
bool array_length::same_class(atom* a)
{
    return dynamic_cast<array_length*>(a)!=0;
}
atom* array_length::evaluate(context* c)
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

    // length of array
    atom* retval = 0;
    if(arg0->get_type()==atom::TYPE_ARRAY)
    {
        int i = arg0->count_args();
        retval = new inumber(i);
    }

    // length of string
    if(arg0->get_type()==atom::TYPE_TEXT)
    {
        int i = static_cast<text_atom*>(arg0)->get_text().length();
        retval = new inumber(i);
    }

    // TODO degree of polynomial

    // default
    if(retval==0)
    {
        retval = new array_length();
        retval->set_arg(0, arg0);
    }
    else
    {
        delete arg0;
    }
    return retval;
}

/*
 * LENGTH OF ARRAY/STRING IN PLACE
 */

atom* array_length_in_place::shallow_copy()
{
    return new array_length_in_place();
}
std::string array_length_in_place::get_keyword(int i)
{
    return "length$";
}
bool array_length_in_place::same_class(atom* a)
{
    return dynamic_cast<array_length_in_place*>(a)!=0;
}
atom* array_length_in_place::evaluate(context* c)
{
    atom* arg0 = get_arg(0);

    // resolve variables
    if(arg0->get_type()==atom::TYPE_VARIABLE)
    {
        arg0 = c->get_var(static_cast<variable*>(arg0)->get_identifier());
    }

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

    // length of array
    atom* retval = 0;
    if(arg0->get_type()==atom::TYPE_ARRAY)
    {
        int i = arg0->count_args();
        retval = new inumber(i);
    }

    // length of string
    if(arg0->get_type()==atom::TYPE_TEXT)
    {
        int i = static_cast<text_atom*>(arg0)->get_text().length();
        retval = new inumber(i);
    }

    // default
    if(retval==0)
    {
        retval = new array_length_in_place();
        retval->set_arg(0, arg0->deep_copy());
    }

    return retval;
}

/*
 * SELECT ELEMENT/CHARACTER
 */

atom* select_from::shallow_copy()
{
    return new select_from();
}
std::string select_from::get_keyword(int i)
{
    return "select";
}
bool select_from::same_class(atom* a)
{
    return dynamic_cast<select_from*>(a)!=0;
}
atom* select_from::evaluate(context* c)
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

    int t = arg1->get_type();
    if((t!=atom::TYPE_ARRAY && t!=atom::TYPE_TEXT) || !(number::is_number(arg0) || is_array_of_numbers(arg0)))
    {
        delete arg0;
        delete arg1;
        return 0;
    }

    atom* retval = 0;
    // if index is number i fetch and copy element at index i
    if(number::is_number(arg0))
        retval = select_once(arg1,static_cast<number*>(arg0)->get_int_value(),true);
    else
    {
        // if index is [] return 0
        if(arg0->count_args()==0)
        {
            retval = new invalid_argument();
            // if index is [i] fetch and copy element at index i
        }
        else if(arg0->count_args()==1)
        {
            retval = select_once(arg1,static_cast<number*>(arg0->get_arg(0))->get_int_value(),true);
            // if index is [i..]
        }
        else
        {
            // cannot fetch multi-dimensional coordinate from string
            if(t==atom::TYPE_TEXT)
            {
                retval = new invalid_argument();
            }
            else
            {
                atom* temp = arg1;
                for(int i=0; i<arg0->count_args(); i++)
                {
                    int index = static_cast<number*>(arg0->get_arg(i))->get_int_value();
                    temp = select_once(temp,index,false);
                }
                retval = temp->deep_copy();
            }
        }
    }

    if(retval==0)
    {
        retval = new select_from();
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
bool select_from::is_array_of_numbers(atom* a)
{
    if(a->get_type()!=atom::TYPE_ARRAY)
        return false;
    for(int i=0; i<a->count_args(); i++)
        if(!number::is_number(a->get_arg(i)))
            return false;
    return true;
}
atom* select_from::select_once(atom* a, int i, bool copy)
{

    // exception
    if(a==0)
        return 0;

    // determine maximum index
    int maxlen = a->get_type()==atom::TYPE_ARRAY?a->count_args():static_cast<text_atom*>(a)->get_text().length();

    // check whether given index is within bounds, adjust index otherwise
    if(i<0)
        i = 0;
    if(i>=maxlen)
        i = maxlen-1;
    // determine return value
    atom* retval = 0;
    if(a->get_type()==atom::TYPE_ARRAY)
        retval = copy ? a->get_arg(i)->deep_copy() : a->get_arg(i);
    else if(a->get_type()==atom::TYPE_TEXT)
        retval = new text_atom(static_cast<text_atom*>(a)->get_text().substr(i,1),false);
    return retval;
}

/*
 * SELECT ELEMENT/CHARACTER IN PLACE
 */

atom* select_from_in_place::shallow_copy()
{
    return new select_from_in_place();
}
std::string select_from_in_place::get_keyword(int i)
{
    return "select$";
}
bool select_from_in_place::same_class(atom* a)
{
    return dynamic_cast<select_from_in_place*>(a)!=0;
}
atom* select_from_in_place::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1);

    // exception (1)
    if(arg0==0 || arg1==0)
    {
        delete arg0;
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        return arg0;
    }
    if(error::is_error(arg1))
    {
        delete arg0;
        return arg1;
    }

    int t = arg1->get_type();
    if(t==atom::TYPE_VARIABLE)
    {
        arg1 = c->get_var(static_cast<variable*>(arg1)->get_identifier());
        t = arg1->get_type();
    }
    if((t!=atom::TYPE_ARRAY && t!=atom::TYPE_TEXT) || !(number::is_number(arg0) || is_array_of_numbers(arg0)))
    {
        delete arg0;
        return 0;
    }

    atom* retval = 0;
    // if index is number i fetch and copy element at index i
    if(number::is_number(arg0))
        retval = select_once(arg1,static_cast<number*>(arg0)->get_int_value(),true);
    else
    {
        // if index is [] return 0
        if(arg0->count_args()==0)
        {
            retval=0;
            // if index is [i] fetch and copy element at index i
        }
        else if(arg0->count_args()==1)
        {
            retval = select_once(arg1,static_cast<number*>(arg0->get_arg(0))->get_int_value(),true);
            // if index is [i..]
        }
        else
        {
            // cannot fetch multi-dimensional coordinate from string
            if(t==atom::TYPE_TEXT)
            {
                retval = 0;
            }
            else
            {
                atom* temp = arg1;
                for(int i=0; i<arg0->count_args(); i++)
                {
                    int index = static_cast<number*>(arg0->get_arg(i))->get_int_value();
                    temp = select_once(temp,index,false);
                }
                retval = temp->deep_copy();
            }
        }
    }

    if(retval==0)
    {
        retval = new select_from_in_place();
        retval->set_arg(0, arg0);
        retval->set_arg(1, arg1->deep_copy());
    }
    else
    {
        delete arg0;
    }

    return retval;
}
bool select_from_in_place::is_array_of_numbers(atom* a)
{
    if(a->get_type()!=atom::TYPE_ARRAY)
        return false;
    for(int i=0; i<a->count_args(); i++)
        if(!number::is_number(a->get_arg(i)))
            return false;
    return true;
}
atom* select_from_in_place::select_once(atom* a, int i, bool copy)
{
    // exception
    if(a==0)
        return 0;
    // determine maximum index
    int maxlen = a->get_type()==atom::TYPE_ARRAY?a->count_args():static_cast<text_atom*>(a)->get_text().length();
    // check whether given index is within bounds, adjust index otherwise
    if(i<0)
        i = 0;
    if(i>=maxlen)
        i = maxlen-1;
    // determine return value
    atom* retval = 0;
    if(a->get_type()==atom::TYPE_ARRAY)
        retval = copy ? a->get_arg(i)->deep_copy() : a->get_arg(i);
    else if(a->get_type()==atom::TYPE_TEXT)
        retval = new text_atom(static_cast<text_atom*>(a)->get_text().substr(i,1),false);
    return retval;
}

/**
 * SET ELEMENT IN ARRAY
 */
bool set_element::same_class(atom* a)
{
    return dynamic_cast<set_element*>(a)!=0;
}
atom* set_element::shallow_copy()
{
    return new set_element();
}
std::string set_element::get_keyword(int i)
{
    return "set";
}
int set_element::count_keywords()
{
    return 1;
}
int set_element::get_arity()
{
    return 3;
}
int set_element::get_precedence()
{
    return 6;
}
bool set_element::is_left_associative()
{
    return false;
}
bool set_element::is_pre_operand()
{
    return false;
}
atom* set_element::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c); // value
    atom* arg1 = get_arg(1)->evaluate(c); // coordinates
    atom* arg2 = get_arg(2)->evaluate(c); // array

    if(arg2->get_type()!=atom::TYPE_ARRAY)
    {
        atom* retval = new set_element();
        retval->set_arg(0,arg0);
        retval->set_arg(1,arg1);
        retval->set_arg(2,arg2);
        return retval;
    }

    // set(<array>,<number>,<value>)
    if(arg1->get_type()==atom::TYPE_INT_NUMBER)
    {
        int index = static_cast<inumber*>(arg1)->get_int_value();
        delete arg2->get_arg(index);
        arg2->set_arg(index,arg0);
        return arg2;
    }
    // set(<multidimensional array>,<coordinate array>,<value>)
    else if(is_array_of_numbers(arg1))
    {
        atom* m = arg2;
        for(int i=0; i<arg1->count_args()-1; i++)
        {
            int index = static_cast<inumber*>(arg1->get_arg(i))->get_int_value();
            arg2 = arg2->get_arg(index);
        }
        int index = static_cast<inumber*>(arg1->get_arg(arg1->count_args()-1))->get_int_value();
        delete arg2->get_arg(index);
        arg2->set_arg(index,arg0);
        return m;
    }

    // default
    atom* retval = new set_element();
    retval->set_arg(0,arg0);
    retval->set_arg(1,arg1);
    retval->set_arg(2,arg2);
    return retval;

}
std::string set_element::to_mathML()
{
    return "set(" + get_arg(0)->to_mathML()+","+get_arg(1)->to_mathML()+","+get_arg(2)->to_mathML()+")";
}

bool set_element::is_array_of_numbers(atom* a)
{
    // check if a is an array
    if(a->get_type()!=atom::TYPE_ARRAY)
        return false;
    // check each element of array
    for(int i=0; i<a->count_args(); i++)
        if(a->get_arg(i)->get_type()!=atom::TYPE_INT_NUMBER)
            return false;
    return true;
}