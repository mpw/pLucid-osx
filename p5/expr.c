#include "cmanifs.h"
#include "cglobals.h"

findfile(e)
EXPRPTR e;
{
	if (filecount >= MAXFILES)
	   { fprintf(stderr,"too many cxfiles\n"); my_exit(1); }
	filetable[filecount] = e;
	filecount++;
	return(filecount-1);
}

EXPRPTR
filenode(filename,first_line,last_line,cursor_position)
int filename;
int first_line,last_line,cursor_position;
{
	STRING calloc(),strsave();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(Q_EXPR));
	p->dim = 0;
	p->f = (FUNCTION) F_CXFILE;
	p->arg1.i = filename;
	p->arg2.i = first_line;
	p->arg3.i = last_line;
	p->arg4.i = cursor_position;
	return(p);
}

EXPRPTR
sconsnode(s,file)
EXPRPTR file;
char *s;
{ EXPRPTR unode(),binnode(),connode();
  if(s[0] == '\0')       return(binnode(F_SCONS,0,connode(F_SWCHAR,s[0]),unode(F_NIL,0,NULL),file));
  return(binnode(F_SCONS,0,connode(F_SWCHAR,s[0]),
	   sconsnode((char *)&s[1],file),file));
}

EXPRPTR
connode(f,n)
int f,n;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(U_EXPR));
	p->dim = 0;
	p->f = (FUNCTION) f;
	p->arg1.i = (int) n;
	return(p);
}

EXPRPTR
f_connode(n)
float n;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(U_EXPR));
	p->dim = 0;
	p->f = (FUNCTION) F_CONST;
	p->arg1.r = (float) n;
	return(p);
}

EXPRPTR
varnode(type,v)
int type;
SYMPTR v;
{
	STRING calloc();
	EXPRPTR p;
	F_STACKPTR f_env;
	int i, placepop, timepop;

	placepop = funclevel - v->fldef;
	/*
	 * getting the amount of time we have to pop
	 * is trickier if we have inherited the variable
	 * through a function definition. this is because
	 * we dont pop off the time that we added to the
	 * the time stack as we called functions.
	 */
	if(placepop>0){
		f_env = f_stack;
		for(i=1; i< (funclevel-v->fldef); i++){
			f_env = f_env->f_tl;
		}
		/*
		 * f_env->f_el is the elemlevel where the last containing
		 * function was declared.
		 */
		timepop = f_env->f_el - v->elemdef;
	}else{
		timepop = elemlevel - v->elemdef;
	}

	if( type==NULLARY && timepop==0 && placepop==0 ){
		/*
		 * a local nullary variable requires no stack
		 * changes during interpretation, so
		 * compile it separately
		 */
		p = (EXPRPTR) calloc(1, sizeof(U_EXPR));
	p->dim = 0;
		p->f = F_LOCAL;
		p->arg1.i = v->idno;
	}else if( type==NONNULLARY ){
		/*
		 *      a function call
		 */
		p = (EXPRPTR) calloc(1, sizeof(Q_EXPR));
	p->dim = 0;
		p->f = F_FCALL;
		p->arg1.i = v->idno;    /* function name */
		p->arg2.i = IS_ELEM(v) ?(timepop+1) : -(timepop+1);
		p->arg3.i = placepop;
		p->arg4.i = identcount;
		defineparms();
	}else{
		/*
		 *      nullary local variable
		 */
		p = (EXPRPTR) calloc(1, sizeof(T_EXPR));
		p->f = F_VAR;
	p->dim = 0;
		p->arg1.i = IS_FORM(v) ?-v->idno-1 :v->idno;
		p->arg2.i = IS_ELEM(v) ?(timepop+1) :-(timepop+1);
		p->arg3.i = placepop;
	}
	return(p);
}

pushexpr()
{
	STRING calloc();
	E_STACKPTR temp;
	/* push the location of the new expression list onto parm_stack */
	temp = (E_STACKPTR) calloc(1, sizeof(E_STACK));
	temp->es_list = parm_list;
	temp->es_tl = parm_stack;
	parm_stack = temp;
}

appeexpr(e)
EXPRPTR e;
{
	STRING calloc();
	E_LISTPTR new;
	new = (E_LISTPTR) calloc(1,sizeof(E_LIST));
	new->el_expr = e;
	new->el_tl = NULL;
	parm_list->el_tl = new;
	parm_list = new;
}

defineparms()
{
	STRING calloc();
	E_LISTPTR scan, temp;
	int flag;

	/* now put the expressions in the expression table */
	/* deleting them from the list as we go */
	/* at first, temp points to the element in parm_list that is
	/* the last one we wish to keep. we have to set up parm_stack
	/* like this so that we can set the last pointer in
	/* parm_list to NULL
	 */
	flag=1;
	temp = parm_stack->es_list;
	scan = temp->el_tl;
	temp->el_tl = NULL;
	while(scan){
		temp = scan;
		exprtable[identcount] = scan->el_expr;
		nametable[identcount] = "actual";
		if ( flag == 0 ) {
		fprintf(stderr,"defining actual, %d as:",identcount);
		echoexpr(scan->el_expr);
		fprintf(stderr,"\n");
		}
		scan = scan->el_tl;
		free(temp);
		identcount++;
	}
	/* pop the top entry from the stack */
	/* dont forget that parm_list must point to the end
	/* of the new, truncated, list
	 */
	parm_list = parm_stack->es_list;
	temp = (E_LISTPTR) parm_stack;
	parm_stack = parm_stack->es_tl;
	free(temp);
}

EXPRPTR unode(f,n,a,file)
int f,n;
EXPRPTR a;
EXPRPTR file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(B_EXPR));
	p->f = (FUNCTION) f;
	p->dim = n;
	p->arg1.x = a;
	p->arg2.x = file;
	return(p);
}

EXPRPTR
binnode(f,n,a,b,file)
int f,n;
EXPRPTR a, b;
EXPRPTR file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(T_EXPR));
	p->f = (FUNCTION) f;
	p->dim = n;
	p->arg1.x = a;
	p->arg1.x = a;
	p->arg2.x = b;
	p->arg3.x = file;
	return(p);
}

EXPRPTR
ternode(f,n,b,c,d,file)
int f,n;
EXPRPTR b,c,d;
EXPRPTR file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(Q_EXPR));
	p->f =  (FUNCTION) f;
	p->dim = n;
	p->arg1.x = b;
	p->arg2.x = c;
	p->arg3.x = d;
	p->arg4.x = file;
	return(p);
}

define(s,e)
SYMPTR s;
EXPRPTR e;
{
	STRING calloc();
	STRING strsave();
	 int flag;
	 flag=1;
	if(s->attr&DEF_A){
		yyerror(s->name,"is already defined");
	}else if(s->attr&GLOB_A){
		 /*
		  * inheriting and trying to redefine it
		  */
		yyerror(s->name," cannot be defined here");
	}else{
		exprtable[s->idno] = e;
		nametable[s->idno] = strsave(s->name);
		s->attr = s->attr|DEF_A;
		 if ( flag == 0 ) {
fprintf(stderr,"defining %s, level %d idno %d as:",s->name, s->lexlevel, s->idno);
		echoexpr(exprtable[s->idno]);
		fprintf(stderr,"\n");
		}
	}
}

yyerror(a,b)
STRING a,b;
{ fprintf(stderr,"%s%s\n",a,b); }
