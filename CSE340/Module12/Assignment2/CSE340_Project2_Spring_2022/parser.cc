#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

void syntax_error() {
    cout << "Syntax Error\n";
    exit(1);
}

void Parser::parse_program() {
    currentScope = "::";
    scopes.push(currentScope);
    Token id = lexer.GetToken();
    if (id.token_type == ID) {
        Token commaSemiColonOrLbrace = lexer.GetToken();
        if (commaSemiColonOrLbrace.token_type == COMMA || commaSemiColonOrLbrace.token_type == SEMICOLON) {
            lexer.UngetToken(commaSemiColonOrLbrace);
            lexer.UngetToken(id);
            parse_global_vars();
            parse_scope();
        } else if (commaSemiColonOrLbrace.token_type == LBRACE) {
            lexer.UngetToken(commaSemiColonOrLbrace);
            lexer.UngetToken(id);
            parse_scope();
        } else {
            syntax_error();
        }
    } else {
        syntax_error();
    }
}

void Parser::parse_global_vars() {
    Token id = lexer.GetToken();
    if (id.token_type == ERROR) {
        syntax_error();
    }

    if (id.token_type == ID) {
        lexer.UngetToken(id);
        parse_var_list(SCOPE_PUBLIC);
    } else {
        lexer.UngetToken(id);
        return;
    }

    id = lexer.GetToken();
    if (id.token_type != SEMICOLON) {
        syntax_error();
    }
}

void Parser::parse_var_list(ScopeType scope) {
    Token id = lexer.GetToken();
    if (id.token_type == ERROR) {
        syntax_error();
    }

    if (id.token_type == ID) {
        Token comma = lexer.GetToken();
        if (comma.token_type == COMMA) {
            parse_var_list(scope);
        } else {
            lexer.UngetToken(comma);
        }

        VarProperties newItem;
        newItem.scope = currentScope;
        newItem.scopeType = scope;
        newItem.name = id.lexeme;
        symbolTable.insert(symbolTable.begin(), newItem);
    } else {
        syntax_error();
    }
}

void Parser::parse_scope() {
    Token scopeName = lexer.GetToken();
    Token lBrace = lexer.GetToken();
    if (scopeName.token_type == ERROR || lBrace.token_type == ERROR) {
        syntax_error();
    }

    if (scopeName.token_type == ID && lBrace.token_type == LBRACE) {
        scopes.push(currentScope);
        currentScope = scopeName.lexeme;
        parse_public_vars();
        parse_private_vars();
        parse_stmt_list();
        currentScope = scopes.top();
        scopes.pop();
        Token rBrace = lexer.GetToken();
        if (rBrace.token_type != RBRACE) {
            syntax_error();
        }
    } else {
        syntax_error();
    }
}

void Parser::parse_vars(TokenType tokenType, ScopeType scopeType) {
    Token keyword = lexer.GetToken();
    if (keyword.token_type == ERROR) {
        syntax_error();
    }

    if (keyword.token_type == tokenType) {
        Token colon = lexer.GetToken();
        if (colon.token_type == COLON) {
            parse_var_list(scopeType);
            Token semicolon = lexer.GetToken();
            if (semicolon.token_type != SEMICOLON) {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    } else {
        lexer.UngetToken(keyword);
    }
}

void Parser::parse_public_vars() {
    parse_vars(PUBLIC, SCOPE_PUBLIC);
}

void Parser::parse_private_vars() {
    parse_vars(PRIVATE, SCOPE_PRIVATE);
}

void Parser::parse_stmt_list() {
    Token id = lexer.GetToken();
    if (id.token_type == ERROR) {
        syntax_error();
    }

    if (id.token_type == ID) {
        Token equalOrLBrace = lexer.GetToken();
        if (equalOrLBrace.token_type == EQUAL) {
            lexer.UngetToken(equalOrLBrace);
            lexer.UngetToken(id);
            parse_stmt();
        } else if (equalOrLBrace.token_type == LBRACE) {
            lexer.UngetToken(equalOrLBrace);
            lexer.UngetToken(id);
            parse_scope();
        } else {
            syntax_error();
        }
        Token anotherId = lexer.GetToken();
        if (anotherId.token_type == ID) {
            lexer.UngetToken(anotherId);
            parse_stmt_list();
        } else {
            lexer.UngetToken(anotherId);
        }
    } else {
        syntax_error();
    }
}

void Parser::parse_stmt() {
    Token variableName = lexer.GetToken();
    if (variableName.token_type == ERROR) {
        syntax_error();
    }

    if (variableName.token_type == ID) {
        Token equalSign = lexer.GetToken();
        if (equalSign.token_type == EQUAL) {
            Token variableValue = lexer.GetToken();
            Token semicolonSymbol = lexer.GetToken();
            if (variableValue.token_type != ID || semicolonSymbol.token_type != SEMICOLON) {
                syntax_error();
            }

            auto parentScope = get_scope(variableName.lexeme);
            auto childScope = get_scope(variableValue.lexeme);
            cout << parentScope
                 << variableName.lexeme
                 << " = "
                 << childScope
                 << variableValue.lexeme
                 << endl;
        } else {
            lexer.UngetToken(equalSign);
            lexer.UngetToken(variableName);
            parse_scope();
        }
    } else {
        syntax_error();
    }
}

string Parser::get_scope(const string &id) {
    for (const auto &currentProps: symbolTable) {
        if (currentProps.name == id && (currentProps.scope == currentScope || currentProps.scopeType == SCOPE_PUBLIC)) {
            if (currentProps.scope == "::") {
                return "::";
            }
            return currentProps.scope + ".";
        }
    }
    return "?.";
}

int main() {
    Parser parser;
    parser.parse_program();
    return 0;
}
