/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include "symtab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode* objList);
void freeReferenceList(ObjectNode* objList);

SymTab* symtab;
Type* intType;
Type* charType;
Type* floatType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
    Type* type = (Type*)malloc(sizeof(Type));
    type->typeClass = TP_INT;
    return type;
}

Type* makeCharType(void) {
    Type* type = (Type*)malloc(sizeof(Type));
    type->typeClass = TP_CHAR;
    return type;
}

Type* makeFloatType(void) {
    Type* type = (Type*)malloc(sizeof(Type));
    type->typeClass = TP_FLOAT;
    return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
    Type* type = (Type*)malloc(sizeof(Type));
    type->typeClass = TP_ARRAY;
    type->arraySize = arraySize;
    type->elementType = elementType;
    return type;
}

Type* duplicateType(Type* type) {
    // TODO
    Type* result = (Type*)malloc(sizeof(Type));
    result->typeClass = type->typeClass;
    if (type->typeClass == TP_ARRAY) {
        result->arraySize = type->arraySize;
        result->elementType = duplicateType(type->elementType);
    }
    return result;
}

int compareType(Type* type1, Type* type2) {
    // TODO
    if (type1->typeClass == type2->typeClass) {
        if (type1->typeClass == TP_ARRAY) {
            return type1->arraySize == type2->arraySize ? compareType(type1->elementType, type2->elementType) : 0;
        } else
            return 1;
    } else
        return 0;
}

void freeType(Type* type) {
    // TODO
    switch (type->typeClass) {
        case TP_INT:
        case TP_CHAR:
        case TP_FLOAT:
            free(type);
            break;
        case TP_ARRAY:
            freeType(type->elementType);
            freeType(type);
            break;
    }
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
    // TODO
    ConstantValue* value = (ConstantValue*)malloc(sizeof(ConstantValue));
    value->type = TP_INT;
    value->intValue = i;
    return value;
}

ConstantValue* makeCharConstant(char ch) {
    // TODO
    ConstantValue* value = (ConstantValue*)malloc(sizeof(ConstantValue));
    value->type = TP_CHAR;
    value->charValue = ch;
    return value;
}

ConstantValue* makeFloatConstant(float f) {
    // TODO
    ConstantValue* value = (ConstantValue*)malloc(sizeof(ConstantValue));
    value->type = TP_FLOAT;
    value->floatValue = f;
    return value;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
    // TODO
    ConstantValue* resultValue = (ConstantValue*)malloc(sizeof(ConstantValue));
    resultValue->type = v->type;  //no pointer -> no need duplicate type

    if (v->type == TP_INT)
        resultValue->intValue = v->intValue;
    else if (v->type == TP_CHAR)
        resultValue->charValue = v->charValue;
    else if (v->type == TP_FLOAT)
        resultValue->floatValue = v->floatValue;

    return resultValue;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    scope->objList = NULL;
    scope->owner = owner;
    scope->outer = outer;
    return scope;
}

Object* createProgramObject(char* programName) {
    Object* program = (Object*)malloc(sizeof(Object));
    strcpy(program->name, programName);
    program->kind = OBJ_PROGRAM;
    program->progAttrs = (ProgramAttributes*)malloc(sizeof(ProgramAttributes));
    program->progAttrs->scope = createScope(program, NULL);
    symtab->program = program;

    return program;
}

Object* createConstantObject(char* name) {
    // TODO
    Object* obj = (Object*)malloc(sizeof(Object));
    strcpy(obj->name, name);
    obj->kind = OBJ_CONSTANT;
    obj->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
    return obj;
}

Object* createTypeObject(char* name) {
    // TODO
    Object* obj = (Object*)malloc(sizeof(Object));
    strcpy(obj->name, name);
    obj->kind = OBJ_TYPE;
    obj->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
    return obj;
}

Object* createVariableObject(char* name) {
    // TODO
    Object* obj = (Object*)malloc(sizeof(Object));
    strcpy(obj->name, name);
    obj->kind = OBJ_VARIABLE;
    obj->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
    obj->varAttrs->scope = symtab->currentScope;
    return obj;
}

Object* createFunctionObject(char* name) {  //new scope
    // TODO
    Object* obj = (Object*)malloc(sizeof(Object));
    strcpy(obj->name, name);
    obj->kind = OBJ_FUNCTION;
    obj->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
    obj->funcAttrs->paramList = NULL;
    obj->funcAttrs->scope = createScope(obj, symtab->currentScope);
    return obj;
}

Object* createProcedureObject(char* name) {
    // TODO
    Object* obj = (Object*)malloc(sizeof(Object));
    strcpy(obj->name, name);
    obj->kind = OBJ_PROCEDURE;
    obj->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
    obj->procAttrs->paramList = NULL;
    obj->procAttrs->scope = createScope(obj, symtab->currentScope);
    return obj;
}

Object* createParameterObject(char* name, enum ParamKind kind, Object* owner) {
    // TODO
    Object* obj = (Object*)malloc(sizeof(Object));
    strcpy(obj->name, name);
    obj->kind = OBJ_PARAMETER;
    obj->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
    obj->paramAttrs->kind = kind;
    obj->paramAttrs->function = owner;
    return obj;
}

void freeObject(Object* obj) {  //further free if pointer
    // TODO
    switch (obj->kind) {
        case OBJ_CONSTANT:
            free(obj->constAttrs->value);
            free(obj->constAttrs);
            break;
        case OBJ_TYPE:
            free(obj->typeAttrs->actualType);
            free(obj->typeAttrs);
            break;
        case OBJ_VARIABLE:
            free(obj->varAttrs->type);
            free(obj->varAttrs);
            break;
        case OBJ_FUNCTION:
            freeReferenceList(obj->funcAttrs->paramList);
            freeType(obj->funcAttrs->returnType);
            freeScope(obj->funcAttrs->scope);
            free(obj->funcAttrs);
            break;
        case OBJ_PROCEDURE:
            freeReferenceList(obj->procAttrs->paramList);
            freeScope(obj->procAttrs->scope);
            free(obj->procAttrs);
            break;
        case OBJ_PROGRAM:
            freeScope(obj->progAttrs->scope);
            free(obj->progAttrs);
            break;
        case OBJ_PARAMETER:
            freeType(obj->paramAttrs->type);
            free(obj->paramAttrs);
    }
    free(obj);
}

void freeScope(Scope* scope) {
    // TODO
    freeObjectList(scope->objList);
    free(scope);
}

void freeObjectList(ObjectNode* objList) {
    // TODO
    ObjectNode* ptr = objList;
    while (ptr != NULL) {
        ObjectNode* node = ptr;
        ptr = ptr->next;
        freeObject(node->object);
        free(node);
    }
}

void freeReferenceList(ObjectNode* objList) {
    // TODO
    ObjectNode* list = objList;

    while (list != NULL) {
        ObjectNode* node = list;
        list = list->next;
        free(node);
    }
}

void addObject(ObjectNode** objList, Object* obj) {
    ObjectNode* node = (ObjectNode*)malloc(sizeof(ObjectNode));
    node->object = obj;
    node->next = NULL;
    if ((*objList) == NULL)
        *objList = node;
    else {
        ObjectNode* n = *objList;
        while (n->next != NULL)
            n = n->next;
        n->next = node;
    }
}

Object* findObject(ObjectNode* objList, char* name) {
    // TODO
    while (objList != NULL) {
        if (strcmp(objList->object->name, name) == 0)
            return objList->object;
        else
            objList = objList->next;
    }
    return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
    Object* obj;
    Object* param;

    symtab = (SymTab*)malloc(sizeof(SymTab));
    symtab->globalObjectList = NULL;

    obj = createFunctionObject("READC");
    obj->funcAttrs->returnType = makeCharType();
    addObject(&(symtab->globalObjectList), obj);

    obj = createFunctionObject("READI");
    obj->funcAttrs->returnType = makeIntType();
    addObject(&(symtab->globalObjectList), obj);

    obj = createFunctionObject("READF");
    obj->funcAttrs->returnType = makeFloatType();
    addObject(&(symtab->globalObjectList), obj);

    obj = createProcedureObject("WRITEI");
    param = createParameterObject("i", PARAM_VALUE, obj);
    param->paramAttrs->type = makeIntType();
    addObject(&(obj->procAttrs->paramList), param);
    addObject(&(symtab->globalObjectList), obj);

    obj = createProcedureObject("WRITEC");
    param = createParameterObject("ch", PARAM_VALUE, obj);
    param->paramAttrs->type = makeCharType();
    addObject(&(obj->procAttrs->paramList), param);
    addObject(&(symtab->globalObjectList), obj);

    obj = createProcedureObject("WRITEF");
    param = createParameterObject("f", PARAM_VALUE, obj);
    param->paramAttrs->type = makeFloatType();
    addObject(&(obj->procAttrs->paramList), param);
    addObject(&(symtab->globalObjectList), obj);

    obj = createProcedureObject("WRITELN");
    addObject(&(symtab->globalObjectList), obj);

    intType = makeIntType();
    charType = makeCharType();
    floatType = makeFloatType();
}

void cleanSymTab(void) {
    freeObject(symtab->program);
    freeObjectList(symtab->globalObjectList);
    free(symtab);
    freeType(intType);
    freeType(charType);
    freeType(floatType);
}

void enterBlock(Scope* scope) {
    symtab->currentScope = scope;
}

void exitBlock(void) {
    symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
    if (obj->kind == OBJ_PARAMETER) {
        Object* owner = symtab->currentScope->owner;
        switch (owner->kind) {
            case OBJ_FUNCTION:
                addObject(&(owner->funcAttrs->paramList), obj);
                break;
            case OBJ_PROCEDURE:
                addObject(&(owner->procAttrs->paramList), obj);
                break;
            default:
                break;
        }
    }

    addObject(&(symtab->currentScope->objList), obj);
}
