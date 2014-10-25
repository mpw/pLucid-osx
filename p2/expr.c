#include "cmanifs.h"
#include "cglobals.h"

void nameclashes(EXPRPTR arg);

EXPRPTR
pass2(e)
EXPRPTR e;
{    noclashes = true;
     nameclashes(e);
     if (noclashes) output(e); }

void nameclashes(arg)
EXPRPTR arg;
{  return;  }
  /*EXPRPTR file;
  switch(arg->f){
  case F_OP:     nameclashes(arg->arg3.x);
		 return;
  case F_VAR: add_used(arg->arg1.s,arg->arg4.x,0);
	      if (arg->arg2.i>0) nameclashes(arg->arg3.x);
	      return;
  case F_DEFN: add_defined(arg->arg1.s,arg->arg5.x);
	       if (arg->arg2.i>0) {
				     add_formals(arg->arg3.x,arg->arg5.x); }
	       nameclashes(arg->arg4.x);
	       if (arg->arg2.i>0) { remove_formals();
				     }
	       return;
  case F_DECL:  add_defined(arg->arg1.s,arg->arg3.x);
		nameclashes(arg->arg2.x);
		return;
  case F_LISTNODE:  if (arg->arg1.x==NULL) { nameclashes(arg->arg2.x);
					     return; }
		    nameclashes(arg->arg1.x);
		    nameclashes(arg->arg2.x);
		    return;
  case F_CONST:   return;
  case F_WHERE:
		  formals_list[valoflevel]=NULL;
		  used_list[valoflevel]=NULL;
		  nameclashes(arg->arg1.x);
		  valoflevel--;
		  return;
  default:        fprintf(stderr,"unknown parsetree node in nameclashes()\n");
		  return;
  }
}

nameclash_valof(type,e)
int type;
EXPRPTR e;
{  if (valoflevel==1) return;
   tmp = used_list[valoflevel];
   new_decls[valoflevel]=NULL;
   if (tmp!=NULL) {
   if (!is_defined(tmp->arg2.s))
       new_decls[valoflevel]=identlistnode(new_decls[valoflevel],tmp->arg2.s);
   while(tmp->arg1.x!=NULL) {
       tmp=tmp->arg1.x;
       if (!is_defined(tmp->arg2.s))
       new_decls[valoflevel]=identlistnode(new_decls[valoflevel],tmp->arg2.s);
       }
   }
   if (new_decls[valoflevel]!=NULL)
   { tmp = append(listnode(NULL,nglobnode(new_decls[valoflevel])),e);}    else tmp=e;
   if (new_decls[valoflevel]!=NULL) export_nglobals();
   new_decls[valoflevel]=NULL;
   if (type == F_EVALOF)
   return(evalofnode(tmp));
   return(valofnode(tmp));
}

export_nglobals()
{  valoflevel--;
   add_used_list(new_decls[valoflevel+1]);
   valoflevel++;
}

add_used_list(e)
EXPRPTR e;
{ EXPRPTR tmp;
  tmp = e;
  if ( tmp==NULL) return;
  add_used(tmp->arg2.s);
  while (tmp->arg1.x!=NULL){
			     tmp=tmp->arg1.x;
			     add_used(tmp->arg2.s);
			   }
}

char
is_defined(s)
char *s;
{ EXPRPTR tmp;
  tmp = defined_list[valoflevel];
  if (tmp==NULL) return(false);
  if (eqstring(s,tmp->arg2.s)) return(true);
  while ( tmp->arg1.x!=NULL ) {
			  tmp = tmp -> arg1.x;
			  if (eqstring(s,tmp->arg2.s)) return(true);
	      }
  return(false);
}

add_defined_list(e,file)
EXPRPTR e,file;
{ EXPRPTR tmp;
  tmp = e;
  if ( tmp==NULL) return;
  add_defined(tmp->arg2.s);
  while (tmp->arg1.x!=NULL){
			     tmp=tmp->arg1.x;
			     add_defined(tmp->arg2.s);
    }
}

remove_formals()
{ formals_list[valoflevel]=NULL ; }

add_formals(e,file)
EXPRPTR e,file;
{
  formals_list[valoflevel]=e; }


add_used(s,file)
char *s;
{ EXPRPTR tmp,identlistnode();
  tmp = formals_list[valoflevel];
  if ( tmp!=NULL) {
  if (eqstring(s,tmp->arg2.s)) return;
  while (tmp->arg1.x!=NULL) {
	      tmp = tmp->arg1.x;
	      if (eqstring(s,tmp->arg2.s)) return;
	  }
  }
  tmp = used_list[valoflevel];
  if (tmp==NULL) { used_list[valoflevel]=identlistnode(NULL,s);
		   return; }
  if (eqstring(s,tmp->arg2.s)) return;
  while (tmp->arg1.x!=NULL) {
	      tmp = tmp->arg1.x;
	      if (eqstring(s,tmp->arg2.s)) return;
	   }
  used_list[valoflevel]=identlistnode(used_list[valoflevel],s);
}

add_defined(s)
char *s;
{ EXPRPTR tmp,identlistnode();
  tmp = defined_list[valoflevel];
  if (tmp==NULL) { defined_list[valoflevel]=identlistnode(NULL,s);
		   return;}
  if (eqstring(s,tmp->arg2.s)) return;
  while (tmp->arg1.x!=NULL) {
	      tmp = tmp->arg1.x;
	      if (eqstring(s,tmp->arg2.s)) return;
     }
  defined_list[valoflevel]=identlistnode(defined_list[valoflevel],s);
}              */

EXPRPTR
filenode(filename,first_line,last_line,cursor_position)
char *filename;
int first_line,last_line,cursor_position;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(EXPR4));
	p->f = F_FILE;
	p->arg1.s = filename;
	p->arg2.i = first_line;
	p->arg3.i = last_line;
	p->arg4.i = cursor_position;
	return(p);
}

EXPRPTR
connode(s1,s2)
char *s1, *s2;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(EXPR2));
	p->f = F_CONST;
	p->arg1.s = s1;
	p->arg2.s = s2;
	return(p);
}

EXPRPTR
f_connode(n)
float n;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1, sizeof(EXPR2));
	p->f =      F_CONST;
	p->arg1.s = "numb";
	p->arg2.r = (float) n;
	return(p);
}

EXPRPTR
varnode(name,argcount,exprlist,file)
char *name;
int argcount;
EXPRPTR exprlist,file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR4));
	p->f =      F_VAR;
	p->arg1.s = name;
	p->arg2.i = argcount;
	p->arg3.x = exprlist;
	p->arg4.x = file;
	return(p);
}

EXPRPTR
opnode(name,argcount,exprlist,file)
char *name;
int argcount;
EXPRPTR exprlist,file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR4));
	p->f =      F_OP;
	p->arg1.s = name;
	p->arg2.i = argcount;
	p->arg3.x = exprlist;
	p->arg4.x = file;
	return(p);
}

EXPRPTR
wherenode(expr,exprlist,file)
EXPRPTR expr;
EXPRPTR exprlist,file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR3));
	p->f =      F_WHERE;
	p->arg1.x = expr;
	p->arg2.x = exprlist;
	p->arg3.x = file;
	return(p);
}

EXPRPTR
defnode(name,argcount,argnames,expr,file)
char *name;
int argcount;
EXPRPTR argnames,expr,file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR5));
	p->f =      F_DEFN;
	p->arg1.s = name;
	p->arg2.i = argcount;
	p->arg3.x = argnames;
	p->arg4.x = expr;
	p->arg5.x = file;
	return(p);
}

EXPRPTR
declnode(name,expr,file)
char *name;
EXPRPTR expr,file;
{
	STRING calloc();
	EXPRPTR p;
	p = (EXPRPTR) calloc(1,sizeof(EXPR3));
	p->f =      F_DECL;
	p->arg1.s = name;
	p->arg2.x = expr;
	p->arg3.x = file;
	return(p);
}


EXPRPTR
identlistnode(tail,name)
EXPRPTR tail;
char *name;
{
	STRING calloc();
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
	p1->arg1.x = expr1;
	p2->arg1.x = p1;
	p1->arg2.x = p2;
	p1->arg2.x = NULL;
	p2->arg2.x = expr2;
	return(p2);
}

yyerror(a)
STRING a;
{ fprintf(stderr,"%s\n",a); }
