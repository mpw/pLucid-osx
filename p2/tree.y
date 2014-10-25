

%term
        WHERE WORD STRING_QUOTED
        NUMB VAR OP DEFN DECL CONST ERRFILE
        F_NUMB F_WORD F_STRING F_SPECIAL
        EXPRL STATL IDENTL BODY
%{
#include "cmanifs.h"
#include "cglobals.h"
EXPRPTR opnode(),wherenode(),defnode(),declnode(),varnode();
EXPRPTR connode(),f_connode(),filenode();
EXPRPTR identlistnode(),listnode(),exprlist2();
%}
%%

accept:
        expr
        { $$.eptr = $1.eptr;
          pass2($1.eptr);
        }
;

constant: '[' CONST '[' F_STRING STRING_QUOTED ']' ']'
          { $$.eptr = connode("string",$5.strg); }
|         '[' CONST '[' F_NUMB NUMB ']' ']'
          { $$.eptr = f_connode($5.numb); }
|         '[' CONST '[' F_WORD STRING_QUOTED ']' ']'
          { $$.eptr = connode("word",$5.strg); }
|         '[' CONST '[' F_SPECIAL STRING_QUOTED  ']' ']'
          { $$.eptr = connode("special",$5.strg); }
;

file: '[' ERRFILE STRING_QUOTED NUMB NUMB NUMB ']'
       { $$.eptr =filenode($3.strg,(int)$4.numb,(int)$5.numb,(int)$6.numb); }
;

variable: '[' VAR WORD NUMB exprlist file ']'
         { $$.eptr = varnode($3.strg,(int)$4.numb,$5.eptr,$6.eptr); }
|         '[' VAR WORD NUMB file ']'
         { $$.eptr = varnode($3.strg,(int)$4.numb,NULL,$5.eptr); }
;

operator: '[' OP WORD NUMB exprlist file ']'
          { $$.eptr = opnode($3.strg,(int)$4.numb,$5.eptr,$6.eptr); }
;

where: '[' WHERE expr wherebody file ']'
        { $$.eptr = wherenode($3.eptr,$4.eptr,$5.eptr); }
;

wherebody:
        statement
        { $$.eptr = listnode(NULL,$1.eptr); }
|       wherebody  statement
                      { $$.eptr = listnode($1.eptr,$2.eptr); }
;

statement: '[' DEFN WORD NUMB argnames expr file ']'
            { $$.eptr =
                      defnode($3.strg,(int)$4.numb,$5.eptr,$6.eptr,$7.eptr); }
|           '[' DEFN WORD NUMB expr file ']'
            { $$.eptr = defnode($3.strg,(int)$4.numb,NULL,$5.eptr,$6.eptr); }
|           '[' DECL WORD expr file ']'
            { $$.eptr = declnode($3.strg,$4.eptr,$5.eptr); }
;

argnames:   WORD
             { $$.eptr = identlistnode(NULL,$1.strg); }
|           argnames WORD
            { $$.eptr = identlistnode($1.eptr,$2.strg); }
;

expr:      where { $$.eptr = $1.eptr; }
|          constant { $$.eptr = $1.eptr; }
|          variable {   $$.eptr = $1.eptr; }
|          operator { $$.eptr = $1.eptr; }
;

exprlist:  expr
            { $$.eptr = listnode(NULL,$1.eptr); }
|          exprlist  expr
            { $$.eptr = listnode($1.eptr,$2.eptr); }
;
