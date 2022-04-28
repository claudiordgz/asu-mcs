/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <tuple>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------
typedef enum {
    END_OF_FILE = 0,
    IF, WHILE, DO, THEN, PRINT, PUBLIC, PRIVATE, SWITCH, INT, REAL, BOOL, TRUE, FALSE, CASE, NOT,
    PLUS, MINUS, DIV, MULT,
    EQUAL, COLON, COMMA, SEMICOLON,
    LBRACE, RBRACE, LPAREN, RPAREN,
    NOTEQUAL, GREATER, LESS, LTEQ, GTEQ,
    DOT, NUM, ID, ERROR, REALNUM
} TokenType;

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(const Token&);
    LexicalAnalyzer();
    int line_no;

  private:
    std::vector<Token> tokens;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    bool SkipComments();
    bool IsKeyword(std::string);
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    Token ScanNumber();
};

#endif  //__LEXER__H__
