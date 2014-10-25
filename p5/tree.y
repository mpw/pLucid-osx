

%term   CONST WRD STRG SWCHAR
        WHERE IDENT STRING_QUOTED
        NUMB NULLRY NONNULLRY OP DEFN DECL CXFILE
        NMB SPL FRMLS
        VALOF NGLOB EGLOB OPNAME
	INTERIOR TERMINAL
%{
#include "cmanifs.h"
#include "cglobals.h"
EXPRPTR varnode();
EXPRPTR sconsnode(),connode(),f_connode(),filenode();
EXPRPTR binnode(),unode(),ternode();
EXPRPTR exit_phrase();
%}
%%

accept:
        expr
        { $$.eptr = $1.eptr; }
;

constant: '[' CONST '[' STRG STRING_QUOTED ']'  ']'
          { $$.eptr =
sconsnode(wordtable[$5.value],filenode(findword("error"),1,1,1)); }
|         '[' CONST '[' NMB NUMB ']' ']'
          { $$.eptr = f_connode($5.numb); }
|         '[' CONST '[' WRD STRING_QUOTED ']' ']'
          { $$.eptr = connode(F_WORD,$5.value); }
|         '[' CONST '[' SPL ']'  ']'
          { $$.eptr = unode($4.value,0,NULL); }
;

file: '[' CXFILE STRING_QUOTED NUMB NUMB NUMB ']'
       { $$.eptr
=filenode($3.value,(int)$4.numb,(int)$5.numb,(int)$6.numb); }
;

variable: '[' NONNULLRY IDENT NUMB { pushexpr(); } exprlist ']'
         { $$.eptr = varnode(NONNULLARY,$3.ident); }
|         '[' NULLRY IDENT NUMB ']'
         { $$.eptr = varnode(NULLARY,$3.ident); }
;

operator: '[' OP '[' INTERIOR NUMB expr ']' file ']'
          {  $$.eptr=unode($4.value,(int)$5.numb,$6.eptr,$8.eptr); }
|         '[' OP '[' INTERIOR NUMB expr expr ']' file ']'
          {  $$.eptr=binnode($4.value,(int)$5.numb,$6.eptr,$7.eptr,$9.eptr);}
|         '[' OP '[' INTERIOR NUMB expr expr expr ']' file ']'
          { $$.eptr=ternode($4.value,(int)$5.numb,$6.eptr,
                                         $7.eptr,$8.eptr,$10.eptr); }
;

valof: '[' VALOF { enter_phrase(); } valofbody ']'
        { $$.eptr = exit_phrase(); }
;

valofbody:
        statement
|       valofbody  statement
;

formals: '[' FRMLS { enter_function(); } argnames
               { idusage = (int) U_NORMAL; } ']'
;

statement: '[' DEFN IDENT NUMB formals expr ']'
            {  define($3.ident,$6.eptr);
               exit_function();
            }
|           '[' DEFN IDENT NUMB expr ']'
            {  define($3.ident,$5.eptr); }
|           '[' { idusage = (int) U_NGLOBAL; }
                NGLOB argnames ']'
             { idusage = (int) U_NORMAL; }
|           '[' { idusage = (int) U_EGLOBAL;
                        if (p_stack->p_iselem==0){
                            p_stack->p_iselem=1;
                            elemlevel++; }
                      }
                EGLOB argnames ']'
             { idusage = (int) U_NORMAL; }
;

argnames:   IDENT
|           argnames IDENT
;

expr:      valof { $$.eptr = $1.eptr; }
|          constant { $$.eptr = $1.eptr; }
|          variable {   $$.eptr = $1.eptr; }
|          operator { $$.eptr = $1.eptr; }
;

exprlist:  expr
            { appeexpr($1.eptr); }
|          exprlist  expr
            { appeexpr($2.eptr); }
;
