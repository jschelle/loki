#include "memry_functors.h"
#include "number.h"
#include "text_atom.h"
#include "variable.h"

/*
 * FUNCTION ASSIGN
 */

atom* function_assign::shallow_copy()
{
    return new function_assign();
}
std::string function_assign::get_keyword(int i)
{
    if(i==0)
        return ":=";
    if(i==1)
        return "->";
    return "";
}
int function_assign::count_keywords()
{
    return 2;
}
int function_assign::get_arity()
{
    return 3;
}
int function_assign::get_precedence()
{
    return 0;
}
bool function_assign::is_left_associative()
{
    return false;
}
bool function_assign::is_pre_operand()
{
    return false;
}
bool function_assign::same_class(atom* a)
{
    return dynamic_cast<function_assign*>(a)!=0;
}
atom* function_assign::evaluate(context* c)
{

    atom* arg0 = get_arg(0)->deep_copy(); // expression
    atom* arg1 = get_arg(1)->evaluate(c); // variable(s)
    atom* arg2 = get_arg(2)->evaluate(c); // identifier

    // exception
    if(arg0==0 || arg1==0 || arg2==0)
    {
        delete arg0;
        delete arg1;
        delete arg2;
        return 0;
    }

    // exception for identifier
    if(arg2->get_type()!=atom::TYPE_VARIABLE)
    {
        delete arg0;
        delete arg1;
        delete arg2;
        return 0;
    }

    // exception for variable(s)
    if(arg1->get_type()!=atom::TYPE_VARIABLE && arg1->get_type()!=atom::TYPE_ARRAY)
    {
        delete arg0;
        delete arg1;
        delete arg2;
        return 0;
    }

    functor* f = 0;
    if(arg1->get_type()==atom::TYPE_VARIABLE)
        f = new function_template(static_cast<variable*>(arg2),static_cast<variable*>(arg1),arg0);
    else if(arg1->get_type()==atom::TYPE_ARRAY)
    {
        std::vector<atom*> v;
        for(int i=arg1->count_args()-1; i>=0; i--)
            v.push_back(arg1->get_arg(i));
        f = new function_template(static_cast<variable*>(arg2),v,arg0);
        delete arg1;
    }

    // toevoegen aan kern
    c->add_functor(static_cast<functor*>(f->deep_copy()));

    return f;
}
std::string function_assign::to_mathML()
{
    return get_arg(1)->to_mathML();
}

/*
 * FUNCTION TEMPLATE
 */

function_template::function_template()
{
}
function_template::function_template(variable* id, std::vector<atom*> v, atom* exp)
{
    identifier = id;
    for(int i=0; i<v.size(); i++)
        variables.push_back(static_cast<variable*>(v[i]->deep_copy()));
    expression = exp;
}
function_template::function_template(variable* id, variable* v, atom* exp)
{
    identifier = id;
    variables.push_back(v);
    expression = exp;
}
atom* function_template::shallow_copy()
{
    function_template* retval = new function_template();
    retval->identifier = static_cast<variable*>(identifier->shallow_copy());
    for(int i=0; i<variables.size(); i++)
        retval->variables.push_back(static_cast<variable*>(variables[i]->shallow_copy()));
    retval->expression = expression->deep_copy();
    return retval;
}
std::string function_template::get_keyword(int i)
{
    if(i==0)
        return identifier->get_identifier();
    return "";
}
int function_template::count_keywords()
{
    return 1;
}
int function_template::get_arity()
{
    return variables.size();
}
int function_template::get_precedence()
{
    return 6;
}
bool function_template::is_left_associative()
{
    return false;
}
bool function_template::is_pre_operand()
{
    return true;
}
bool function_template::same_class(atom* a)
{
    function_template* t2 = dynamic_cast<function_template*>(a);
    if(t2==0)
        return false;
    return t2->identifier->equals(identifier);
}
atom* function_template::evaluate(context* c)
{

    // evaluate arguments
    std::map<std::string,atom*> vars;
    for(int i=0; i<get_arity(); i++)
    {
        std::string v = variables[i]->get_identifier();
        atom* a = get_arg(i)->evaluate(c);
        vars.insert(std::make_pair<std::string,atom*>(v,a));
    }

    // evaluate function
    atom* exp_copy = copy_and_modify(expression,vars);
    atom* retval = exp_copy->evaluate(c);
    delete exp_copy;

    // delete evaluated arguments
    std::map<std::string,atom*>::iterator it;
    for(it=vars.begin(); it!=vars.end(); it++)
        delete it->second;
    vars.clear();

    // return
    return retval;
}
atom* function_template::copy_and_modify(atom* root, std::map<std::string,atom*> vars)
{
    // is variable?
    if(root->get_type()==atom::TYPE_VARIABLE)
    {
        // is overwritten?
        if(vars.find(static_cast<variable*>(root)->get_identifier())!=vars.end())
        {
            return vars.find(static_cast<variable*>(root)->get_identifier())->second->deep_copy();
        }
        else
        {
            return root->shallow_copy();
        }
    }
    // not a variable
    else
    {
        atom* root_copy = root->shallow_copy();
        for(int i=0; i<root->count_args(); i++)
            root_copy->set_arg(i,copy_and_modify(root->get_arg(i),vars));
        return root_copy;
    }
}
std::string function_template::to_mathML()
{
    std::string s = "<apply><csymbol>" + identifier->to_mathML() + "</csymbol>";
    for(int i=count_args()-1; i>=0; i--)
        s+=get_arg(i)->to_mathML();
    s+="</apply>";
    return s;
}
function_template::~function_template()
{
    delete expression;
    delete identifier;
    for(int i=0; i<variables.size(); i++)
        delete variables[i];
    variables.clear();
}

/*
 * ASSIGN
 */

atom* assign::shallow_copy()
{
    return new assign();
}
std::string assign::get_keyword(int i)
{
    return ":=";
}
int assign::get_precedence()
{
    return 0;
}
bool assign::is_left_associative()
{
    return false;
}
bool assign::same_class(atom* a)
{
    return dynamic_cast<assign*>(a)!=0;
}
atom* assign::evaluate(context* c)
{
    atom* val = get_arg(0)->evaluate(c);
    atom* idt = get_arg(1)->deep_copy();

    // exception
    if(val==0 || idt==0)
    {
        delete val;
        delete idt;
        return 0;
    }

    // exception
    if(idt->get_type()!=atom::TYPE_VARIABLE)
    {
        delete val;
        delete idt;
        return 0;
    }

    // default
    atom* cpy = val->deep_copy();
    c->set_var(static_cast<variable*>(idt)->get_identifier(),cpy);
    delete idt;
    return val;
}
std::string assign::to_mathML()
{
    return get_arg(1)->to_mathML();
}

/*
 * UNASSIGN (aka FREE)
 */

atom* unassign::shallow_copy()
{
    return new unassign();
}
std::string unassign::get_keyword(int i)
{
    return "free";
}
bool unassign::same_class(atom* a)
{
    return dynamic_cast<unassign*>(a)!=0;
}
atom* unassign::evaluate(context* c)
{
    atom* idt = get_arg(0)->deep_copy();
    // exception
    if(idt==0)
        return 0;
    // exception
    if(!idt->get_type()==atom::TYPE_VARIABLE && idt->get_type()!=atom::TYPE_TEXT)
    {
        delete idt;
        return 0;
    }
    // default
    std::string identifier = idt->get_type()==atom::TYPE_VARIABLE ? static_cast<variable*>(idt)->get_identifier() : static_cast<text_atom*>(idt)->get_text();
    c->free_var(identifier);
    c->free_functor(identifier);
    return idt;
}

/*
 * IS_ASSIGNED
 */

atom* is_assigned::shallow_copy()
{
    return new is_assigned();
}
std::string is_assigned::get_keyword(int i)
{
    return "isAssigned";
}
bool is_assigned::same_class(atom* a)
{
    return dynamic_cast<is_assigned*>(a)!=0;
}
atom* is_assigned::evaluate(context* c)
{
    atom* idt = get_arg(0);
    // exception
    if(idt==0)
        return 0;
    // exception
    if(!idt->get_type()==atom::TYPE_VARIABLE && idt->get_type()!=atom::TYPE_TEXT)
        return 0;
    // default
    std::string identifier = idt->get_type()==atom::TYPE_VARIABLE ? static_cast<variable*>(idt)->get_identifier() : static_cast<text_atom*>(idt)->get_text();
    if(c->is_defined_var(identifier) || c->is_defined_functor(identifier))
        return new inumber(1);
    return new inumber(0);
}