/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

// Return error message
void error(ErrorCode err, int lineNo, int colNo) {
    switch (err) {
        case ERR_ENDOFCOMMENT:
            printf("%d-%d:%s\n", lineNo, colNo, ERM_ENDOFCOMMENT);
            break;
        case ERR_IDENTTOOLONG:
            printf("%d-%d:%s\n", lineNo, colNo, ERM_IDENTTOOLONG);
            break;
        case ERR_INVALIDCHARCONSTANT:
            printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDCHARCONSTANT);
            break;
        case ERR_INVALIDSYMBOL:
            printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDSYMBOL);
            break;
        case ERR_UNEXPECTEDFLOAT:
            printf("%d-%d:%s\n", lineNo, colNo, ERM_UNEXPECTEDFLOAT);
            break;
    }
    exit(-1);
}
