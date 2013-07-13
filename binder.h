/*
 * File:   binder.h
 * Author: joris
 *
 */

#ifndef BINDER_H
#define BINDER_H

#include <map>
#include <vector>
#include "atom.h"
#include "hybrid_lexer.h"

typedef std::vector<atom*> binding;

/**
 * FRACTION
 * used for keeping track of arity whilst attempting to bind tokens to atoms,
 * and thus avoiding floating-point arithmetic
 **/

struct frac_t
{
    int m_num;
    int m_den;
};
typedef struct frac_t frac;

/**
 * BINDER
 **/

class binder
{
public:
    std::vector<binding> bind(std::vector<lexeme> tokens, std::vector<functor*>* functors);
private:

    // util methods for manipulating fractions
    frac add_fracs(frac f0, frac f1);
    frac sub_fracs(frac f0, frac f1);
    int gcd(int i0, int i1);
    frac frac_one();
    frac frac_zero();

    // methods for bracket-matching
    bool check_brackets(std::vector<lexeme>* tokens);
    bool is_left_bracket(lexeme l);
    bool is_right_bracket(lexeme l);
    bool brackets_match(int left_tag, int right_tag);

    // methods for heuristical binding
    void bind_by_arity(std::vector<lexeme> tokens);
    void bind_by_arity(std::vector<lexeme>* tokens, int* chosen_options, int from, int at, int to, frac arity, std::vector<std::vector<int> >* retval);
    void merge_options(std::vector<std::vector<int> >* v0, std::vector<std::vector<int> >* v, int from, int to);
    void enlarge_by_factor(std::vector<std::vector<int> >* v, int f);
    void remove_nonviable_options(std::vector<lexeme>* l, std::vector<functor*>* o, int i);

    // build vector of atom*
    std::vector<binding> build_bindings(std::vector<lexeme>* l);
    binding build_binding(std::vector<lexeme>* l, int p);

    // util
    std::vector<lexeme> throw_away_whitespace(std::vector<lexeme> l);
    std::vector<functor*> find_functors(std::string* keyword);

    // fields
    std::map<int,int> m_matching_brackets; // keep track of bracket matches
    std::map<int,std::vector<int> > m_matching_separators;      // keep track of separators matching an opening bracket
    std::vector<std::vector<int> > m_option_indices;    // keep track of which option to select for which atom
    std::vector<functor*>* m_functors;

};

#endif