#include "variable.h"
#include "context.h"
#include <iostream>

/**
 * VARIABLE
 **/
int variable::get_type()
{
    return atom::TYPE_VARIABLE;
}
atom* variable::shallow_copy()
{
    variable* v = new variable(m_identifier);
    return v;
}
variable::variable(std::string id)
{
    this->m_identifier=id;
}
std::string variable::get_identifier()
{
    return m_identifier;
}
bool variable::same_class(atom* a)
{
    return dynamic_cast<variable*>(a)!=0;
}
bool variable::equals(atom* a)
{
    if(!same_class(a))
        return false;
    return static_cast<variable*>(a)->m_identifier.compare(m_identifier)==0;
}
atom* variable::evaluate(context* c)
{
    if(c->is_defined_var(m_identifier))
    {
        // stop variables from evaluating to themselves
        // eg: x:=x
        atom* val = c->get_var(m_identifier);
        if(val->equals(this))
            return deep_copy();
        return val->evaluate(c);
    }
    return deep_copy();
}
std::string variable::to_mathML()
{
    return "<ci>"+m_identifier+"</ci>";
}

