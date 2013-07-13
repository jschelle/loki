/*
 * File:   bracket.cpp
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#include "meta_atoms.h"

// round brackets

int left_round_bracket::get_type()
{
    return atom::TYPE_LEFT_ROUND_BRACKET;
}
atom* left_round_bracket::shallow_copy()
{
    return new left_round_bracket();
}
bool left_round_bracket::same_class(atom* a)
{
    return dynamic_cast<left_round_bracket*>(a)!=0;
}
atom* left_round_bracket::evaluate(context* c)
{
    return 0;
}
std::string left_round_bracket::to_mathML()
{
    return "";
}

int right_round_bracket::get_type()
{
    return atom::TYPE_RIGHT_ROUND_BRACKET;
}
atom* right_round_bracket::shallow_copy()
{
    return new right_round_bracket();
}
bool right_round_bracket::same_class(atom* a)
{
    return dynamic_cast<right_round_bracket*>(a)!=0;
}
atom* right_round_bracket::evaluate(context* c)
{
    return 0;
}
std::string right_round_bracket::to_mathML()
{
    return "";
}

// straight brackets

int left_straight_bracket::get_type()
{
    return atom::TYPE_LEFT_STRAIGHT_BRACKET;
}
atom* left_straight_bracket::shallow_copy()
{
    return new left_straight_bracket();
}
bool left_straight_bracket::same_class(atom* a)
{
    return dynamic_cast<left_straight_bracket*>(a)!=0;
}
atom* left_straight_bracket::evaluate(context* c)
{
    return 0;
}
std::string left_straight_bracket::to_mathML()
{
    return "";
}

int right_straight_bracket::get_type()
{
    return atom::TYPE_RIGHT_STRAIGHT_BRACKET;
}
atom* right_straight_bracket::shallow_copy()
{
    return new right_straight_bracket();
}
bool right_straight_bracket::same_class(atom* a)
{
    return dynamic_cast<right_straight_bracket*>(a)!=0;
}
atom* right_straight_bracket::evaluate(context* c)
{
    return 0;
}
std::string right_straight_bracket::to_mathML()
{
    return "";
}

// curly brackets

int right_curly_bracket::get_type()
{
    return atom::TYPE_RIGHT_CURLY_BRACKET;
}
atom* right_curly_bracket::shallow_copy()
{
    return new right_curly_bracket();
}
bool right_curly_bracket::same_class(atom* a)
{
    return dynamic_cast<right_curly_bracket*>(a)!=0;
}
atom* right_curly_bracket::evaluate(context* c)
{
    return 0;
}
std::string right_curly_bracket::to_mathML()
{
    return "";
}

int left_curly_bracket::get_type()
{
    return atom::TYPE_LEFT_CURLY_BRACKET;
}
atom* left_curly_bracket::shallow_copy()
{
    return new left_curly_bracket();
}
bool left_curly_bracket::same_class(atom* a)
{
    return dynamic_cast<left_curly_bracket*>(a)!=0;
}
atom* left_curly_bracket::evaluate(context* c)
{
    return 0;
}
std::string left_curly_bracket::to_mathML()
{
    return "";
}

// separator
int separator::get_type()
{
    return atom::TYPE_FUNCTION_ARGUMENT_SEPARATOR;
}
atom* separator::shallow_copy()
{
    return new separator();
}
bool separator::same_class(atom* a)
{
    return dynamic_cast<separator*>(a)!=0;
}
atom* separator::evaluate(context* c)
{
    return 0;
}
std::string separator::to_mathML()
{
    return "";
}

// whitespace
int whitespace::get_type()
{
    return atom::TYPE_WHITESPACE;
}
atom* whitespace::shallow_copy()
{
    return new whitespace();
}
bool whitespace::same_class(atom* a)
{
    return dynamic_cast<whitespace*>(a)!=0;
}
atom* whitespace::evaluate(context* c)
{
    return 0;
}
std::string whitespace::to_mathML()
{
    return "";
}
