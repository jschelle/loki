/*
 * File:   binder.cc
 * Author: joris
 *
 */

#include "binder.h"
#include "lexer.h"
#include "hybrid_lexer.h"
#include "number.h"
#include "variable.h"
#include "text_atom.h"
#include "meta_atoms.h"
#include <stack>
#include <iostream>
#include <stdlib.h>

std::vector<binding> binder::bind(std::vector<lexeme> tokens, std::vector<functor*>* functors)
{

    // throw away whitespaces
    std::vector<lexeme> lex2 = throw_away_whitespace(tokens);

    // clear fields
    m_matching_brackets.clear();
    m_matching_separators.clear();
    m_option_indices.clear();

    // check brackets
    if(!check_brackets(&lex2))
    {
        m_matching_brackets.clear();
        m_matching_separators.clear();
        return std::vector<binding>();
    }

    // attempt to construct solutions that satisfy arity constraints
    m_functors = functors;
    bind_by_arity(lex2);

    // construct bindings from indices
    return build_bindings(&lex2);
}
bool binder::check_brackets(std::vector<lexeme>* tokens)
{

    std::stack<int> m_brack_pos;
    std::stack<int> m_brack_tag;

    for(int i=0; i<tokens->size(); i++)
    {

        lexeme l = (*tokens)[i];

        if(is_left_bracket(l))
        {

            m_brack_pos.push(l.m_index);
            m_brack_tag.push(l.m_tag);
            m_matching_separators.insert(std::make_pair<int,std::vector<int> >(l.m_index,std::vector<int>()));

        }
        else if(is_right_bracket(l))
        {

            // stack must contain left bracket to match right one
            if(m_brack_pos.empty())
                return false;

            // pop bracket and position from stack
            int left_pos = m_brack_pos.top();
            m_brack_pos.pop();
            int left_tag = m_brack_tag.top();
            m_brack_tag.pop();

            // brackets must match
            if(!brackets_match(left_tag,l.m_tag))
                return false;

            // update fields
            m_matching_brackets.insert(std::make_pair<int,int>(left_pos,l.m_index));

        }
        else if(l.m_tag==hybrid_lexer::TYPE_FUNCTION_ARGUMENT_SEPARATOR)
        {
            // determine level
            int left_pos = m_brack_pos.top();
            m_matching_separators.find(left_pos)->second.push_back(l.m_index);
        }
    }

    // default
    return true;
}
bool binder::is_left_bracket(lexeme l)
{
    return (l.m_tag == hybrid_lexer::TYPE_LEFT_CURLY_BRACKET) || (l.m_tag==hybrid_lexer::TYPE_LEFT_ROUND_BRACKET) || (l.m_tag == hybrid_lexer::TYPE_LEFT_STRAIGHT_BRACKET);
}
bool binder::is_right_bracket(lexeme l)
{
    return (l.m_tag == hybrid_lexer::TYPE_RIGHT_CURLY_BRACKET) || (l.m_tag==hybrid_lexer::TYPE_RIGHT_ROUND_BRACKET) || (l.m_tag == hybrid_lexer::TYPE_RIGHT_STRAIGHT_BRACKET);
}
bool binder::brackets_match(int left_tag, int right_tag)
{
    // curly
    if(left_tag == hybrid_lexer::TYPE_LEFT_CURLY_BRACKET)
        return right_tag == hybrid_lexer::TYPE_RIGHT_CURLY_BRACKET;
    // round
    if(left_tag == hybrid_lexer::TYPE_LEFT_ROUND_BRACKET)
        return right_tag == hybrid_lexer::TYPE_RIGHT_ROUND_BRACKET;
    // straight
    if(left_tag == hybrid_lexer::TYPE_LEFT_STRAIGHT_BRACKET)
        return right_tag == hybrid_lexer::TYPE_RIGHT_STRAIGHT_BRACKET;
    // default
    return false;
}
void binder::bind_by_arity(std::vector<lexeme> tokens)
{

    // use recursive function
    int* locally_chosen_options = new int[tokens.size()];
    bind_by_arity(&tokens,locally_chosen_options,0,0,tokens.size(),frac_zero(), &m_option_indices);

    while(m_matching_brackets.size()>0)
    {

        // find smallest (leftmost) position where a group starts
        int left = tokens.size()+1;
        std::map<int,int>::iterator it;
        for(it=m_matching_brackets.begin(); it!=m_matching_brackets.end(); it++)
            if(it->first<left)
                left = it->first;

        // find matching right position
        int right = m_matching_brackets.find(left)->second;

        // delete from map
        m_matching_brackets.erase(m_matching_brackets.find(left));

        // call processing on parts of group (use information on separators)
        std::vector<int> sep = m_matching_separators.find(left)->second;
        for(int i=0; i<=sep.size(); i++)
        {

            // determine left boundary
            int l = (i!=0 ? sep[i-1] : left);

            // determine right boundary
            int r = (i!=sep.size() ? sep[i] : right);

            // process
            std::vector<std::vector<int> > group;
            bind_by_arity(&tokens,locally_chosen_options,l+1,l+1,r,frac_zero(),&group);

            // merge
            merge_options(&m_option_indices,&group,l+1,r);
        }

    }

    delete [] locally_chosen_options;
}
void binder::bind_by_arity(std::vector<lexeme>* lexemes, int* chosen_options, int from, int at, int to, frac arity, std::vector<std::vector<int> >* retval)
{
    // detect end of preprocessing
    if(at==to)
    {

        // arity should be 1
        if(arity.m_den!=arity.m_num)
            return;

        // debug
        if(retval==0)
            return;

        // add current configuration to retval
        std::vector<int> v;
        for(int i=0; i<lexemes->size(); i++)
            v.push_back(chosen_options[i]);
        retval->push_back(v);

    }
    else
    {

        lexeme l = (*lexemes)[at];
        // groups
        if(is_left_bracket(l))
        {
            // skip group
            chosen_options[at]=-1;
            chosen_options[m_matching_brackets.find(at)->second]=-1;
            frac arity_diff;
            if(l.m_tag==hybrid_lexer::TYPE_LEFT_ROUND_BRACKET)
            {
                arity_diff.m_num = m_matching_separators.find(at)->second.size()+1;
                arity_diff.m_den = 1;
            }
            else
            {
                arity_diff = frac_one();
            }
            bind_by_arity(lexemes,chosen_options,from,m_matching_brackets.find(at)->second+1,to,add_fracs(arity,arity_diff),retval);
            return;
        }
        // operators
        else if(l.m_tag==hybrid_lexer::TYPE_OPERATOR)
        {
            // find all options
            std::vector<functor*> options = find_functors(&l.m_text);
            remove_nonviable_options(lexemes, &options, at);
            // recursively try all options
            for(int i=0; i<options.size(); i++)
            {
                // eliminate options (1)
                if(options[i]==0)
                    continue;
                // eliminate options (2)
                // prevent 5 - 5 from being interpreted as "5 unary_minus 5"
                // TODO
                if(at!=from && (*lexemes)[at-1].m_tag!=hybrid_lexer::TYPE_OPERATOR && options[i]->get_arity()==1 && options[i]->is_pre_operand())
                {
                    continue;
                }
                // recursively try option
                chosen_options[at] = i;
                frac arity_diff;
                arity_diff.m_num = 1-options[i]->get_arity();
                arity_diff.m_den = options[i]->count_keywords();
                bind_by_arity(lexemes,chosen_options,from,at+1,to,add_fracs(arity,arity_diff),retval);
            }
            return;
        }
        // arguments
        else
        {
            chosen_options[at] = -1;
            bind_by_arity(lexemes,chosen_options,from,at+1,to,add_fracs(frac_one(),arity),retval);
            return;
        }
    }
}
std::vector<functor*> binder::find_functors(std::string* keyword)
{
    std::vector<functor*> retval;
    for(int i=0; i<m_functors->size(); i++)
    {
        functor* f = (*m_functors)[i];
        for(int j=0; j<f->count_keywords(); j++)
        {
            if(keyword->compare(f->get_keyword(j))==0)
                retval.push_back(f);
        }
    }
    return retval;
}

void binder::merge_options(std::vector<std::vector<int> >* v0, std::vector<std::vector<int> >* v, int from, int to)
{
    enlarge_by_factor(v0,v->size());
    for(int i=0; i<v0->size(); i++)
    {
        for(int j=from; j<to; j++)
        {
            (*v0)[i][j] = (*v)[i%v->size()][j];
        }
    }
}
void binder::enlarge_by_factor(std::vector<std::vector<int> >* v, int f)
{

    // if asked to enlarge by a factor of 1, don't do anything
    if(f<=1)
        return;

    // keep track of original size
    int size = v->size();

    for(int i=1; i<f; i++)
    {
        // iterate over original vector to copy each element
        for(int j=0; j<size; j++)
        {
            // copy single element
            std::vector<int> copy;
            for(int k=0; k<(*v)[j].size(); k++)
                copy.push_back((*v)[j][k]);
            // add copy to vector
            v->push_back(copy);
        }
    }
}
std::vector<binding> binder::build_bindings(std::vector<lexeme>* l)
{
    std::vector<binding> v;
    for(int i=0; i<m_option_indices.size(); i++)
        v.push_back(build_binding(l,i));
    return v;
}
binding binder::build_binding(std::vector<lexeme>* l, int p)
{
    std::vector<atom*> v;
    for(int i=0; i<l->size(); i++)
    {
        lexeme lex = (*l)[i];
        int index = m_option_indices[p][i];
        // build double
        if(lex.m_tag==hybrid_lexer::TYPE_DOUBLE)
        {
            v.push_back(new dnumber(atof(lex.m_text.c_str())));
        }
        // build integer
        else if(lex.m_tag==hybrid_lexer::TYPE_INTEGER)
        {
            v.push_back(new inumber(atoi(lex.m_text.c_str())));
        }
        // build variable
        else if(lex.m_tag==hybrid_lexer::TYPE_VARIABLE)
        {
            v.push_back(new variable(lex.m_text));
        }
        // build text
        else if(lex.m_tag==hybrid_lexer::TYPE_STRING)
        {
            v.push_back(new text_atom(lex.m_text,true));
        }
        // straight brackets
        else if(lex.m_tag==hybrid_lexer::TYPE_LEFT_STRAIGHT_BRACKET)
        {
            v.push_back(new left_straight_bracket());
        }
        else if(lex.m_tag==hybrid_lexer::TYPE_RIGHT_STRAIGHT_BRACKET)
        {
            v.push_back(new right_straight_bracket());
        }
        // curly brackets
        else if(lex.m_tag==hybrid_lexer::TYPE_LEFT_CURLY_BRACKET)
        {
            v.push_back(new left_curly_bracket());
        }
        else if(lex.m_tag==hybrid_lexer::TYPE_RIGHT_CURLY_BRACKET)
        {
            v.push_back(new right_curly_bracket());
        }
        // round brackets
        else if(lex.m_tag==hybrid_lexer::TYPE_LEFT_ROUND_BRACKET)
        {
            v.push_back(new left_round_bracket());
        }
        else if(lex.m_tag==hybrid_lexer::TYPE_RIGHT_ROUND_BRACKET)
        {
            v.push_back(new right_round_bracket());
        }
        // separator
        else if(lex.m_tag==hybrid_lexer::TYPE_FUNCTION_ARGUMENT_SEPARATOR)
        {
            v.push_back(new separator());
        }
        // operators
        else if(lex.m_tag==hybrid_lexer::TYPE_OPERATOR)
        {
            v.push_back(find_functors(&lex.m_text)[index]->deep_copy());
        }
    }
    return v;
}
frac binder::add_fracs(frac f0, frac f1)
{
    // calc numerator and denominator
    int den = f0.m_den*f1.m_den;
    int num = f0.m_num*f1.m_den + f1.m_num*f0.m_den;
    // calc gcd, attempt to simplify fraction
    int g = gcd(den,num);
    // construct return value
    frac retval;
    retval.m_den = den/g;
    retval.m_num = num/g;
    // return
    return retval;
}
frac binder::sub_fracs(frac f0, frac f1)
{
    // calc numerator and denominator
    int den = f0.m_den*f1.m_den;
    int num = f0.m_num*f1.m_den - f1.m_num*f0.m_den;
    // calc gcd, attempt to simplify fraction
    int g = gcd(den,num);
    // construct return value
    frac retval;
    retval.m_den = den/g;
    retval.m_num = num/g;
    // return
    return retval;
}
frac binder::frac_one()
{
    frac retval;
    retval.m_num = 1;
    retval.m_den = 1;
    return retval;
}
frac binder::frac_zero()
{
    frac retval;
    retval.m_num = 0;
    retval.m_den = 1;
    return retval;
}
int binder::gcd(int i0, int i1)
{
    if(i0 == 0)
        return i1;
    else if(i1 == 0)
        return i0;
    else if(i0==i1)
        return i0;
    else if(i0<0 && i1>=0)
        return gcd(-i0,i1);
    else if(i1<0 && i0>=0)
        return gcd(i0,-i1);
    else if(i0>i1)
        return gcd(i0 % i1,i1);
    else
        return gcd(i0,i1 % i0);
}
std::vector<lexeme> binder::throw_away_whitespace(std::vector<lexeme> l)
{
    std::vector<lexeme> v;
    for(int i=0; i<l.size(); i++)
        if(l[i].m_tag!=hybrid_lexer::TYPE_SPACING)
            v.push_back(l[i]);
    for(int i=0; i<v.size(); i++)
        v[i].m_index=i;
    return v;
}
void binder::remove_nonviable_options(std::vector<lexeme>* l, std::vector<functor*>* o, int p)
{
    std::vector<int> to_delete;

    lexeme lex = (*l)[p];

    for(int i=0; i<o->size(); i++)
    {
        functor* f = (*o)[i];

        // if it is a multi-keyword operator, the other keywords should appear somewhere in the lexemes
        if(f->count_keywords() > 1)
        {

            // find keyword-part index
            int keyword_index = 0;
            for(int j=0; j<f->count_keywords(); j++)
            {
                if(f->get_keyword(j).compare(lex.m_text)==0)
                {
                    keyword_index = j;
                    break;
                }
            }

            // look for occurences of other keyword-parts (LEFT of keyword_index, LEFT of p)
            int prev = p-1;
            bool found_left = true;
            for(int j=keyword_index-1; j>=0; j--)
            {
                found_left = false;
                for(int k=prev; k>=0; k--)
                {
                    lex = (*l)[k];
                    if(lex.m_text.compare(f->get_keyword(j))==0)
                    {
                        prev = k-1;
                        found_left = true;
                        break;
                    }
                }
                if(!found_left)
                {
                    break;
                }
            }

            // look for occurences of other keyword-parts (RIGHT of keyword_index, RIGHT of p)
            int next = p+1;
            bool found_right = true;
            for(int j=keyword_index+1; j<f->count_keywords(); j++)
            {
                found_right = false;
                for(int k=next; k<l->size(); k++)
                {
                    lex = (*l)[k];
                    if(lex.m_text.compare(f->get_keyword(j))==0)
                    {
                        next = k+1;
                        found_right = true;
                        break;
                    }
                }
                if(!found_right)
                {
                    break;
                }
            }

            if(!found_left || !found_right)
            {
                to_delete.push_back(i);
            }

        }

    }
    // delete elements
    for(int i=to_delete.size()-1; i>=0; i--)
    {
        (*o)[to_delete[i]] = 0;
    }
}