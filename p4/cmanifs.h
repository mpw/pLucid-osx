#include <stdio.h>
#include <ctype.h>
#define YYSTYPE union stacktype
#include "y.tab.h"

#define cycle   for(;;)
#define NL      '\n'
#define setodd  1|
#define EMPTYSTRING '\0'
#define YYVCOPY(x,y) copy(  y , sizeof(union stacktype) , 1 ,  x )

#define F_CONST 1
#define F_VAR   2
#define F_OP    3
#define F_WHERE 4
#define F_DEFN  5
#define F_DECL  6
#define F_IDENTLISTNODE 7
#define F_LISTNODE 8
#define F_FILE 9
#define F_VALOF 10
#define F_EGLOBALS 11
#define F_NGLOBALS 12
#define F_EVALOF 13

typedef struct EXPR EXPR, *EXPRPTR;

typedef union X_OR_I {
	int i;
	float r;
	char *s;
	EXPRPTR x;
		      } X_OR_I;
typedef struct EXPR2 {
	int f;
	X_OR_I arg1, arg2;
		      } EXPR2;

typedef struct EXPR3 {
	int f;
	X_OR_I arg1,arg2,arg3;
		      } EXPR3;

typedef struct EXPR4 {
	int f;
	X_OR_I arg1,arg2,arg3,arg4;
		      } EXPR4;

typedef struct EXPR5 {
	int f;
	X_OR_I arg1,arg2,arg3,arg4,arg5;
		      } EXPR5;

struct EXPR {
	int f;
	X_OR_I arg1, arg2, arg3, arg4, arg5;
	     } ;

typedef char *STRING;
typedef struct FFTYPE     FFITEM,   *FFPTR;

struct FFTYPE {
	STRING name;
	EXPRPTR rhs;
	FFPTR next_d;
	};


union stacktype{

	/* for numbers returned by yylex */
	float numb;

	/* for symbol table entries returned by yylex */
	char *strg;

	/* for expressions pointers returned by yacc actions */
	EXPRPTR eptr;
};
