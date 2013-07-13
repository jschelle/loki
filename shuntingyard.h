#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include <vector>
#include <map>
#include <stack>
#include "atom.h"

/**
 * BINDING
 * typedef to avoid having to type std::vector<std::vector<atom*>>
 **/
typedef std::vector<atom*> binding;

/**
 * GROUP INFO
 * data structure for keeping track of brackets and separators
 **/
struct group_info_t
{
    std::map<int,int>* matching_brackets;
    std::map<int,std::vector<int>* >* matching_separators;
};
typedef struct group_info_t group_info;

/**
 * SHUNTINGYARD
 **/
class shuntingyard
{
public:
    static binding to_postfix(binding infix);
    static atom* build_tree(binding postfix);
private:
    // memory management util
    static void delete_binding(binding b);
    static void delete_stack(std::stack<atom*> s);
    static void delete_group_info(group_info g);
    // pre-processing
    static group_info process_groups(binding infix, bool* errorFlag);
    // post-processing
    static std::vector<atom*> merge_multi_keywords(std::vector<atom*> v);
    // private parsing
    static binding build_trees(binding postfix, bool* errorFlag);
    static binding to_postfix(binding infix, int start, int end, group_info* g);
    static binding build_group(binding infix, int start, int end, group_info* g, bool* errorFlag);
    // function argument separator
    static bool is_argument_separator(atom* a);
    // brackets
    static bool is_left_bracket(atom* a);
    static bool is_right_bracket(atom* a);
    // number
    static bool is_number(atom* a);
    // variable
    static bool is_variable(atom* a);
    // operator
    static bool is_multi_keyword_operator(atom* a);
    static bool is_operator(atom* a);
};

#endif
