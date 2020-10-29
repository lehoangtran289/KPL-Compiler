/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include "parser.h"

#include <stdlib.h>

#include "error.h"
#include "reader.h"
#include "scanner.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
    Token *tmp = currentToken;
    currentToken = lookAhead;
    lookAhead = getValidToken();
    free(tmp);
}

void eat(TokenType tokenType) {
    if (lookAhead->tokenType == tokenType) {
        printToken(lookAhead);
        scan();
    } else
        missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
    assert("Parsing a Program ....");
    eat(KW_PROGRAM);
    eat(TK_IDENT);
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_PERIOD);
    assert("Program parsed!");
}

void compileBlock(void) {
    assert("Parsing a Block ....");
    if (lookAhead->tokenType == KW_CONST) {
        eat(KW_CONST);
        compileConstDecl();
        compileConstDecls();
        compileBlock2();
    } else
        compileBlock2();
    assert("Block parsed!");
}

void compileBlock2(void) {
    if (lookAhead->tokenType == KW_TYPE) {
        eat(KW_TYPE);
        compileTypeDecl();
        compileTypeDecls();
        compileBlock3();
    } else
        compileBlock3();
}

void compileBlock3(void) {
    if (lookAhead->tokenType == KW_VAR) {
        eat(KW_VAR);
        compileVarDecl();
        compileVarDecls();
        compileBlock4();
    } else
        compileBlock4();
}

void compileBlock4(void) {
    compileSubDecls();
    compileBlock5();
}

void compileBlock5(void) {
    eat(KW_BEGIN);
    compileStatements();
    eat(KW_END);
}

void compileConstDecls(void) {
    while (lookAhead->tokenType == TK_IDENT)
        compileConstDecl();
}

void compileConstDecl(void) {
    eat(TK_IDENT);
    eat(SB_EQ);
    compileConstant();
    eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
    while(lookAhead->tokenType == TK_IDENT)
        compileTypeDecl();
}

void compileTypeDecl(void) {
    // TODO
}

void compileVarDecls(void) {
    // TODO
}

void compileVarDecl(void) {
    // TODO
}

void compileSubDecls(void) {
    assert("Parsing subtoutines ....");
    // TODO
    assert("Subtoutines parsed ....");
}

void compileFuncDecl(void) {
    assert("Parsing a function ....");
    // TODO
    assert("Function parsed ....");
}

void compileProcDecl(void) {
    assert("Parsing a procedure ....");
    // TODO
    assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
    // TODO
}

void compileConstant(void) {
    // TODO
}

void compileConstant2(void) {
    // TODO
}

void compileType(void) {
    // TODO
}

void compileBasicType(void) {
    switch (lookAhead->tokenType) {
        case KW_INTEGER:
            eat(KW_INTEGER);
            break;

        case KW_CHAR:
            eat(KW_CHAR);
            break;

        default:
            error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

void compileParams(void) {
    // TODO
}

void compileParams2(void) {
    // TODO
}

void compileParam(void) {
    // TODO
}

void compileStatements(void) {
    // TODO
}

void compileStatements2(void) {
    // TODO
}

void compileStatement(void) {
    switch (lookAhead->tokenType) {
        case TK_IDENT:
            compileAssignSt();
            break;
        case KW_CALL:
            compileCallSt();
            break;
        case KW_BEGIN:
            compileGroupSt();
            break;
        case KW_IF:
            compileIfSt();
            break;
        case KW_WHILE:
            compileWhileSt();
            break;
        case KW_FOR:
            compileForSt();
            break;
            // EmptySt needs to check FOLLOW tokens
        case SB_SEMICOLON:
        case KW_END:
        case KW_ELSE:
            break;
            // Error occurs
        default:
            error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

void compileAssignSt(void) {
    assert("Parsing an assign statement ....");
    // TODO
    assert("Assign statement parsed ....");
}

void compileCallSt(void) {
    assert("Parsing a call statement ....");
    // TODO
    assert("Call statement parsed ....");
}

void compileGroupSt(void) {
    assert("Parsing a group statement ....");
    // TODO
    assert("Group statement parsed ....");
}

void compileIfSt(void) {
    assert("Parsing an if statement ....");
    eat(KW_IF);
    compileCondition();
    eat(KW_THEN);
    compileStatement();
    if (lookAhead->tokenType == KW_ELSE)
        compileElseSt();
    assert("If statement parsed ....");
}

void compileElseSt(void) {
    eat(KW_ELSE);
    compileStatement();
}

void compileWhileSt(void) {
    assert("Parsing a while statement ....");
    // TODO
    assert("While statement pased ....");
}

void compileForSt(void) {
    assert("Parsing a for statement ....");
    // TODO
    assert("For statement parsed ....");
}

void compileArguments(void) {
    // TODO
}

void compileArguments2(void) {
    // TODO
}

void compileCondition(void) {
    // TODO
}

void compileCondition2(void) {
    // TODO
}

void compileExpression(void) {
    assert("Parsing an expression");
    // TODO
    assert("Expression parsed");
}

void compileExpression2(void) {
    // TODO
}

void compileExpression3(void) {
    // TODO
}

void compileTerm(void) {
    compileFactor();
    while (lookAhead->tokenType == SB_TIMES || lookAhead->tokenType == SB_SLASH) {
        switch (lookAhead->tokenType) {
            case SB_TIMES:
                eat(SB_TIMES);
                compileFactor();
                break;
            case SB_SLASH:
                eat(SB_SLASH);
                compileFactor();
                break;
        }
    }
}

void compileTerm2(void) {
    switch (lookAhead->tokenType) {
        case SB_TIMES:
            eat(SB_TIMES);
            compileFactor();
            compileTerm2();
            break;
        case SB_SLASH:
            eat(SB_SLASH);
            compileFactor();
            compileTerm2();
            break;
        // check the FOLLOW set
        case SB_PLUS:
        case SB_MINUS:
        case KW_TO:
        case KW_DO:
        case SB_RPAR:
        case SB_COMMA:
        case SB_EQ:
        case SB_NEQ:
        case SB_LE:
        case SB_LT:
        case SB_GE:
        case SB_GT:
        case SB_RSEL:
        case SB_SEMICOLON:
        case KW_END:
        case KW_ELSE:
        case KW_THEN:
            break;
        default:
            error(ERR_INVALIDTERM, lookAhead->lineNo,
                  lookAhead->colNo);
    }
}

void compileFactor(void) {
    // TODO
}

void compileIndexes(void) {
    // TODO
}

int compile(char *fileName) {
    if (openInputStream(fileName) == IO_ERROR)
        return IO_ERROR;

    currentToken = NULL;
    lookAhead = getValidToken();

    compileProgram();

    free(currentToken);
    free(lookAhead);
    closeInputStream();
    return IO_SUCCESS;
}
