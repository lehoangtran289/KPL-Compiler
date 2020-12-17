/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __ERROR_H__
#define __ERROR_H__
#include "token.h"

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
  ERR_UNEXPECTEDFLOAT,
  ERR_INVALIDCONSTANT,
  ERR_INVALIDTYPE,
  ERR_INVALIDBASICTYPE,
  ERR_INVALIDPARAM,
  ERR_INVALIDSTATEMENT,
  ERR_INVALIDARGUMENTS,
  ERR_INVALIDCOMPARATOR,
  ERR_INVALIDEXPRESSION,
  ERR_INVALIDTERM,
  ERR_INVALIDFACTOR,
  ERR_INVALIDCONSTDECL,
  ERR_INVALIDTYPEDECL,
  ERR_INVALIDVARDECL,
  ERR_INVALIDSUBDECL,
} ErrorCode;


#define ERM_ENDOFCOMMENT "End of comment expected!" //(* Factorial
#define ERM_IDENTTOOLONG "Identification too long!" //fasdasdsadasdasdasdsadasdasdads
#define ERM_INVALIDCHARCONSTANT "Invalid const char!" //'aasdads'
#define ERM_INVALIDSYMBOL "Invalid symbol!" //^
#define ERM_INVALIDCONSTANT "Invalid constant!" //Const c = -=;  
#define ERM_INVALIDTYPE "Invalid type!" //Type d = do;
#define ERM_INVALIDBASICTYPE "Invalid basic type!"  //Function F(n : Float) : if;
#define ERM_INVALIDPARAM "Invalid parameter!"     //Function F*(n : Float)
#define ERM_INVALIDSTATEMENT "Invalid statement!" //Begin do a := 1;
#define ERM_INVALIDARGUMENTS "Invalid arguments!" //Call WriteI F(i))
#define ERM_INVALIDCOMPARATOR "Invalid comparator!" //If f * 2 - 0
#define ERM_INVALIDEXPRESSION "Invalid expression!" //ok-
#define ERM_INVALIDTERM "Invalid term!" //If f . 2 = 0
#define ERM_INVALIDFACTOR "Invalid factor!" // if Do % 2 = 0
#define ERM_INVALIDCONSTDECL "Invalid constant declaration!"  //Const * = 10;   
#define ERM_INVALIDTYPEDECL "Invalid type declaration!" //Type *d = Integer;
#define ERM_INVALIDVARDECL "Invalid variable declaration!"  //Var *n : Float;
#define ERM_INVALIDSUBDECL "Invalid subroutine declaration!"  //Program Example2; (* Factorial *) do

void error(ErrorCode err, int lineNo, int colNo);
void missingToken(TokenType tokenType, int lineNo, int colNo);
void assert(char *msg);

#endif
