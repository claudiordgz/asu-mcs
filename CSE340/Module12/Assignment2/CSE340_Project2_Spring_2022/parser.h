#ifndef  __PARSER_H__
#define  __PARSER_H__

#include <stack>
#include <vector>
#include "lexer.h"

typedef enum { SCOPE_PRIVATE = 0, SCOPE_PUBLIC = 1 } ScopeType;

struct VarProperties {
    std::string name;
    std::string scope;
    ScopeType scopeType;
};

class Parser {
  public:
    void parse_program();

  private:
    LexicalAnalyzer lexer;
    std::string currentScope;
    std::stack<std::string> scopes;
    std::vector<VarProperties> symbolTable;

    void parse_global_vars();
    void parse_var_list(ScopeType);
    void parse_scope();
    void parse_public_vars();
    void parse_private_vars();
    void parse_stmt_list();
    void parse_stmt();

    std::string get_scope(const std::string&);
    void parse_vars(TokenType, ScopeType);
};


#endif