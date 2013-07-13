#include "shuntingyard.h"

#include <vector>
#include <iostream>
#include "command_sequence_atom.h"
#include "array_atom.h"
#include "functor.h"

void shuntingyard::delete_binding(binding b)
{
    for(int i=0; i<b.size(); i++)
    {
        delete b[i];
        b[i] = 0;
    }
    b.clear();
}
void shuntingyard::delete_stack(std::stack<atom*> s)
{
    while(s.size()>0)
    {
        delete s.top();
        s.top() = 0;
        s.pop();
    }
}
void shuntingyard::delete_group_info(group_info g)
{
    delete g.matching_brackets;
    std::map<int,std::vector<int>* >::iterator it;
    for(it=g.matching_separators->begin(); it!=g.matching_separators->end(); it++)
        delete it->second;
    delete g.matching_separators;
}
group_info shuntingyard::process_groups(binding infix, bool* errorFlag)
{

    (*errorFlag) = false;

    // separators
    std::stack<std::vector<int>* > separators;
    std::map<int,std::vector<int>* >* matching_separators = new std::map<int,std::vector<int>* >();

    // brackets
    std::map<int,int>* matching_brackets = new std::map<int,int>();
    std::stack<int> positions;
    std::stack<int> types;

    for(int i=0; i<infix.size(); i++)
    {
        atom* t = infix[i];
        if(t->get_type()==atom::TYPE_LEFT_STRAIGHT_BRACKET || t->get_type()==atom::TYPE_LEFT_CURLY_BRACKET || t->get_type()==atom::TYPE_LEFT_ROUND_BRACKET)
        {
            positions.push(i);
            types.push(t->get_type());
            separators.push(new std::vector<int>());
        }
        else if(t->get_type()==atom::TYPE_FUNCTION_ARGUMENT_SEPARATOR)
        {
            // errors
            if(separators.size()==0)
            {
                (*errorFlag) = true;
                break;
            }
            separators.top()->push_back(i);
        }
        else if(t->get_type()==atom::TYPE_RIGHT_STRAIGHT_BRACKET || t->get_type()==atom::TYPE_RIGHT_CURLY_BRACKET || t->get_type()==atom::TYPE_RIGHT_ROUND_BRACKET)
        {
            // bracket shortage
            if(positions.size()==0)
            {
                (*errorFlag) = true;
                break;
            }
            // match brackets
            bool m0 = types.top()==atom::TYPE_LEFT_STRAIGHT_BRACKET && t->get_type()==atom::TYPE_RIGHT_STRAIGHT_BRACKET;
            bool m1 = types.top()==atom::TYPE_LEFT_CURLY_BRACKET && t->get_type()==atom::TYPE_RIGHT_CURLY_BRACKET;
            bool m2 = types.top()==atom::TYPE_LEFT_ROUND_BRACKET && t->get_type()==atom::TYPE_RIGHT_ROUND_BRACKET;
            if(!m0 && !m1 && !m2)
            {
                (*errorFlag) = true;
                break;
            }
            matching_brackets->insert(std::make_pair<int,int>(positions.top(),i));
            matching_separators->insert(std::make_pair<int,std::vector<int>* >(positions.top(),separators.top()));
            // pop
            positions.pop();
            types.pop();
            separators.pop();
        }
    }

    group_info retval;
    retval.matching_brackets = matching_brackets;
    retval.matching_separators = matching_separators;
    return retval;

}
std::vector<atom*> shuntingyard::merge_multi_keywords(std::vector<atom*> v)
{
    for(int i=0; i<v.size(); i++)
    {
        if(is_multi_keyword_operator(v[i]))
        {
            functor* fn = static_cast<functor*>(v[i]);
            int end_of_keywords = i+fn->count_keywords()-1;
            // must be possible for next parts of multi-keyword to appear
            if(end_of_keywords>=v.size())
            {
                delete_binding(v);
                return std::vector<atom*>();
            }
            // consequent keywords must appear
            for(int j=0; j<fn->count_keywords(); j++)
                if(!fn->same_class(v[j+i]))
                {
                    delete_binding(v);
                    return std::vector<atom*>();
                }
            // delete all but one
            std::vector<atom*> toDelete;
            for(int j=1; j<fn->count_keywords(); j++)
            {
                toDelete.push_back(v[i]);
                v.erase(v.begin()+i);
            }
            for(int j=0; j<toDelete.size(); j++)
                delete toDelete[j];
        }
    }
    return v;
}
binding shuntingyard::to_postfix(binding infix)
{
    // pre-processing
    bool err = false;
    group_info g = process_groups(infix,&err);
    if(err)
    {
        delete_group_info(g);
        return binding();
    }

    // postfix conversion
    binding retval = to_postfix(infix,0,infix.size(),&g);
    if(retval.size()==0)
    {
        delete_group_info(g);
        return retval;
    }

    delete_group_info(g);

    // post-processing
    retval = merge_multi_keywords(retval);

    // return
    return retval;
}

// convert to postfix
binding shuntingyard::to_postfix(binding infix, int start, int end, group_info* g)
{
    std::vector<atom*> output;
    std::stack<atom*> stk;
    for(int i=start; i<end; i++)
    {
        atom* t = infix[i];
        // if the token is an left straight bracket
        // parse whatever is between this bracket and the matching right bracket
        // create an array-atom from the return value
        if(t->get_type()==atom::TYPE_LEFT_STRAIGHT_BRACKET)
        {
            int s2 = i;
            int e2 = g->matching_brackets->find(i)->second;
            bool err = false;
            binding arr_elements = build_group(infix,s2,e2,g,&err);
            if(err)
            {
                delete_binding(output);
                delete_stack(stk);
                return std::vector<atom*>();
            }
            output.push_back(new array_atom(arr_elements,false));
            i=e2;
            continue;
        }
        // if the token is an left curly bracket
        // parse whatever is between this bracket and the matching right bracket
        // create a command sequence atom from the return value
        if(t->get_type()==atom::TYPE_LEFT_CURLY_BRACKET)
        {
            int s2 = i;
            int e2 = g->matching_brackets->find(i)->second;
            bool err = false;
            binding sqc_elements = build_group(infix,s2,e2,g,&err);
            if(err)
            {
                delete_binding(output);
                delete_stack(stk);
                return std::vector<atom*>();
            }
            output.push_back(new command_sequence_atom(sqc_elements,false));
            i=e2;
            continue;
        }
        // if the token is a number, then add it to the output queue
        if(is_number(t))
        {
            output.push_back(t->deep_copy());
        }
        // if the token is a variable, then add it to the output queue
        else if(t->get_type()==atom::TYPE_VARIABLE)
        {
            output.push_back(t->deep_copy());
        }
        // if the token is a text, then add it to the output queue
        else if(t->get_type()==atom::TYPE_TEXT)
        {
            output.push_back(t->deep_copy());
        }
        // if the token is a left parenthesis, push it onto the stack
        else if(is_left_bracket(t))
        {
            stk.push(t);
        }
        // If the token is a function argument separator (e.g., a comma)
        else if(is_argument_separator(t))
        {
            // Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
            // If no left parentheses are encountered, either the separator was misplaced or parentheses were mismatched.
            while(stk.size()>0 && !is_left_bracket(stk.top()))
            {
                output.push_back(stk.top());
                stk.pop();
            }
        }
        // if the token is a right parenthesis:
        else if(is_right_bracket(t))
        {
            // until the token at the top of the stack is a left parenthesis, pop operators off the stack and onto the output queue
            while(stk.size()>0 && !is_left_bracket(stk.top()))
            {
                output.push_back(stk.top());
                stk.pop();
            }
            // pop the left parenthesis from the stack but not onto the output queue
            // if the stack runs out without finding a left parenthesis then there are mismatched parenthesis
            if(stk.size()==0 || !is_left_bracket(stk.top()))
            {
                return std::vector<atom*>();
            }
            else
            {
                stk.pop();
            }
        }
        // if the token is an operator, o1, then:
        else if(is_operator(t))
        {
            functor* o1 = static_cast<functor*>(t);
            // while there is an operator o2, at the top of the stack
            while(stk.size()>0 && is_operator(stk.top()))
            {
                functor* o2 = static_cast<functor*>(stk.top());
                // AND
                // either o1 is left-associative and its precedence is less than or equal to that of o2
                bool cond0 = o1->is_left_associative() && (o1->get_precedence()<=o2->get_precedence());
                // or o1 is right-associative and its precedence is less than that of o2
                bool cond1 = !o1->is_left_associative() && (o1->get_precedence()<o2->get_precedence());
                // pop o2 off the stack and onto the output queue
                if(cond0 || cond1)
                {
                    stk.pop();
                    output.push_back(o2);
                }
                else
                {
                    break;
                }
            }
            // push o1 onto the stack
            stk.push(o1->deep_copy());

        }
    }
    // while there are still tokens in the stack
    while(stk.size()>0)
    {
        // if the token on the top of the stack is a parenthesis, then there are mismatched parenthesis
        if(is_left_bracket(stk.top()) || is_right_bracket(stk.top()))
        {
            delete_binding(output);
            delete_stack(stk);
            return std::vector<atom*>();
        }
        output.push_back(stk.top());
        stk.pop();
    }
    return output;
}

std::vector<atom*> shuntingyard::build_group(binding infix, int start, int end, group_info* g, bool* errorFlag)
{
    if(end==start+1)
        return std::vector<atom*>();
    std::vector<atom*> retval;
    std::vector<int>* v = g->matching_separators->find(start)->second;
    (*errorFlag) = false;
    for(int i=0; i<=v->size(); i++)
    {

        // determine start
        int s = ( i==0 ? start+1 : ((*v)[i-1]+1) );
        // determine end
        int e = ( i==v->size() ? end : (*v)[i] );

        // turn infix to postfix, and postfix to atom
        binding pst = to_postfix(infix,s,e,g);
        pst = merge_multi_keywords(pst);
        atom* a = build_tree(pst);
        delete_binding(pst);
        if(a==0)
        {
            delete_binding(retval);
            (*errorFlag) = true;
            return std::vector<atom*>();
        }
        retval.push_back(a);
    }
    return retval;
}

// build tree
atom* shuntingyard::build_tree(std::vector<atom*> postfix)
{
    // exception
    if(postfix.size()==0)
    {
        return 0;
    }
    // build trees
    bool err;
    binding trees = build_trees(postfix,&err);
    if(trees.size()!=1)
    {
        delete_binding(trees);
        return 0;
    }
    return trees[0];
}
binding shuntingyard::build_trees(binding postfix, bool* errorFlag)
{
    std::vector<atom*> trees;
    std::stack<atom*> stk;
    if(errorFlag!=0)
        (*errorFlag) = false;
    for(int i=0; i<postfix.size(); i++)
    {
        atom* atm = postfix[i]->deep_copy();
        if(atm->get_type()==atom::TYPE_OPERATOR)
        {
            functor* opr = static_cast<functor*>(atm);
            // if there aren't enough atoms left as argument then there are syntactical problems
            if(opr->get_arity()>stk.size())
            {
                // set error flag
                if(errorFlag!=0)
                    (*errorFlag)=true;
                // delete operator
                delete opr;
                // empty out stack
                delete_stack(stk);
                return std::vector<atom*>();
            }
            // assign arguments
            for(int j=0; j<opr->get_arity(); j++)
            {
                opr->set_arg(j,stk.top());
                stk.pop();
            }
            // push operator onto stack
            stk.push(opr);
        }
        else
        {
            stk.push(atm);
        }
    }
    while(!stk.empty())
    {
        trees.push_back(stk.top());
        stk.pop();
    }
    return trees;
}
// argument separator
bool shuntingyard::is_argument_separator(atom* a)
{
    return a->get_type()==atom::TYPE_FUNCTION_ARGUMENT_SEPARATOR;
}
// brackets
bool shuntingyard::is_left_bracket(atom* a)
{
    return a->get_type()==atom::TYPE_LEFT_ROUND_BRACKET;
}
bool shuntingyard::is_right_bracket(atom* a)
{
    return a->get_type()==atom::TYPE_RIGHT_ROUND_BRACKET;
}
// numbers
bool shuntingyard::is_number(atom* a)
{
    return a->get_type()==atom::TYPE_INT_NUMBER || a->get_type()==atom::TYPE_DOUBLE_NUMBER;
}
// variables
bool shuntingyard::is_variable(atom* a)
{
    return a->get_type()==atom::TYPE_VARIABLE;
}
// operators
bool shuntingyard::is_multi_keyword_operator(atom* a)
{
    if(a->get_type()==atom::TYPE_OPERATOR)
    {
        functor* fn = static_cast<functor*>(a);
        return fn->count_keywords()>1;
    }
    return false;
}
bool shuntingyard::is_operator(atom* a)
{
    return a->get_type()==atom::TYPE_OPERATOR;
}
