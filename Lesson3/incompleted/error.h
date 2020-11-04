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


#define ERM_ENDOFCOMMENT "End of comment expected!" //ok
#define ERM_IDENTTOOLONG "Identification too long!" //ok
#define ERM_INVALIDCHARCONSTANT "Invalid const char!" //ok
#define ERM_INVALIDSYMBOL "Invalid symbol!" //ok
#define ERM_UNEXPECTEDFLOAT "err: Float number expected!"
#define ERM_INVALIDCONSTANT "Invalid constant!" //ok
#define ERM_INVALIDTYPE "Invalid type!" //ok
#define ERM_INVALIDBASICTYPE "Invalid basic type!"  //ok
#define ERM_INVALIDPARAM "Invalid parameter!"     //ok-
#define ERM_INVALIDSTATEMENT "Invalid statement!" //ok-
#define ERM_INVALIDARGUMENTS "Invalid arguments!" //ok-
#define ERM_INVALIDCOMPARATOR "Invalid comparator!"
#define ERM_INVALIDEXPRESSION "Invalid expression!" //ok-
#define ERM_INVALIDTERM "Invalid term!" //ok-
#define ERM_INVALIDFACTOR "Invalid factor!" //ok
#define ERM_INVALIDCONSTDECL "Invalid constant declaration!"  //ok
#define ERM_INVALIDTYPEDECL "Invalid type declaration!" //ok
#define ERM_INVALIDVARDECL "Invalid variable declaration!"  //ok
#define ERM_INVALIDSUBDECL "Invalid subroutine declaration!"  //ok?

void error(ErrorCode err, int lineNo, int colNo);
void missingToken(TokenType tokenType, int lineNo, int colNo);
void assert(char *msg);

#endif
