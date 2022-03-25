/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <tuple>
#include <algorithm>
#include <stack>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR" // TODO: Add labels for new token types here (as string)
    ,"REALNUM", "BASE08NUM", "BASE16NUM"
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

std::tuple<TokenType, std::string> GetRealNumber(std::string currentLexeme, InputBuffer &input);
std::tuple<TokenType, std::string> GetBaseNumber(std::string currentLexeme, InputBuffer &input);
std::tuple<TokenType, std::string> GetHexNumber(std::string currentLexeme, InputBuffer &input);

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

std::string invokeType(TokenType &type, InputBuffer &input, std::string currentLexeme, std::tuple<TokenType, std::string> (*func)(std::string, InputBuffer &)) {
    auto typeAndLexeme = func(currentLexeme, input);
    TokenType t = std::get<0>(typeAndLexeme);
    std::string lexeme = std::get<1>(typeAndLexeme);
    if (t != ERROR) {
        type = t;
        return lexeme;
    } else {
        return "";
    }
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        TokenType type = NUM;
        char isOtherType;
        std::string possibleLexeme = "";
        input.GetChar(isOtherType);
        if (isOtherType == '.') {
            possibleLexeme = invokeType(type, input, tmp.lexeme, &GetRealNumber);
            if (possibleLexeme.size() == 0)
                input.UngetChar(isOtherType);
            else
                tmp.lexeme += possibleLexeme;
        } else if (isOtherType == 'x') {
            possibleLexeme = invokeType(type, input, tmp.lexeme, &GetBaseNumber);
            if (possibleLexeme.size() == 0)
                input.UngetChar(isOtherType);
            else
                tmp.lexeme += possibleLexeme;
        } else {
            input.UngetChar(isOtherType);
            possibleLexeme = invokeType(type, input, tmp.lexeme, &GetHexNumber);
            if (possibleLexeme.size() != 0)
                tmp.lexeme += possibleLexeme;

        }

        tmp.token_type = type;
        tmp.line_no = line_no;
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else {
                tmp.token_type = ERROR;
            }

            return tmp;
    }
}


std::tuple<TokenType, std::string> GetRealNumber(std::string currentLexeme, InputBuffer &input) {
    char c;
    std:string s;
    input.GetChar(c);
    if (isdigit(c)) {
        s += '.';
        while (!input.EndOfInput() && isdigit(c)) {
            s += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        return std::make_tuple(REALNUM, s);
    }
    input.UngetChar(c);
    return std::make_tuple(ERROR, s);
}

std::tuple<TokenType, std::string> GetBaseNumber(std::string currentLexeme, InputBuffer &input) {
    char first;
    std::string s;
    input.GetChar(first);
    bool invalidbase8 = currentLexeme.find("9") != std::string::npos || currentLexeme.find("8") != std::string::npos;
    if (first == '0' && !invalidbase8) {
        
        if (input.EndOfInput()) {
            input.UngetChar(first);
            return std::make_tuple(ERROR, s);
        }

        char second;
        input.GetChar(second);
        if (second == '8') {
            s += 'x';
            s += first;
            s += second;
            return std::make_tuple(BASE08NUM, s);
        }
        input.UngetChar(second);
    } else if (first == '1') {
        
        if (input.EndOfInput()) {
            input.UngetChar(first);
            return std::make_tuple(ERROR, s);
        }

        char second;
        input.GetChar(second);
        if (second == '6') {
            s += 'x';
            s += first;
            s += second;
            return std::make_tuple(BASE16NUM, s);
        }
        input.UngetChar(second);
    }
    input.UngetChar(first);
    return std::make_tuple(ERROR, s);
}

std::tuple<TokenType, std::string> GetHexNumber(std::string currentLexeme, InputBuffer &input) {
    char c;
    std:string s;
    std::stack<char> chars;             
    std::vector<char> hex_alpha = {'A', 'B', 'C', 'D', 'E', 'F'};
    std::vector<char> hex_digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    
    chars.push('z');
    input.GetChar(chars.top());
    
    if (std::find(hex_alpha.begin(), hex_alpha.end(), chars.top()) != hex_alpha.end()) {
        if (input.EndOfInput()) {
            input.UngetChar(chars.top());
            return std::make_tuple(ERROR, s);
        }
        while (!input.EndOfInput() && std::find(hex_digits.begin(), hex_digits.end(), chars.top()) != hex_digits.end()) {
            s += chars.top();
            chars.push('z');
            input.GetChar(chars.top());
        }
        if (chars.top() == 'x') {
            TokenType type = ERROR;
            string possibleLexeme = invokeType(type, input, currentLexeme, &GetBaseNumber);
            if (possibleLexeme.size() != 0 && type == BASE16NUM) {
                s += possibleLexeme;
                return std::make_tuple(type, s);
            }
        }
        
        // unget all chars in reverse order
        while (!chars.empty())
        {
            input.UngetChar(chars.top());
            chars.pop();
        }
        return std::make_tuple(ERROR, s);
    }

    input.UngetChar(chars.top());
    return std::make_tuple(ERROR, s);
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}

