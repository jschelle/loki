
#include "array_atom.h"
#include "command_sequence_atom.h"
#include "context.h"
#include "number.h"
#include "progr_functors.h"
#include "text_atom.h"
#include "variable.h"
#include "error.h"
#include <fstream>
#include <iostream>

/*
 * PROGRAMMING IF
 */

atom* programming_if::shallow_copy()
{
    return new programming_if();
}
bool programming_if::same_class(atom* a)
{
    return dynamic_cast<programming_if*>(a)!=0;
}
int programming_if::count_keywords()
{
    return 3;
}
std::string programming_if::get_keyword(int i)
{
    if(i==0)
        return "if";
    else if(i==1)
        return "then";
    else if(i==2)
        return "endif";
    else
        return "";
}
int programming_if::get_arity()
{
    return 2;
}
int programming_if::get_precedence()
{
    return -1;
}
bool programming_if::is_left_associative()
{
    return false;
}
bool programming_if::is_pre_operand()
{
    return true;
}
atom* programming_if::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->deep_copy();
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

    // c-style if
    if(number::is_number(arg1) && static_cast<number*>(arg1)->get_double_value()!=0)
    {
        atom* retval = arg0->evaluate(c);
        delete retval;
        delete arg0;
        delete arg1;
        return 0;
    }

    // default
    delete arg0;
    delete arg1;
    return 0;
}
std::string programming_if::to_mathML()
{
    return "";
}

/*
 * PROGRAMMING WHILE
 */

atom* programming_while::shallow_copy()
{
    return new programming_while();
}
bool programming_while::same_class(atom* a)
{
    return dynamic_cast<programming_while*>(a)!=0;
}
int programming_while::count_keywords()
{
    return 3;
}
std::string programming_while::get_keyword(int i)
{
    if(i==0)
        return "while";
    else if(i==1)
        return "do";
    else if(i==2)
        return "done";
    else
        return "";
}
int programming_while::get_arity()
{
    return 2;
}
int programming_while::get_precedence()
{
    return -1;
}
bool programming_while::is_left_associative()
{
    return false;
}
bool programming_while::is_pre_operand()
{
    return true;
}
atom* programming_while::evaluate(context* c)
{
    atom* arg0 = get_arg(0)->deep_copy();
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

    // while loop
    while(number::is_number(arg1) && static_cast<number*>(arg1)->get_double_value()!=0)
    {
        atom* retval = arg0->evaluate(c);
        delete retval;
        delete arg1;
        arg1 = get_arg(1)->evaluate(c);
    }

    // default
    delete arg0;
    delete arg1;
    return 0;
}
std::string programming_while::to_mathML()
{
    return "";
}

/*
 * PROGRAMMING PROCEDURE
 */

atom* programming_proc::shallow_copy()
{
    return new programming_proc();
}
bool programming_proc::same_class(atom* a)
{
    return dynamic_cast<programming_proc*>(a)!=0;
}
int programming_proc::count_keywords()
{
    return 5;
}
std::string programming_proc::get_keyword(int i)
{
    if(i==0)
        return "proc";
    else if(i==1)
        return "maps";
    else if(i==2)
        return "to";
    else if(i==3)
        return "by";
    else if(i==4)
        return "endproc";
    else
        return "";
}
int programming_proc::get_arity()
{
    return 4;
}
int programming_proc::get_precedence()
{
    return -1;
}
bool programming_proc::is_left_associative()
{
    return false;
}
bool programming_proc::is_pre_operand()
{
    return false;
}
atom* programming_proc::evaluate(context* c)
{
    atom* arg0 = get_arg(0);    // code
    atom* arg1 = get_arg(1);    // output
    atom* arg2 = get_arg(2);    // input
    atom* arg3 = get_arg(3);    // name

    // exception (1)
    bool exception = false;
    if(arg0==0 || arg1==0 || arg2==0 || arg3==0)
    {
        delete arg0;
        delete arg1;
        delete arg2;
        delete arg3;
        return new invalid_argument();
    }
    // exception (2)
    if(error::is_error(arg0))
    {
        delete arg1;
        delete arg2;
        delete arg3;
        return arg0;
    }
    if(error::is_error(arg1))
    {
        delete arg0;
        delete arg2;
        delete arg3;
        return arg1;
    }
    if(error::is_error(arg2))
    {
        delete arg0;
        delete arg1;
        delete arg3;
        return arg2;
    }
    if(error::is_error(arg3))
    {
        delete arg0;
        delete arg1;
        delete arg2;
        return arg3;
    }

    // check 'output'
    if(!exception && arg1->get_type()!=atom::TYPE_VARIABLE && arg1->get_type()!=atom::TYPE_ARRAY)
        exception = true;
    if(!exception && arg1->get_type()==atom::TYPE_ARRAY && !is_array_of_variables(arg1))
        exception = true;

    // check 'input'
    if(!exception && arg2->get_type()!=atom::TYPE_VARIABLE && arg2->get_type()!=atom::TYPE_ARRAY)
        exception = true;
    if(!exception && arg2->get_type()==atom::TYPE_ARRAY && !is_array_of_variables(arg2))
        exception = true;

    // check 'name'
    if(!exception && arg3->get_type()!=atom::TYPE_TEXT)
        exception = true;

    // handle exceptions
    if(exception)
    {
        delete arg0;
        delete arg1;
        delete arg2;
        delete arg3;
        return new invalid_argument();
    }

    // create vector for 'output'
    std::vector<atom*> outp;
    if(arg1->get_type()==atom::TYPE_ARRAY)
    {
        for(int i=0; i<arg1->count_args(); i++)
            outp.push_back(arg1->get_arg(i)->deep_copy());
    }
    else
    {
        outp.push_back(arg1->deep_copy());
    }

    // create vector for 'input'
    std::vector<atom*> inp;
    if(arg2->get_type()==atom::TYPE_ARRAY)
    {
        for(int i=0; i<arg2->count_args(); i++)
            inp.push_back(arg2->get_arg(arg2->count_args()-i-1)->deep_copy());
    }
    else
    {
        inp.push_back(arg2->deep_copy());
    }

    // create string for 'name'
    std::string name = static_cast<text_atom*>(arg3)->get_text();

    // create copy of 'code'
    atom* code = arg0->deep_copy();

    // create procedure
    atom* proc = new procedure_atom(name,inp,outp,code);
    c->add_functor(static_cast<functor*>(proc));

    // return
    return new text_atom(name,false);
}
std::string programming_proc::to_mathML()
{
    return "";
}
bool programming_proc::is_array_of_variables(atom* a)
{
    if(a->get_type()!=atom::TYPE_ARRAY)
        return false;
    for(int i=0; i<a->count_args(); i++)
        if(a->get_arg(i)->get_type()!=atom::TYPE_VARIABLE)
            return false;
    return true;
}

/*
 * PROGRAMMING PROCEDURE ATOM
 */

procedure_atom::procedure_atom()
{
    m_name = "";
    m_code = 0;
}
procedure_atom::procedure_atom(std::string name, std::vector<atom*> input, std::vector<atom*> output, atom* code)
{
    m_name = name;
    m_input = input;
    m_output = output;
    m_code = code;
}
atom* procedure_atom::shallow_copy()
{
    procedure_atom* cpy = new procedure_atom();
    // copy 'input'
    for(int i=0; i<m_input.size(); i++)
        cpy->m_input.push_back(m_input[i]->deep_copy());
    // copy 'output'
    for(int i=0; i<m_output.size(); i++)
        cpy->m_output.push_back(m_output[i]->deep_copy());
    // copy name
    cpy->m_name = m_name+"";
    // copy code
    cpy->m_code = m_code->deep_copy();
    // return
    return cpy;
}
bool procedure_atom::same_class(atom* a)
{
    procedure_atom* other = dynamic_cast<procedure_atom*>(a);
    if(other==0)
        return false;
    return other->m_name.compare(m_name)==0;
}
int procedure_atom::count_keywords()
{
    return 1;
}
std::string procedure_atom::get_keyword(int i)
{
    return m_name;
}
int procedure_atom::get_arity()
{
    return m_input.size();
}
int procedure_atom::get_precedence()
{
    return 6;
}
bool procedure_atom::is_pre_operand()
{
    return true;
}
bool procedure_atom::is_left_associative()
{
    return false;
}
atom* procedure_atom::evaluate(context* c)
{

    // create empty context
    context* context_copy = new context(c,true,false);

    // assign variables
    for(int i=0; i<m_input.size(); i++)
    {
        // prepend variable names with "_" to avoid collision with user input
        // since user is only allowed to input identifiers '[a-Z]+'
        std::string varname = "_"+static_cast<variable*>(m_input[i])->get_identifier();
        context_copy->set_var(varname,get_arg(i)->evaluate(c));
    }

    // evaluate code
    atom* m_code_cpy = copy_and_modify(m_code);
    atom* retval = m_code_cpy->evaluate(context_copy);
    delete m_code_cpy;
    delete retval;

    // construct return value
    if(m_output.size()==1)
    {
        std::string varname = "_"+static_cast<variable*>(m_output[0])->get_identifier();
        if(context_copy->get_var(varname)==0)
        {
            // return copy with evaluated arguments
            retval = shallow_copy();
            for(int i=0; i<m_input.size(); i++)
                retval->set_arg(i,get_arg(i)->evaluate(c));
        }
        else
            retval = context_copy->get_var(varname)->deep_copy();
    }
    else
    {
        // TODO: abort when one output value evaluates to 0x0
        std::vector<atom*> v;
        for(int i=0; i<m_output.size(); i++)
        {
            std::string varname = "_"+static_cast<variable*>(m_output[i])->get_identifier();
            v.push_back(context_copy->get_var(varname)->deep_copy());
        }
        retval = new array_atom(v,false);
    }

    // destroy context
    delete context_copy;

    // return
    return retval;
}
procedure_atom::~procedure_atom()
{
    // delete 'input'
    for(int i=0; i<m_input.size(); i++)
        delete m_input[i];
    // delete 'output'
    for(int i=0; i<m_output.size(); i++)
        delete m_output[i];
    // delete 'code'
    delete m_code;
}
std::string procedure_atom::to_mathML()
{
    std::string s = "<apply><cname>" + m_name + "</cname>";
    for(int i=0; i<count_args(); i++)
        s+=get_arg(i)->to_mathML();
    s+="</apply>";
    return s;
}
atom* procedure_atom::copy_and_modify(atom* a)
{
    if(a->get_type()==atom::TYPE_VARIABLE)
    {
        std::string id = static_cast<variable*>(a)->get_identifier();
        return new variable("_"+id);
    }
    else
    {
        atom* cpy = a->shallow_copy();
        for(int i=0; i<a->count_args(); i++)
            cpy->set_arg(i,copy_and_modify(a->get_arg(i)));
        return cpy;
    }
}

/*
 * PROGRAMMING EXTERNAL LOADING
 */

programming_load::programming_load(core* c)
{
    m_core = c;
}
atom* programming_load::shallow_copy()
{
    return new programming_load(m_core);
}
std::string programming_load::get_keyword(int i)
{
    return "load";
}
bool programming_load::is_pre_operand()
{
    return true;
}
bool programming_load::same_class(atom* a)
{
    return dynamic_cast<programming_load*>(a)!=0;
}
atom* programming_load::evaluate(context* c)
{
    // evaluate argument
    atom* arg = get_arg(0)->evaluate(c);

    // exception
    if(arg==0)
        return 0;

    // argument must be of type string_atom
    if(arg->get_type()!=atom::TYPE_TEXT)
    {
        delete arg;
        return 0;
    }

    // get filename
    std::string filename = static_cast<text_atom*>(arg)->get_text();
    delete arg;

    // tokenize file into commands
    // put commands into command sequence
    std::vector<atom*> cv;
    std::vector<std::string> commands = loadFile(filename);
    for(int i=0; i<commands.size(); i++)
    {
        atom* a = m_core->evaluate_to_atom(commands[i]);
        cv.push_back(a);
    }

    // check for errors
    bool parsing_error = false;
    for(int i=0; i<cv.size(); i++)
    {
        if(cv[i]==0)
        {
            parsing_error = true;
            break;
        }
    }
    if(parsing_error)
    {
        for(int i=0; i<cv.size(); i++)
        {
            delete cv[i];
        }
        return new bad_parse();
    }

    // default
    return new command_sequence_atom(cv,false);
}
std::vector<std::string> programming_load::loadFile(std::string filename)
{

    std::vector<std::string> instructions;
    std::ifstream file(filename.c_str());
    std::string line = "";
    std::string cmnd = "";
    int level = 0;

    while(file.is_open() && std::getline(file,line))
    {

        // skip comment lines
        if(line[0]=='#')
            continue;

        // process { and }
        for(int i=0; i<line.length(); i++)
        {
            if(line[i]=='{')
                level++;
            else if(line[i]=='}')
                level--;
        }

        // direct to output if level==0 and no concatenation
        // skip blank lines
        if(level==0 && line.length()!=0 && cmnd.length()==0)
        {
            instructions.push_back(line);
            line="";
        }
        // direct to output if level==0
        // add concatenation
        else if(level==0 && cmnd.length()!=0)
        {
            cmnd+=line;
            instructions.push_back(cmnd);
            cmnd="";
            line="";
        }
        // concatenate commands
        else if(level!=0)
        {
            cmnd+=line;
        }

    }

    // close file
    file.close();

    // return
    return instructions;
}
