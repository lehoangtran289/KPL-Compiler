/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "error.h"
#include "reader.h"
#include "scanner.h"
#include "semantics.h"

Token* currentToken;
Token* lookAhead;

extern Type* intType;
extern Type* charType;
extern Type* floatType;
extern SymTab* symtab;

void scan(void) {
    Token* tmp = currentToken;
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

// ------------------------------------------------------------

void compileProgram(void) {
    // TODO: create, enter, and exit program block
    Object* program;

    eat(KW_PROGRAM);
    eat(TK_IDENT);
    program = createProgramObject(currentToken->string);
    enterBlock(program->progAttrs->scope);

    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_PERIOD);
    exitBlock();
}

void compileBlock(void) {
    // TODO: create and declare constant objects
    Object* constantObject;

    if (lookAhead->tokenType == KW_CONST) {
        eat(KW_CONST);

        do {
            eat(TK_IDENT);
            // Check if a constant identifier is fresh in the block
            checkFreshIdent(currentToken->string);

            constantObject = createConstantObject(currentToken->string);

            eat(SB_EQ);
            constantObject->constAttrs->value = compileConstant();
            declareObject(constantObject);

            eat(SB_SEMICOLON);
        } while (lookAhead->tokenType == TK_IDENT);

        compileBlock2();
    } else
        compileBlock2();
}

void compileBlock2(void) {
    // TODO: create and declare type objects
    Object* typeObj;

    if (lookAhead->tokenType == KW_TYPE) {
        eat(KW_TYPE);

        do {
            eat(TK_IDENT);
            // Check if a constant identifier is fresh in the block
            checkFreshIdent(currentToken->string);

            typeObj = createTypeObject(currentToken->string);

            eat(SB_EQ);
            typeObj->typeAttrs->actualType = compileType();
            declareObject(typeObj);

            eat(SB_SEMICOLON);
        } while (lookAhead->tokenType == TK_IDENT);

        compileBlock3();
    } else
        compileBlock3();
}

void compileBlock3(void) {
    // TODO: create and declare variable objects
    Object* varObj;

    if (lookAhead->tokenType == KW_VAR) {
        eat(KW_VAR);

        do {
            eat(TK_IDENT);
            // Check if a constant identifier is fresh in the block
            checkFreshIdent(currentToken->string);

            varObj = createVariableObject(currentToken->string);

            eat(SB_COLON);
            varObj->varAttrs->type = compileType();
            declareObject(varObj);

            eat(SB_SEMICOLON);
        } while (lookAhead->tokenType == TK_IDENT);

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

// ------------------------------------------------------------

void compileSubDecls(void) {
    switch (lookAhead->tokenType) {
        case KW_FUNCTION:
            compileFuncDecl();
            compileSubDecls();
            break;

        case KW_PROCEDURE:
            compileProcDecl();
            compileSubDecls();
            break;

        case KW_BEGIN:
            break;

        default:
            // error(ERR_INVALIDSUBDECL, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

void compileFuncDecl(void) {
    // TODO: create and declare a function object
    Object* funcObj;
    Type* returnType;

    eat(KW_FUNCTION);
    eat(TK_IDENT);
    // Check if a function identifier is fresh in the block
    checkFreshIdent(currentToken->string);

    funcObj = createFunctionObject(currentToken->string);
    declareObject(funcObj);
    enterBlock(funcObj->funcAttrs->scope);

    compileParams();
    eat(SB_COLON);
    returnType = compileBasicType();
    funcObj->funcAttrs->returnType = returnType;

    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_SEMICOLON);
    exitBlock();
}

void compileProcDecl(void) {
    // TODO: create and declare a procedure object
    Object* procObj;

    eat(KW_PROCEDURE);
    eat(TK_IDENT);
    // Check if a procedure identifier is fresh in the block
    checkFreshIdent(currentToken->string);

    procObj = createProcedureObject(currentToken->string);
    declareObject(procObj);
    enterBlock(procObj->procAttrs->scope);

    compileParams();
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_SEMICOLON);
    exitBlock();
}

// ------------------------------------------------------------

ConstantValue* compileUnsignedConstant(void) {
    // TODO: create and return an unsigned constant value
    ConstantValue* constValue;

    switch (lookAhead->tokenType) {
        case TK_NUMBER:
            eat(TK_NUMBER);
            constValue = makeIntConstant(currentToken->value);
            break;
        case TK_FLOAT:
            eat(TK_FLOAT);
            constValue = makeFloatConstant(currentToken->value);
            break;
        case TK_IDENT:
            eat(TK_IDENT);
            // check if the constant identifier is declared and get its value
            Object* obj = checkDeclaredConstant(currentToken->string);
            constValue = duplicateConstantValue(obj->constAttrs->value);
            break;
        case TK_CHAR:
            eat(TK_CHAR);
            constValue = makeCharConstant(currentToken->value);
            break;
        default:
            error(ERR_INVALID_CONSTANT, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
    return constValue;
}

ConstantValue* compileConstant(void) {
    // TODO: create and return a constant
    ConstantValue* constValue;

    switch (lookAhead->tokenType) {
        case SB_PLUS:
            eat(SB_PLUS);
            constValue = compileConstant2();
            break;
        case SB_MINUS:
            eat(SB_MINUS);
            constValue = compileConstant2();
            constValue->intValue = -constValue->intValue;
            break;
        case TK_CHAR:
            eat(TK_CHAR);
            constValue = makeCharConstant(currentToken->string[0]);
            break;
        default:
            constValue = compileConstant2();
            break;
    }
    return constValue;
}

ConstantValue* compileConstant2(void) {
    // TODO: create and return a constant value
    ConstantValue* constValue;

    switch (lookAhead->tokenType) {
        case TK_NUMBER:
            eat(TK_NUMBER);
            constValue = makeIntConstant(currentToken->value);
            break;
        case TK_FLOAT:
            eat(TK_FLOAT);
            constValue = makeFloatConstant(currentToken->value);
            break;
        case TK_IDENT:
            eat(TK_IDENT);
            Object* obj = checkDeclaredConstant(currentToken->string);
            if (obj->constAttrs->value->type == TP_INT)
                constValue = duplicateConstantValue(obj->constAttrs->value);
            else
                error(ERR_UNDECLARED_INT_CONSTANT, currentToken->lineNo, currentToken->colNo);
            break;
        default:
            error(ERR_INVALID_CONSTANT, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
    return constValue;
}

// ------------------------------------------------------------

Type* compileType(void) {
    // TODO: create and return a type
    Type* type;
    Type* elementType;
    int arraySize;
    Object* obj;

    switch (lookAhead->tokenType) {
        case KW_INTEGER:
            eat(KW_INTEGER);
            type = makeIntType();
            break;
        case KW_FLOAT:
            eat(KW_FLOAT);
            type = makeFloatType();
            break;
        case KW_CHAR:
            eat(KW_CHAR);
            type = makeCharType();
            break;
        case TK_IDENT:
            eat(TK_IDENT);
            // check if the type identifier is declared and get its actual type
            obj = checkDeclaredType(currentToken->string);
            type = duplicateType(obj->typeAttrs->actualType);
            break;
        case KW_ARRAY:
            eat(KW_ARRAY);
            eat(SB_LSEL);
            eat(TK_NUMBER);
            arraySize = currentToken->value;
            eat(SB_RSEL);
            eat(KW_OF);
            elementType = compileType();
            type = makeArrayType(arraySize, elementType);
            break;

        default:
            error(ERR_INVALID_TYPE, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
    return type;
}

Type* compileBasicType(void) {
    // TODO: create and return a basic type
    Type* type;

    switch (lookAhead->tokenType) {
        case KW_INTEGER:
            eat(KW_INTEGER);
            type = makeIntType();
            break;
        case KW_FLOAT:
            eat(KW_FLOAT);
            type = makeFloatType();
            break;
        case KW_CHAR:
            eat(KW_CHAR);
            type = makeCharType();
            break;
        default:
            error(ERR_INVALID_BASICTYPE, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
    return type;
}

// ------------------------------------------------------------

void compileParams(void) {
    if (lookAhead->tokenType == SB_LPAR) {
        eat(SB_LPAR);
        compileParam();
        while (lookAhead->tokenType == SB_SEMICOLON) {
            eat(SB_SEMICOLON);
            compileParam();
        }
        eat(SB_RPAR);
    }
}

void compileParam(void) {
    // TODO: create and declare a parameter
    Object* paramObj;
    Type* type;
    enum ParamKind paramKind;

    switch (lookAhead->tokenType) {
        case TK_IDENT:
            paramKind = PARAM_VALUE;
            break;
        case KW_VAR:
            eat(KW_VAR);
            paramKind = PARAM_REFERENCE;
            break;
        default:
            error(ERR_INVALID_PARAMETER, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
    eat(TK_IDENT);
    // check if the parameter identifier is fresh in the block
    checkFreshIdent(currentToken->string);

    paramObj = createParameterObject(currentToken->string, paramKind, symtab->currentScope->owner);
    eat(SB_COLON);
    type = compileBasicType();
    paramObj->paramAttrs->type = type;
    declareObject(paramObj);
}

// ------------------------------------------------------------

void compileStatements(void) {
    compileStatement();
    while (lookAhead->tokenType == SB_SEMICOLON) {
        eat(SB_SEMICOLON);
        compileStatement();
    }
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
            error(ERR_INVALID_STATEMENT, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

// ------------------------------------------------------------

Type* compileLValue(void) {
    // TODO*: parse a lvalue (a variable, an array element, a parameter, the current function identifier)
    Object* var;
    Type* varType;

    eat(TK_IDENT);
    // check if the identifier is a function identifier, or a variable identifier, or a parameter
    var = checkDeclaredLValueIdent(currentToken->string);
    if (var->kind == OBJ_VARIABLE) {
        varType = var->varAttrs->type;
        if (varType->typeClass == TP_ARRAY)
            varType = compileIndexes(varType);
    } else if (var->kind == OBJ_FUNCTION)
        varType = var->funcAttrs->returnType;
    else
        varType = var->paramAttrs->type;

    return varType;
}

void compileAssignSt(void) {
    Type* lvalueType = compileLValue();
    switch (lookAhead->tokenType) {
        case SB_ASSIGN:
            eat(SB_ASSIGN);
            break;
        case SB_ASSIGN_PLUS:
            eat(SB_ASSIGN_PLUS);
            break;
        case SB_ASSIGN_MINUS:
            eat(SB_ASSIGN_MINUS);
            break;
        case SB_ASSIGN_TIMES:
            eat(SB_ASSIGN_TIMES);
            break;
        case SB_ASSIGN_SLASH:
            eat(SB_ASSIGN_SLASH);
            break;
        default:
            break;
    }
    Type* expressionType = compileExpression();
    checkTypeEquality(lvalueType, expressionType);
}

void compileCallSt(void) {
    Object* proc;

    eat(KW_CALL);
    eat(TK_IDENT);

    proc = checkDeclaredProcedure(currentToken->string);

    // if (proc->procAttrs->paramList != NULL)
    compileArguments(proc->procAttrs->paramList);
}

void compileGroupSt(void) {
    eat(KW_BEGIN);
    compileStatements();
    eat(KW_END);
}

void compileIfSt(void) {
    eat(KW_IF);
    compileCondition();
    eat(KW_THEN);
    compileStatement();
    if (lookAhead->tokenType == KW_ELSE)
        compileElseSt();
}

void compileElseSt(void) {
    eat(KW_ELSE);
    compileStatement();
}

void compileWhileSt(void) {
    eat(KW_WHILE);
    compileCondition();
    eat(KW_DO);
    compileStatement();
}

void compileForSt(void) {
    // TODO*: Check type consistency of FOR's variable
    eat(KW_FOR);
    eat(TK_IDENT);

    // check if the identifier is a variable
    Object* var = checkDeclaredVariable(currentToken->string);
    checkBasicType(var->varAttrs->type);

    eat(SB_ASSIGN);
    Type* expressionType = compileExpression();
    checkBasicType(expressionType);

    eat(KW_TO);
    Type* expressionType2 = compileExpression();
    checkBasicType(expressionType2);

    // Compare types equality
    checkTypeEquality(var->varAttrs->type, expressionType);
    checkTypeEquality(expressionType, expressionType2);

    eat(KW_DO);
    compileStatement();
}

// ------------------------------------------------------------

void compileArgument(Object* param) {
    // TODO*: parse an argument, and check type consistency
    //       If the corresponding parameter is a reference, the argument must be a lvalue

    if (param->paramAttrs->kind == PARAM_REFERENCE) {
        if (lookAhead->tokenType == TK_IDENT) {
            checkDeclaredLValueIdent(lookAhead->string);
        } else {
            error(ERR_TYPE_INCONSISTENCY, lookAhead->lineNo, lookAhead->colNo);
        }
    }

    Type* argType = compileExpression();
    checkTypeEquality(argType, param->paramAttrs->type);
}

void compileArguments(ObjectNode* paramList) {
    //TODO*: parse a list of arguments, check the consistency of the arguments and the given parameters
    switch (lookAhead->tokenType) {
        case SB_LPAR:
            if (paramList == NULL) {
                error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
            }

            eat(SB_LPAR);
            compileArgument(paramList->object);

            while (lookAhead->tokenType == SB_COMMA) {
                eat(SB_COMMA);
                paramList = paramList->next;
                if (paramList != NULL)
                    compileArgument(paramList->object);
                else
                    error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
            }

            // error when no COMMA found and paramList.size > 0
            if (paramList->next != NULL)
                error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);

            eat(SB_RPAR);
            break;

        // Check FOLLOW set
        case SB_TIMES:
        case SB_SLASH:
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
            // paramList.size > 0 and no SB_LPAR found
            if (paramList)
                error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
            break;
        default:
            error(ERR_INVALID_ARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    }
}

// ------------------------------------------------------------

void compileCondition(void) {
    // TODO*: check the type consistency of LHS and RSH, check the basic type
    Type* expressionType = compileExpression();
    checkBasicType(expressionType);

    switch (lookAhead->tokenType) {
        case SB_EQ:
            eat(SB_EQ);
            break;
        case SB_NEQ:
            eat(SB_NEQ);
            break;
        case SB_LE:
            eat(SB_LE);
            break;
        case SB_LT:
            eat(SB_LT);
            break;
        case SB_GE:
            eat(SB_GE);
            break;
        case SB_GT:
            eat(SB_GT);
            break;
        default:
            error(ERR_INVALID_COMPARATOR, lookAhead->lineNo, lookAhead->colNo);
    }

    Type* expressionType2 = compileExpression();
    checkBasicType(expressionType2);

    checkTypeEquality(expressionType, expressionType2);
}

// ------------------------------------------------------------

Type* compileExpression(void) {
    Type* type;

    switch (lookAhead->tokenType) {
        case SB_PLUS:
            eat(SB_PLUS);
            type = compileExpression2();
            checkNumberType(type);
            break;
        case SB_MINUS:
            eat(SB_MINUS);
            type = compileExpression2();
            checkNumberType(type);
            break;
        default:
            type = compileExpression2();
    }
    return type;
}

Type* compileExpression2(void) {
    Type* type1;
    Type* type2;

    type1 = compileTerm();
    type2 = compileExpression3();
    if (type2 == NULL)
        return type1;
    else {
        checkTypeEquality(type1, type2);
        return type1;
    }
}

Type* compileExpression3(void) {
    Type* type1;
    Type* type2;

    switch (lookAhead->tokenType) {
        case SB_PLUS:
            eat(SB_PLUS);
            type1 = compileTerm();
            checkNumberType(type1);
            type2 = compileExpression3();
            if (type2 != NULL)
                checkNumberType(type2);
            return type1;
            break;
        case SB_MINUS:
            eat(SB_MINUS);
            type1 = compileTerm();
            checkNumberType(type1);
            type2 = compileExpression3();
            if (type2 != NULL)
                checkNumberType(type2);
            return type1;
            break;
        case SB_MOD:
            eat(SB_MOD);
            type1 = compileTerm();
            checkIntType(type1);
            type2 = compileExpression3();
            if (type2 != NULL)
                checkIntType(type2);
            return type1;
            break;
            // check the FOLLOW set
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
            return NULL;
            break;
        default:
            error(ERR_INVALID_EXPRESSION, lookAhead->lineNo, lookAhead->colNo);
    }
    return NULL;
}

// ------------------------------------------------------------

Type* compileTerm(void) {
    // TODO: check type of Term2
    Type* type;

    type = compileFactor();
    compileTerm2();

    return type;
}

void compileTerm2(void) {
    // TODO: check type of term2
    Type* type;

    switch (lookAhead->tokenType) {
        case SB_TIMES:
            eat(SB_TIMES);
            type = compileFactor();
            checkNumberType(type);
            compileTerm2();
            break;
        case SB_SLASH:
            eat(SB_SLASH);
            type = compileFactor();
            checkNumberType(type);
            compileTerm2();
            break;
        case SB_MOD:
            eat(SB_MOD);
            type = compileFactor();
            checkIntType(type);
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
            error(ERR_INVALID_TERM, lookAhead->lineNo, lookAhead->colNo);
    }
}

Type* compileFactor(void) {
    // TODO*: parse a factor and return the factor's type

    Object* obj;
    Type* type;

    switch (lookAhead->tokenType) {
        case TK_NUMBER:
            eat(TK_NUMBER);
            type = makeIntType();
            break;
        case TK_FLOAT:
            eat(TK_FLOAT);
            if (lookAhead->tokenType == SB_MOD)
                error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
            type = makeFloatType();
            break;
        case TK_CHAR:
            eat(TK_CHAR);
            type = makeCharType();
            break;
        case TK_IDENT:
            eat(TK_IDENT);
            // check if the identifier is declared
            obj = checkDeclaredIdent(currentToken->string);

            switch (obj->kind) {
                case OBJ_CONSTANT:
                    if (obj->constAttrs->value->type == TP_INT) {
                        type = makeIntType();
                    } else if (obj->constAttrs->value->type == TP_FLOAT) {
                        if (lookAhead->tokenType == SB_MOD)
                            error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
                        type = makeFloatType();
                    } else if (obj->constAttrs->value->type == TP_CHAR) {
                        type = makeCharType();
                    }
                    break;
                case OBJ_VARIABLE:
                    if (obj->varAttrs->type->typeClass == TP_ARRAY) {
                        type = compileIndexes(obj->varAttrs->type);
                    } else if (obj->varAttrs->type->typeClass == TP_FLOAT && lookAhead->tokenType == SB_MOD) {
                        error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
                    } else {
                        type = obj->varAttrs->type;
                    }
                    break;
                case OBJ_PARAMETER:
                    type = obj->paramAttrs->type;
                    break;
                case OBJ_FUNCTION:
                    type = obj->funcAttrs->returnType;
                    // if (obj->funcAttrs->paramList != NULL)
                    compileArguments(obj->funcAttrs->paramList);
                    break;
                default:
                    error(ERR_INVALID_FACTOR, currentToken->lineNo, currentToken->colNo);
                    break;
            }
            break;
        default:
            error(ERR_INVALID_FACTOR, lookAhead->lineNo, lookAhead->colNo);
    }
    return type;
}

Type* compileIndexes(Type* arrayType) {
    // TODO*: parse a sequence of indexes, check the consistency to the arrayType, and return the element type
    while (lookAhead->tokenType == SB_LSEL) {
        checkArrayType(arrayType);
        arrayType = arrayType->elementType;
        eat(SB_LSEL);
        Type* type = compileExpression();
        checkIntType(type);
        eat(SB_RSEL);
    }

    return arrayType;
}

// ------------------------------------------------------------

int compile(char* fileName) {
    if (openInputStream(fileName) == IO_ERROR)
        return IO_ERROR;

    currentToken = NULL;
    lookAhead = getValidToken();

    initSymTab();

    compileProgram();

    printObject(symtab->program, 0);

    cleanSymTab();

    free(currentToken);
    free(lookAhead);
    closeInputStream();
    return IO_SUCCESS;
}
