#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include "parser.h"

using namespace std;

void syntax_error();
void assert_tokenType(LexicalAnalyzer &l, TokenType type);
void type_error(const int &line_number, const string &ERROR);
void print_variables(const vector<Variable> &Variables);
void set_type(map<string, Variable> Variables, stack<string> &lastVariables, const string& type);

int Parser::parse_program() {
  Token t = lexer.GetToken();
  if (t.token_type != END_OF_FILE) {
    lexer.UngetToken(t);
    parse_globalVars();
    parse_body();
  }
  return 0;
}

int Parser::parse_globalVars() {
  Token t = lexer.GetToken();
  if (t.token_type == LBRACE) {
    lexer.UngetToken(t);
  }
  else {
    lexer.UngetToken(t);
    parse_vardecllist();
  }
  return 0;
}


int Parser::parse_body() {
  assert_tokenType(lexer, LBRACE);
  parse_stmtlist();
  assert_tokenType(lexer, RBRACE);
}


int Parser::parse_vardecllist() {
  parse_vardecl();
  Token t = lexer.GetToken();
  if (t.token_type == LBRACE) {
    lexer.UngetToken(t);
  }
  else {
    lexer.UngetToken(t);
    parse_vardecllist();
  }
  return 0;
}

// parse var_list
int Parser::parse_varlist(stack<string> &newVariables) {
  Token t = lexer.GetToken();
  if (t.token_type == ID) {
    Variable temp_node;
    temp_node.name = t.lexeme;
    newVariables.push(t.lexeme);
    Variables.insert(pair<string, Variable>(t.lexeme, temp_node));
    VarNum++;
    t = lexer.GetToken();
    if (t.token_type == COLON) {
      lexer.UngetToken(t);
    }
    else if (t.token_type == COMMA) {
      parse_varlist(newVariables);
    } else {
      syntax_error();
    }
  }
  else {
    syntax_error();
  }
}

int Parser::parse_unaryOperator() {
  assert_tokenType(lexer, NOT);
}

int Parser::parse_binaryOperator() {
  Token t = lexer.GetToken();
  switch (t.token_type) {
    case PLUS:
    case MINUS:
    case MULT:
    case DIV:
    case GREATER:
    case LESS:
    case GTEQ:
    case LTEQ:
    case EQUAL:
    case NOTEQUAL:
      break;
    default:
      syntax_error();
  }
}

int Parser::parse_primary() {
  Token t = lexer.GetToken();
  bool exists = false;
  Variable temp_node;

  switch (t.token_type) {
    case ID: {
      auto itr = Variables.find(t.lexeme);
      if (itr == Variables.end()) {
        temp_node.name = t.lexeme;
        temp_node.type = "undefined";
        Variables.insert(pair<string, Variable>(t.lexeme, temp_node));
        RightHandSide.push_back(temp_node);
        VarNum++;
      } else {
        if (itr->second.type == "undefined") {
          temp_node.type = "undefined";
          RightHandSide.push_back(temp_node);
        } else {
          temp_node.type = itr->second.type;
          RightHandSide.push_back(temp_node);
        }
        if (LeftHandSide.type == "undefined") {
          LeftHandSide.type = itr->second.type;
        }
      }
      break;
    }
    case NUM:
      temp_node.type = "int";
      RightHandSide.push_back(temp_node);
      if (LeftHandSide.type == "undefined") {
        LeftHandSide.type = "int";
      }
      break;
    case REALNUM:
      temp_node.type = "real";
      RightHandSide.push_back(temp_node);
      if (LeftHandSide.type == "undefined") {
        LeftHandSide.type = "real";
      }
      break;
    case TRUE:
    case FALSE:
      temp_node.type = "bool";
      RightHandSide.push_back(temp_node);
      if (LeftHandSide.type == "undefined") {
        LeftHandSide.type = "bool";
      }
      break;
  }
  auto itr = Variables.find(LeftHandSide.name);
  if (itr != Variables.end()) {
    itr->second.type = LeftHandSide.type;
  }
}

int Parser::parse_expression() {
  Token t = lexer.GetToken();
  string tempLeft;
  switch (t.token_type) {
    case NOT:
      lexer.UngetToken(t);
      parse_unaryOperator();
      if (LeftHandSide.type == "bool" || LeftHandSide.type == "undefined") {
        LeftHandSide.type = "bool";
      } else {
        type_error(lexer.line_no, "C3");
      }
      parse_expression();
      break;
    case PLUS:
    case MINUS:
    case MULT:
    case DIV:
      lexer.UngetToken(t);
      parse_binaryOperator();
      tempLeft = LeftHandSide.type;
      LeftHandSide.type = "undefined";
      parse_expression();
      parse_expression();
      LeftHandSide.type = tempLeft;
      break;
    case GREATER:
    case LESS:
    case GTEQ:
    case LTEQ:
    case EQUAL:
    case NOTEQUAL:
      if (LeftHandSide.type == "bool" || LeftHandSide.type == "undefined") {
        lexer.UngetToken(t);
        parse_binaryOperator();
        tempLeft = LeftHandSide.type;
        LeftHandSide.type = "undefined";
        parse_expression();
        parse_expression();
        LeftHandSide.type = tempLeft;
      }
      else {
        type_error(lexer.line_no, "C1");
      }
      break;
    case ID:
    case NUM:
    case REALNUM:
    case TRUE:
    case FALSE:
      lexer.UngetToken(t);
      parse_primary();
      break;
    default:
      syntax_error();
  }
}

int Parser::parse_assstmt() {
  Token t = lexer.GetToken();
  bool exists = false;
  auto itr = Variables.find(t.lexeme);
  if (itr != Variables.end()) {
    LeftHandSide = itr->second;
    Token equalsToken = lexer.GetToken();
    Token notToken = lexer.GetToken();
    if (notToken.token_type == NOT) {
      if (itr->second.type != "bool" && itr->second.type != "undefined") {
        type_error(lexer.line_no, "C3");
      }
      itr->second.type = "bool";
    }
    lexer.UngetToken(notToken);
    lexer.UngetToken(equalsToken);
  } else {
    Variable temp_node;
    temp_node.name = t.lexeme;
    temp_node.type = "undefined";

    Token equalsToken = lexer.GetToken();
    Token notToken = lexer.GetToken();
    if (notToken.token_type == NOT) {
      temp_node.type = "bool";
    }
    lexer.UngetToken(notToken);
    lexer.UngetToken(equalsToken);

    auto newItem = pair<string, Variable>(t.lexeme, temp_node);
    Variables.insert(newItem);
    LeftHandSide = newItem.second;
    VarLast = VarNum;
    VarNum++;
  }

  assert_tokenType(lexer, EQUAL);
  parse_expression();
  typecheck_equation();
  assert_tokenType(lexer, SEMICOLON);
}

void Parser::typecheck_equation() {
  for (auto & i : RightHandSide) {
    if ((RightHandSide[0].type == i.type) || (i.type == "undefined") || (RightHandSide[0].type == "undefined")) {
      if (RightHandSide[0].type != i.type && RightHandSide[0].type == "undefined") {
        RightHandSide[0].type = i.type;
      }
      if (RightHandSide[0].type != i.type && i.type == "undefined") {
        i.type = RightHandSide[0].type;
      }
    }
    else {
      type_error(lexer.line_no, "C2");
    }
  }
  for (auto & j : RightHandSide) {
    auto itr = Variables.find(j.name);
    if (itr != Variables.end()) {
      itr->second.type = j.type;
    }
  }
  Equation newequation;
  newequation.LHS = LeftHandSide;
  newequation.RHS = RightHandSide;
  AllEquations.push_back(newequation);
  RightHandSide.clear();
}

int Parser::parse_case() {
  assert_tokenType(lexer, CASE);
  assert_tokenType(lexer, NUM);
  assert_tokenType(lexer, COLON);
  parse_body();
}

int Parser::parse_caselist() {
  parse_case();
  Token t = lexer.GetToken();
  if (t.token_type == CASE) {
    lexer.UngetToken(t);
    parse_caselist();
  }
  else {
    lexer.UngetToken(t);
  }
}

int Parser::parse_switchstmt() {
  assert_tokenType(lexer, SWITCH);
  assert_tokenType(lexer, LPAREN);
  Token t = lexer.GetToken();
  if (t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT ||
      t.token_type == DIV|| t.token_type == ID) {
    lexer.UngetToken(t);
  } else {
    type_error(lexer.line_no, "C5");
  }
  parse_expression();
  typecheck_equation();
  assert_tokenType(lexer, RPAREN);
  assert_tokenType(lexer, LBRACE);
  parse_caselist();
  assert_tokenType(lexer, RBRACE);
}

int Parser::parse_whilestmt() {
  assert_tokenType(lexer, WHILE);
  assert_tokenType(lexer, LPAREN);
  Token t = lexer.GetToken();
  if (t.token_type == ID) {
    auto itr = Variables.find(t.lexeme);
    if (itr != Variables.end()) {
      if (itr->second.type != "bool" && itr->second.type != "undefined") {
        type_error(lexer.line_no, "C4");
      }
    }
  }
  LeftHandSide.type = "bool";
  lexer.UngetToken(t);
  parse_expression();
  typecheck_equation();
  assert_tokenType(lexer, RPAREN);
  parse_body();
}

int Parser::parse_ifstmt() {
  assert_tokenType(lexer, IF);
  assert_tokenType(lexer, LPAREN);
  Token t = lexer.GetToken();
  if (t.token_type == ID) {
    auto itr = Variables.find(t.lexeme);
    if (itr != Variables.end()) {
      if (itr->second.type != "bool" && itr->second.type != "undefined") {
        type_error(lexer.line_no, "C4");
      }
    }
  }
  LeftHandSide.type = "bool";
  lexer.UngetToken(t);
  parse_expression();
  typecheck_equation();
  assert_tokenType(lexer, RPAREN);
  parse_body();
}

int Parser::parse_stmt() {
  Token t = lexer.GetToken();
  switch (t.token_type) {
    case ID:
      lexer.UngetToken(t);
      parse_assstmt();
      break;
    case IF:
      lexer.UngetToken(t);
      parse_ifstmt();
      break;
    case WHILE:
      lexer.UngetToken(t);
      parse_whilestmt();
      break;
    case SWITCH:
      lexer.UngetToken(t);
      parse_switchstmt();
      break;
    default:
      syntax_error();
  }
}

int Parser::parse_stmtlist() {
  parse_stmt();
  Token t = lexer.GetToken();

  if (t.token_type == RBRACE) {
    lexer.UngetToken(t);
  }
  else if (t.token_type == ID || t.token_type == IF || t.token_type == WHILE || t.token_type == SWITCH){
    lexer.UngetToken(t);
    parse_stmtlist();
  }
}


// parse scope end



int Parser::parse_typename(stack<string> &lastVariables) {
  Token t = lexer.GetToken();
  switch (t.token_type) {
    case INT:
      set_type(Variables, lastVariables, "int");
      break;
    case REAL:
      set_type(Variables, lastVariables, "real");
      break;
    case BOOL:
      set_type(Variables, lastVariables, "bool");
      break;
    default:
      syntax_error();
  }
}

int Parser::parse_vardecl() {
  VarLast = VarNum;
  stack<string> lastVariables;
  parse_varlist(lastVariables);
  assert_tokenType(lexer, COLON);
  parse_typename(lastVariables);
  assert_tokenType(lexer, SEMICOLON);
}

int main() {
  Parser parser;
  parser.parse_program();
  vector<Variable> v;
  for(auto & Variable : parser.Variables) {
    v.push_back( Variable.second );
  }
  print_variables(v);
  return 0;
}

void type_error(const int &line_number, const string &ERROR) {
  cout << "TYPE MISMATCH " << line_number << " " << ERROR << endl;
  exit(1);
}

void syntax_error() {
  cout << "Syntax Error" << endl;
  exit(1);
}

void assert_tokenType(LexicalAnalyzer &l, TokenType type) {
  Token t = l.GetToken();
  if (t.token_type != type)
    syntax_error();
}

void print_variables(const vector<Variable> &Variables) {
  bool lastund = false;
  for (const auto & i : Variables) {
    if (i.type == "undefined") {
      if (! lastund) {
        cout << i.name;
      }
      if(lastund){
        cout << ", " << i.name;
      }
      lastund = true;
    }
    if (i.type == "int") {
      if (lastund) {
        cout << ": ? #" << endl;
      }
      cout << i.name;
      cout << ": int #" << endl;
      lastund = false;
    }
    if (i.type == "real") {
      if (lastund) {
        cout << ": ? #" << endl;
      }
      cout << i.name;
      cout << ": real #" << endl;
      lastund = false;
    }
    if (i.type == "bool") {
      if (lastund) {
        cout << ": ? #" << endl;
      }
      cout << i.name;
      cout << ": bool #" << endl;
      lastund = false;
    }

  }
  if (lastund) {
    cout << ": ? #" << endl;
  }
}

void set_type(map<string, Variable> Variables, stack<string> &lastVariables, const string& type) {
  while(!lastVariables.empty()) {
    auto topL = lastVariables.top();
    auto itr = Variables.find(topL);
    if (itr != Variables.end()) {
      itr->second.type = type;
    }
    lastVariables.pop();
  }
}