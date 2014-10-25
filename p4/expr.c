#include "cmanifs.h"
#include "cglobals.h"


EXPRPTR
filenode(filename,first_line,last_line,cursor_position)
char *filename;
int first_line,last_line,cursor_position;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(EXPR4));
	p->f = F_FILE;
	p->arg1.s = strsave(filename);
	p->arg2.i = first_line;
	p->arg3.i = last_line;
	p->arg4.i = cursor_position;
	return(p);
}

EXPRPTR
connode(s1,s2)
char *s1, *s2;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(EXPR2));
	p->f = F_CONST;
	p->arg1.s = strsave(s1);
	p->arg2.s = strsave(s2);
	return(p);
}

EXPRPTR
f_connode(n)
float n;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(EXPR2));
	p->f =      F_CONST;
	p->arg1.s = strsave("numb");
	p->arg2.r = (float) n;
	return(p);
}

EXPRPTR
varnode(name,argcount,exprlist)
char *name;
int argcount;
EXPRPTR exprlist;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR3));
	p->f =      F_VAR;
	p->arg1.s = strsave(name);
	p->arg2.i = argcount;
	p->arg3.x = exprlist;
	return(p);
}

EXPRPTR
opnode(name,argcount,exprlist,file)
char *name;
int argcount;
EXPRPTR exprlist,file;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR4));
	p->f =      F_OP;
	p->arg1.s = strsave(name);
	p->arg2.i = argcount;
	p->arg3.x = exprlist;
	p->arg4.x = file;
	return(p);
}

EXPRPTR
eglobnode(namelist)
EXPRPTR namelist;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR));
	p->f =      F_EGLOBALS;
	p->arg1.x = namelist;
	return(p);
}

EXPRPTR
nglobnode(namelist)
EXPRPTR namelist;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR));
	p->f =      F_NGLOBALS;
	p->arg1.x = namelist;
	return(p);
}

EXPRPTR
valofnode(exprlist)
EXPRPTR exprlist;
{
	STRING calloc();
	EXPRPTR p,listnode();
	p = (EXPRPTR) calloc(1,sizeof(EXPR));
	p->f =      F_VALOF;
	p->arg1.x = exprlist;
	return(p);
}

EXPRPTR
evalofnode(exprlist)
EXPRPTR exprlist;
{
	STRING calloc();
	EXPRPTR p,listnode();
	p = (EXPRPTR) calloc(1,sizeof(EXPR));
	p->f =      F_EVALOF;
	p->arg1.x = exprlist;
	return(p);
}

EXPRPTR
wherenode(expr,exprlist)
EXPRPTR expr;
EXPRPTR exprlist;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR2));
	p->f =      F_WHERE;
	p->arg1.x = expr;
	p->arg2.x = exprlist;
	return(p);
}

EXPRPTR
defnode(name,argcount,argnames,expr)
char *name;
int argcount;
EXPRPTR argnames,expr;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR4));
	p->f =      F_DEFN;
	p->arg1.s = strsave(name);
	p->arg2.i = argcount;
	p->arg3.x = argnames;
	p->arg4.x = expr;
	return(p);
}

EXPRPTR
declnode(name,expr)
char *name;
EXPRPTR expr;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR2));
	p->f =      F_DECL;
	p->arg1.s = strsave(name);
	p->arg2.x = expr;
	return(p);
}


EXPRPTR
identlistnode(tail,name)
EXPRPTR tail;
char *name;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR2));
	p->f =      F_IDENTLISTNODE;
	p->arg1.x = tail;
	p->arg2.s = name;
	return(p);
}

EXPRPTR
listnode(tail,expr)
EXPRPTR expr,tail;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR2));
	p->f =      F_LISTNODE;
	p->arg1.x = tail;
	p->arg2.x = expr;
	return(p);
}

EXPRPTR
exprlist2(expr1,expr2)
EXPRPTR expr1,expr2;
{
	STRING calloc();
	EXPRPTR p1,p2;
	p1 = (EXPRPTR) calloc(1,sizeof(EXPR2));
	p2 = (EXPRPTR) calloc(1,sizeof(EXPR2));
	p1->f =      F_LISTNODE;
	p2->f =      F_LISTNODE;
	p1->arg1.x = NULL;
	p2->arg1.x = p1;
	p1->arg2.x = expr1;
	p2->arg2.x = expr2;
	return(p2);
}

yyerror(a)
STRING a;
{ fprintf(stderr,"%s\n",a); }
