#include "hybrid_lexer.h"

hybrid_lexer::hybrid_lexer()
{
    make_lexer();
}
hybrid_lexer::~hybrid_lexer()
{
    delete m_lex;
}
std::vector<lexeme> hybrid_lexer::tokenize(std::string s, std::vector<functor*>* f)
{
    // construct std::vector<string> from functors
    std::vector<std::string> keywords;
    for(int i=0; i<f->size(); i++)
    {
        functor* f0 = (*f)[i];
        for(int j=0; j<f0->count_keywords(); j++)
            keywords.push_back(f0->get_keyword(j));
    }

    int pos = 0;
    std::vector<lexeme> retval;
    while(pos<s.length())
    {
        lexeme l = pick_longest(s,pos,&keywords);
        l.m_index = retval.size();
        pos = l.m_end;
        retval.push_back(l);
    }
    return retval;
}
lexeme hybrid_lexer::pick_longest(std::string s, int p, std::vector<std::string>* f)
{
    lexeme l0 = lex_by_regex(s,p);
    lexeme l1 = lex_by_list(s,p,f);
    if(l0.m_end>l1.m_end)
        return l0;
    return l1;
}
lexeme hybrid_lexer::lex_by_regex(std::string s, int p)
{
    // create lexer
    if(m_lex==0)
        make_lexer();
    // use lexer to find next lexeme
    lexeme l = m_lex->next_lexeme(s,p);
    return l;
}
void hybrid_lexer::make_lexer()
{
    // create lexer
    m_lex = new lexer();

    m_lex->add_rule("(0|1|2|3|4|5|6|7|8|9)+",hybrid_lexer::TYPE_INTEGER);    // integer
    m_lex->add_rule("(0|1|2|3|4|5|6|7|8|9)*.(0|1|2|3|4|5|6|7|8|9)+",hybrid_lexer::TYPE_DOUBLE);     // double
    m_lex->add_rule("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)(a|A|b|B|c|C|d|D|e|E|f|F|g|G|h|H|i|I|j|J|k|K|l|L|m|M|n|N|o|O|p|P|q|Q|r|R|s|S|t|T|u|U|v|V|w|W|x|X|y|Y|z|Z)*",hybrid_lexer::TYPE_VARIABLE);   // identifier
    m_lex->add_rule("'(/|_| |0|1|2|3|4|5|6|7|8|9|a|A|b|B|c|C|d|D|e|E|f|F|g|G|h|H|i|I|j|J|k|K|l|L|m|M|n|N|o|O|p|P|q|Q|r|R|s|S|t|T|u|U|v|V|w|W|x|X|y|Y|z|Z)*'",hybrid_lexer::TYPE_STRING);        // string

    m_lex->add_rule("( |\t)+",hybrid_lexer::TYPE_SPACING);        // whitespace

    m_lex->add_rule("{",hybrid_lexer::TYPE_LEFT_CURLY_BRACKET);         // curly left bracket
    m_lex->add_rule("}",hybrid_lexer::TYPE_RIGHT_CURLY_BRACKET);         // curly right bracket
    m_lex->add_rule("[",hybrid_lexer::TYPE_LEFT_STRAIGHT_BRACKET);         // square left bracket
    m_lex->add_rule("]",hybrid_lexer::TYPE_RIGHT_STRAIGHT_BRACKET);         // square right bracket
    m_lex->add_rule("\\(",hybrid_lexer::TYPE_LEFT_ROUND_BRACKET);       // round left bracket
    m_lex->add_rule("\\)",hybrid_lexer::TYPE_RIGHT_ROUND_BRACKET);      // round right bracket

    m_lex->add_rule("(;|,)",hybrid_lexer::TYPE_FUNCTION_ARGUMENT_SEPARATOR);    // function argument separator
}
lexeme hybrid_lexer::lex_by_list(std::string s, int p, std::vector<std::string>* f)
{
    int maxlen = 0;
    for(int i=0; i<f->size(); i++)
    {
        std::string op = (*f)[i];
        if(starts_with(s,op,p) && op.length()>maxlen)
        {
            maxlen = op.length();
        }
    }
    // create new lexeme
    lexeme l;
    l.m_start = p;
    l.m_end = p+maxlen;
    l.m_tag = hybrid_lexer::TYPE_OPERATOR;
    l.m_text = s.substr(p,maxlen);
    return l;
}
bool hybrid_lexer::starts_with(std::string s, std::string prefix, int p)
{
    if(p+prefix.length()>s.length())
        return false;
    for(int i=0; i<prefix.length(); i++)
    {
        char c0 = s[p+i];
        char c1 = prefix[i];
        if(c0!=c1)
            return false;
    }
    return true;
}
