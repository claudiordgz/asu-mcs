#ifndef  __PARSER_H__
#define  __PARSER_H__

#include <stack>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include "lexer.h"

struct Variable {
    std::string name;
    char* value;
    std::string type;
    bool BOOL;
    bool SWITCH;
};
struct Equation {
    Variable LHS;
    std::vector<Variable> RHS;
};

class Parser {
  public:
    int parse_program();
    std::map<std::string,Variable> Variables;

  private:
    LexicalAnalyzer lexer;

    int parse_globalVars();
    int parse_vardecllist();
    int parse_vardecl();
    int parse_varlist(std::stack<std::string> &newVariables);
    int parse_typename(std::stack<std::string> &newVariables);

    int parse_body();
    int parse_stmtlist();
    int parse_stmt();
    int parse_assstmt();
    int parse_expression();

    int parse_unaryOperator();
    int parse_binaryOperator();
    int parse_primary();
    int parse_ifstmt();
    int parse_whilestmt();

    int parse_switchstmt();
    int parse_caselist();
    int parse_case();

    void typecheck_equation();

    int VarNum = 0;
    int VarLast = 0;
    Variable LeftHandSide;
    std::vector<Variable> RightHandSide;
    Variable EXPRESSION;
    std::vector<Equation> AllEquations;
};


#endif