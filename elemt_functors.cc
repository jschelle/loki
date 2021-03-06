/*
 * File:   elemt_functors.cc
 * Author: joris
 *
 */

#include "type_functors.h"
#include "context.h"
#include "elemt_functors.h"
#include "number.h"
#include "power_functors.h"
#include "error.h"
#include <iostream>
#include <algorithm>

bool atom_tuple::compare(atom_tuple* t0, atom_tuple* t1)
{
    // handle numbers
    if(number::is_number(t0->m_first) && !number::is_number(t1->m_first))
        return false;
    if(number::is_number(t1->m_first) && !number::is_number(t0->m_first))
        return true;

    std::string s0 = t0->m_first->to_mathML();
    s0 += t0->m_second->to_mathML();

    std::string s1 = t1->m_first->to_mathML();
    s1 += t1->m_second->to_mathML();

    return s0 >= s1;
}
atom_tuple::~atom_tuple()
{
    delete m_first;
    delete m_second;
}

/*
 * PLUS CHAIN
 */
plus_chain::plus_chain(atom* r)
{
    m_root = r;
    scan_tree();
}
void plus_chain::scan_tree()
{
    // init
    m_terms.clear();
    // scan tree
    scan_tree(m_root,new inumber(1));
}
void plus_chain::scan_tree(atom* a, atom* coeff)
{

    // split over '+'
    if(plus().same_class(a))
    {
        scan_tree(a->get_arg(0), coeff->deep_copy());
        scan_tree(a->get_arg(1), coeff->deep_copy());
        delete coeff;
    }
    // split over unary '-'
    else if(unary_minus().same_class(a))
    {
        atom* ncoeff = new unary_minus();
        ncoeff->set_arg(0,coeff->deep_copy());
        scan_tree(a->get_arg(0),ncoeff);
        delete coeff;
    }
    // split over '-'
    else if(minus().same_class(a))
    {
        atom* ncoeff = new unary_minus();
        ncoeff->set_arg(0, coeff->deep_copy());
        scan_tree(a->get_arg(0), ncoeff);
        scan_tree(a->get_arg(1), coeff->deep_copy());
        delete coeff;
    }
    // split over '*'
    else if(multiply().same_class(a))
    {
        atom* arg0 = a->get_arg(0);
        atom* arg1 = a->get_arg(1);
        if(number::is_number(arg0))
        {
            atom* ncoeff = new multiply();
            ncoeff->set_arg(0,coeff->deep_copy());
            ncoeff->set_arg(1,arg0->deep_copy());
            scan_tree(arg1,ncoeff);
            delete coeff;
        }
        else if(number::is_number(arg1))
        {
            atom* ncoeff = new multiply();
            ncoeff->set_arg(0,coeff->deep_copy());
            ncoeff->set_arg(1,arg1->deep_copy());
            scan_tree(arg0,ncoeff);
            delete coeff;
        }
        else
        {
            add_term(a,coeff);
        }
    }
    // default
    else
    {
        add_term(a,coeff);
    }
}
void plus_chain::add_term(atom* a, atom* coeff)
{

    /* IMPORTANT:
     * atom* a comes from the expression-tree being scanned,
     * the expression might be deleted later, and thus a must be copied before use.
     * atom* coeff was generated by scanning the tree, and can be used
     */

    // handle numbers
    if(number::is_number(a) && number::is_number(coeff))
    {
        number* n = number::mul(static_cast<number*>(a),static_cast<number*>(coeff));
        add_constant(n);
        delete coeff;
        delete n;
        return;
    }

    // find matching term
    for(int i=0; i<m_terms.size(); i++)
    {
        atom_tuple* el = m_terms[i];
        if(el->m_first->equals(a))
        {
            atom* newcoeff = new plus();
            newcoeff->set_arg(0,el->m_second);
            newcoeff->set_arg(1,coeff);
            el->m_second = newcoeff;
            return;
        }
    }

    // not found (default)
    atom_tuple* el = new atom_tuple();
    el->m_first = a->deep_copy();
    el->m_second = coeff;
    m_terms.push_back(el);

}
void plus_chain::add_constant(number* n)
{
    // attempt to find other number
    for(int i=0; i<m_terms.size(); i++)
    {
        atom_tuple* el = m_terms[i];
        if(number::is_number(el->m_first) && number::is_number(el->m_second))
        {
            number* prod = number::mul(static_cast<number*>(el->m_first),static_cast<number*>(el->m_second));
            delete el->m_second;
            delete el->m_first;
            el->m_first = number::add(n,prod);
            el->m_second = new inumber(1);
            delete prod;
            return;
        }
    }
    // not found
    atom_tuple* el = new atom_tuple();
    el->m_first = n->deep_copy();
    el->m_second = new inumber(1);
    m_terms.push_back(el);
}
atom* plus_chain::to_atom(context* c)
{

    // evaluate coefficients
    for(int i=0; i<m_terms.size(); i++)
    {
        atom_tuple* el = m_terms[i];
        atom* evalu =  el->m_second->evaluate(c);
        delete  el->m_second;
        el->m_second = evalu;
    }

    // delete terms where coeff == 0
    for(int i=m_terms.size()-1; i>=0; i--)
    {
        atom_tuple* el = m_terms[i];
        if(is_zero(el->m_second) || is_zero(el->m_first))
        {
            delete el;
            m_terms.erase(m_terms.begin()+i);
        }
    }

    // sort terms
    std::sort(m_terms.begin(),m_terms.end(),atom_tuple::compare);

    // merge into 1 atom
    // SPECIAL CASE : 0 terms
    if(m_terms.size()==0)
    {
        return new inumber(0);
    }

    // SPECIAL CASE : 1 term
    if(m_terms.size()==1)
        return create_term(m_terms[0]);

    // DEFAULT :  multiple terms
    atom* retval = new plus();
    retval->set_arg(0,create_term(m_terms[0]));
    retval->set_arg(1,create_term(m_terms[1]));

    // add further terms
    for(int i=2; i<m_terms.size(); i++)
    {
        atom* temp = new plus();
        temp->set_arg(0,retval);
        temp->set_arg(1,create_term(m_terms[i]));
        retval = temp;
    }
    return retval;
}
bool plus_chain::is_zero(atom* a)
{
    if(!number::is_number(a))
        return false;
    return static_cast<number*>(a)->get_double_value()==0.0;
}
atom* plus_chain::create_term(atom_tuple* el)
{
    // handle '0'
    if(is_zero(el->m_first) || is_zero(el->m_second))
    {
        // delete el->m_first;
        // delete el->m_second;
        return new inumber(0);
    }
    // handle '1'
    if(number::is_number(el->m_second))
    {
        if(static_cast<number*>(el->m_second)->get_double_value()==1.0)
        {
            // delete el->m_second;
            return el->m_first->deep_copy();
        }
        // numbers are handled differently
        multiply* m =new multiply();
        m->set_arg(0, el->m_first->deep_copy());
        m->set_arg(1, el->m_second->deep_copy());
        return m;
    }
    // default
    multiply* m = new multiply();
    m->set_arg(1, el->m_first->deep_copy());
    m->set_arg(0, el->m_second->deep_copy());
    return m;
}
plus_chain::~plus_chain()
{
    for(int i=0; i<m_terms.size(); i++)
    {
        delete m_terms[i];
    }
    m_terms.clear();
}

/**
 * PLUS
 */
atom* plus::shallow_copy()
{
    return new plus();
}
std::string plus::get_keyword(int i)
{
    return "+";
}
int plus::get_precedence()
{
    return 2;
}
bool plus::same_class(atom* a)
{
    return dynamic_cast<plus*>(a)!=0;
}
atom* plus::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);

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
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        atom* retval = number::add(static_cast<number*>(arg0),static_cast<number*>(arg1));
        delete arg0;
        delete arg1;
        return retval;
    }
    atom* temp = new plus();
    temp->set_arg(0, arg0);
    temp->set_arg(1, arg1);

    if(is_chain_root())
    {
        atom* retval = plus_chain(temp).to_atom(c);
        delete temp;
        return retval;
    }

    // default
    return temp;
}
std::string plus::to_mathML()
{
    return "<apply><plus/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}
bool plus::is_chain_root()
{
    if(get_parent()==0)
        return true;
    atom* parent = get_parent();
    if(plus().same_class(parent) || minus().same_class(parent) || unary_minus().same_class(parent))
        return false;
    return true;
}

/**
 * MINUS
 */
atom* minus::shallow_copy()
{
    return new minus();
}
std::string minus::get_keyword(int i)
{
    return "-";
}
int minus::get_precedence()
{
    return 2;
}
bool minus::same_class(atom* a)
{
    return dynamic_cast<minus*>(a)!=0;
}
atom* minus::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);

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
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        atom* retval = number::sub(static_cast<number*>(arg1),static_cast<number*>(arg0));
        delete arg0;
        delete arg1;
        return retval;
    }

    // default
    atom* temp = new minus();
    temp->set_arg(0, arg0);
    temp->set_arg(1, arg1);

    // chain logic
    if(is_chain_root())
    {
        atom* retval = plus_chain(temp).to_atom(c);
        delete temp;
        return retval;
    }

    // default
    return temp;
}
std::string minus::to_mathML()
{
    return "<apply><minus/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}
bool minus::is_chain_root()
{
    if(get_parent()==0)
        return true;
    atom* parent = get_parent();
    if(plus().same_class(parent) || minus().same_class(parent) || unary_minus().same_class(parent))
        return false;
    return true;
}

/**
 * UNARY MINUS
 */
atom* unary_minus::shallow_copy()
{
    return new unary_minus();
}
std::string unary_minus::get_keyword(int i)
{
    return "-";
}
int unary_minus::get_precedence()
{
    return 4;
}
bool unary_minus::same_class(atom* a)
{
    return dynamic_cast<unary_minus*>(a)!=0;
}
atom* unary_minus::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);
    // exception
    if(arg0==0)
    {
        return 0;
    }
    // numbers
    if(number::is_number(arg0))
    {
        atom* retval = number::neg(static_cast<number*>(arg0));
        delete arg0;
        return retval;
    }

    // change --(x)
    if(arg0->same_class(this))
    {
        atom* retval = arg0->get_arg(0)->deep_copy();
        delete arg0;
        return retval;
    }

    // default
    unary_minus* retval = new unary_minus();
    retval->set_arg(0,arg0);
    return retval;
}
std::string unary_minus::to_mathML()
{
    return "<apply><minus/>" + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * MULTIPLICATION CHAIN
 */

mult_chain::mult_chain(atom* a)
{
    m_root = a;
    scan_tree();
}
void mult_chain::scan_tree(atom* a, atom* p)
{
    // split over '*'
    if(multiply().same_class(a))
    {
        scan_tree(a->get_arg(0),p->deep_copy());
        scan_tree(a->get_arg(1),p->deep_copy());
        delete p;
    }
    // split over '/'
    else if(divide().same_class(a))
    {
        if(number::is_number(a->get_arg(0)))
        {
            number* nd = number::inv(static_cast<number*>(a->get_arg(0)));
            scan_tree(nd,p->deep_copy());
            scan_tree(a->get_arg(1),p->deep_copy());
        }
        else
        {
            atom* np = new unary_minus();
            np->set_arg(0,p->deep_copy());
            scan_tree(a->get_arg(0),np);
            scan_tree(a->get_arg(1),p->deep_copy());
            delete p;
        }
    }
    // split over unary '-'
    else if(unary_minus().same_class(a))
    {
        scan_tree(a->get_arg(0),p->deep_copy());
        atom* minus_one = new inumber(-1);
        scan_tree(minus_one,p->deep_copy());
        delete p;
    }
    // split over '^'
    else if(power().same_class(a))
    {
        if(number::is_number(p) && number::is_number(a->get_arg(0)))
        {
            atom* np = number::mul(static_cast<number*>(p),static_cast<number*>(a->get_arg(0)));
            scan_tree(a->get_arg(1),np);
            delete p;
        }
        else
        {
            atom* np = new multiply();
            np->set_arg(0, p->deep_copy());
            np->set_arg(1, a->get_arg(0)->deep_copy());
            scan_tree(a->get_arg(1),np);
            delete p;
        }
    }
    // default
    else
    {
        mul_factor(a,p);
    }
}
void mult_chain::scan_tree()
{

    // init
    m_factors.clear();

    // scan tree
    scan_tree(m_root,new inumber(1));

}
void mult_chain::mul_factor(atom* a, atom* power)
{

    // handle numbers
    if(number::is_number(a) && number::is_number(power))
    {
        number* n = number::pow(static_cast<number*>(a),static_cast<number*>(power));
        delete power;
        mul_constant(n);
        return;
    }

    // attempt to find matching factor
    for(int i=0; i<m_factors.size(); i++)
    {
        atom_tuple* el = m_factors[i];
        // multiply equal factors
        if(el->m_first->equals(a))
        {
            // add powers
            atom* npower = new plus();
            npower->set_arg(0, el->m_second);
            npower->set_arg(1, power);
            el->m_second = npower;
            return;
        }
    }

    // not found (default)
    atom_tuple* el = new atom_tuple();
    el->m_first = a->deep_copy();
    el->m_second = power;
    m_factors.push_back(el);
}
void mult_chain::mul_constant(number* c)
{
    // find other number
    for(int i=0; i<m_factors.size(); i++)
    {
        atom_tuple* el = m_factors[i];
        if(number::is_number(el->m_first) && number::is_number(el->m_second))
        {
            // calculate value of factor (m_first ^ m_second)
            number* temp = number::pow(static_cast<number*>(el->m_first),static_cast<number*>(el->m_second));
            // delete previous
            delete el->m_first;
            delete el->m_second;
            // set new
            el->m_first = number::mul(temp,c);
            el->m_second = new inumber(1);
            delete temp;
            return;
        }
    }
    // add new factor
    atom_tuple* el = new atom_tuple();
    el->m_first = c;
    el->m_second = new inumber(1);
    m_factors.push_back(el);
}
atom* mult_chain::to_atom(context* c)
{

    // evaluate powers
    for(int i=0; i<m_factors.size(); i++)
    {
        atom_tuple* el = m_factors[i];
        atom* evalu = el->m_second->evaluate(c);
        delete el->m_second;
        el->m_second = evalu;
    }

    // delete factors that do not contribute
    for(int i=m_factors.size()-1; i>=0; i--)
    {
        atom_tuple* el = m_factors[i];
        // delete factors where power == 0
        if(is_zero(el->m_second))
        {
            delete el;
            m_factors.erase(m_factors.begin()+i);
            continue;
        }
        // delete factors == 1
        if(number::is_number(el->m_first) && number::is_number(el->m_second))
        {
            number* c = number::pow(static_cast<number*>(el->m_first),static_cast<number*>(el->m_second));
            double cval = c->get_double_value();
            delete c;
            if(cval==1.0)
            {
                delete el;
                m_factors.erase(m_factors.begin()+i);
            }
        }
    }

    // sort terms
    std::sort(m_factors.begin(),m_factors.end(),atom_tuple::compare);

    // merge into 1 atom
    // SPECIAL CASE : 0 terms
    if(m_factors.size()==0)
    {
        return new inumber(1);
    }

    // SPECIAL CASE : 1 term
    if(m_factors.size()==1)
    {
        return create_factor(m_factors[0]);
    }

    // special case 0 is a factor
    if(is_zero(m_factors[m_factors.size()-1]->m_first) || is_zero(m_factors[m_factors.size()-1]->m_second))
    {
        return new inumber(0);
    }

    // make first part of product
    atom* retval = new multiply();
    retval->set_arg(0,create_factor(m_factors[0]));
    retval->set_arg(1,create_factor(m_factors[1]));

    // multiply further terms
    for(int i=2; i<m_factors.size(); i++)
    {
        atom* temp = new multiply();
        temp->set_arg(0,retval);
        temp->set_arg(1,create_factor(m_factors[i]));
        retval = temp;
    }
    return retval;
}
bool mult_chain::is_zero(atom* a)
{
    if(number::is_number(a))
        return static_cast<number*>(a)->get_double_value()==0.0;
    return false;
}
atom* mult_chain::create_factor(atom_tuple* el)
{

    atom* fac = el->m_first;
    atom* pow = el->m_second;

    // handle '0'
    if(number::is_number(pow))
    {
        if(static_cast<number*>(pow)->get_double_value()==0.0)
        {
            return new inumber(1);
        }
    }
    if(number::is_number(fac))
    {
        if(dynamic_cast<number*>(fac)->get_double_value()==0.0)
        {
            return new inumber(0);
        }
    }

    // handle '1'
    if(number::is_number(pow))
    {
        if(static_cast<number*>(pow)->get_double_value()==1.0)
        {
            return fac->deep_copy();
        }
    }
    if(number::is_number(fac))
    {
        if(dynamic_cast<number*>(fac)->get_double_value()==1.0)
        {
            return new inumber(1);
        }
    }

    // default
    power* m = new power();
    m->set_arg(0, pow->deep_copy());
    m->set_arg(1, fac->deep_copy());
    return m;

}
mult_chain::~mult_chain()
{
    for(int i=0; i<m_factors.size(); i++)
        delete m_factors[i];
    m_factors.clear();
}

/**
 * MULTIPLY
 */
atom* multiply::shallow_copy()
{
    return new multiply();
}
std::string multiply::get_keyword(int i)
{
    return "*";
}
int multiply::get_precedence()
{
    return 3;
}
bool multiply::same_class(atom* a)
{
    return dynamic_cast<multiply*>(a)!=0;
}
atom* multiply::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);

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
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        atom* retval = number::mul(static_cast<number*>(arg0),static_cast<number*>(arg1));
        delete arg0;
        delete arg1;
        return retval;
    }

    // distributivity
    if(plus().same_class(arg0))
    {
        // construct (arg1 * arg0->get_arg(0))
        atom* a0 = new multiply();
        a0->set_arg(0,arg0->get_arg(0)->deep_copy());
        a0->set_arg(1,arg1->deep_copy());
        // construct (arg1 * arg0->get_arg(1))
        atom* a1 = new multiply();
        a1->set_arg(0,arg0->get_arg(1)->deep_copy());
        a1->set_arg(1,arg1->deep_copy());
        // delete
        delete arg0;
        delete arg1;
        // construct (arg1 * arg0->get_arg(0)) + (arg1 * arg0->get_arg(1))
        atom* a2 = new plus();
        a2->set_arg(0,a0);
        a2->set_arg(1,a1);
        // evaluate distributed parts
        atom* retval = a2->evaluate(c);
        delete a2;
        return retval;
    }
    else if(plus().same_class(arg1))
    {
        // construct (arg0 * arg1->get_arg(0))
        atom* a0 = new multiply();
        a0->set_arg(0,arg1->get_arg(0)->deep_copy());
        a0->set_arg(1,arg0->deep_copy());
        // construct (arg0 * arg1->get_arg(1))
        atom* a1 = new multiply();
        a1->set_arg(0,arg1->get_arg(1)->deep_copy());
        a1->set_arg(1,arg0->deep_copy());
        // delete
        delete arg0;
        delete arg1;
        // construct (arg0 * arg1->get_arg(0)) + (arg0 * arg1->get_arg(1))
        atom* a2 = new plus();
        a2->set_arg(0,a0);
        a2->set_arg(1,a1);
        // evaluate distributed parts
        atom* retval = a2->evaluate(c);
        delete a2;
        return retval;
    }

    // default
    atom* temp = new multiply();
    temp->set_arg(0, arg0);
    temp->set_arg(1, arg1);

    // chain logic
    if(is_chain_root())
    {
        atom* retval = mult_chain(temp).to_atom(c);
        delete temp;
        return retval;
    }

    // default
    return temp;
}
bool multiply::is_chain_root()
{
    if(get_parent()==0)
        return true;
    atom* parent = get_parent();
    if(divide().same_class(parent) || multiply().same_class(parent) || power().same_class(parent))
        return false;
    return true;
}
std::string multiply::to_mathML()
{
    return "<apply><times/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

/**
 * DIVIDE
 */
atom* divide::shallow_copy()
{
    return new divide();
}
std::string divide::get_keyword(int i)
{
    return "/";
}
int divide::get_precedence()
{
    return 3;
}
bool divide::same_class(atom* a)
{
    return dynamic_cast<divide*>(a)!=0;
}
atom* divide::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->evaluate(c);
    atom* arg1 = get_arg(1)->evaluate(c);

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

    // exception (3)
    if(number::is_number(arg0) && static_cast<number*>(arg0)->get_double_value()==0)
    {
        delete arg0;
        delete arg1;
        return new divide_by_zero();
    }

    // numbers
    if(number::is_number(arg0) && number::is_number(arg1))
    {
        atom* retval = 0;
        if(c->get_mode()==context::SYMBOLIC_MODE)
        {
            retval = number::div(static_cast<number*>(arg1),static_cast<number*>(arg0));
        }
        else
        {
            retval = new dnumber(static_cast<number*>(arg1)->get_double_value()/static_cast<number*>(arg0)->get_double_value());
        }
        delete arg0;
        delete arg1;
        return retval;
    }

    // default
    atom* temp = new divide();
    temp->set_arg(0, arg0);
    temp->set_arg(1, arg1);

    // chain logic
    if(is_chain_root())
    {
        atom* retval = mult_chain(temp).to_atom(c);
        delete temp;
        return retval;
    }

    // default
    return temp;
}
bool divide::is_chain_root()
{
    if(get_parent()==0)
        return true;
    atom* parent = get_parent();
    if(divide().same_class(parent) || multiply().same_class(parent) || power().same_class(parent))
        return false;
    return true;
}
std::string divide::to_mathML()
{
    return "<apply><divide/>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() +"</apply>";
}

