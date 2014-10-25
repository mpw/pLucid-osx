#include <stdio.h>
#include <ctype.h>

#define YYSTYPE union stacktype

#include "y.tab.h"

#define NKEYWORDS         162
#define BUFFERLENGTH      200
#define NESTINGDEPTH       50
#define NOOFFILES          12
#define F_CONST             1
#define F_VAR               2
#define F_OP                3
#define F_WHERE             4
#define F_DEFN              5
#define F_DECL              6
#define F_IDENTLISTNODE     7
#define F_EXPRLISTNODE      8
#define F_LISTNODE          9
#define F_BODY             10
#define F_FILE             11


typedef struct RES  RES,  *RESPTR;
typedef struct EXPR EXPR, *EXPRPTR;
typedef char              *STRING;

typedef union X_OR_I {
		       long      i;
		       double    r;
		       char    *s;
		       EXPRPTR  x;
		     } X_OR_I;

typedef struct EXPR2 {
		       int     f;
		       X_OR_I  arg1, arg2;
		     } EXPR2;

typedef struct EXPR3 {
		       int     f;
		       X_OR_I  arg1,arg2,arg3;
		     } EXPR3;

typedef struct EXPR4 {
		       int     f;
		       X_OR_I  arg1,arg2,arg3,arg4;
		     } EXPR4;

typedef struct EXPR5 {
		       int     f;
		       X_OR_I  arg1,arg2,arg3,arg4,arg5;
		     } EXPR5;

struct EXPR          {
		       int     f;
		       X_OR_I  arg1, arg2, arg3, arg4,arg5;
		     };


struct RES           {
		       int     len;
		       int     line;
		       char   *strg;
		     };

union stacktype      {

		       /* for numbers returned by yylex */
		       float numb;

		       /* for symbol table entries returned by yylex */
		       RESPTR resptr;

		       /* for expressions pointers returned by yacc actions */
		       EXPRPTR eptr;
		     };
