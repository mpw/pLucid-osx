
%{
#include "cmanifs.h"
#include "cglobals.h"
EXPRPTR opnode(),wherenode(),defnode(),declnode(),varnode();
EXPRPTR lu_connode(),connode(),f_connode();
EXPRPTR listnode(),identlistnode();
EXPRPTR stripnumb(),bodynode(),exprlist2(),exprlist3();
float stripn(),stripcode();
STRING stripname();
%}
%term
      LU0FUNC LU1FUNC F1FUNC F2FUNC LU2FUNC LU3FUNC WHERE END IS CURRENT
      IF THEN ELSE ELSIF FI CASE COND OF DEFAULT
      AND NOT OR FBY CONS REAR
      IDENT CONST
      U_TYPE
      RELOP MULOP ADDOP APPEND EXP
      METALU R0FUNC R1FUNC R2FUNC R3FUNC C1FUNC C2FUNC
      COMMA COLON DOT WORD STRCONC
      LEND LSTART CLSTART CLEND STRING_QUOTED
%left WHERE
%left LU2FUNC 
%right FBY 
%left IF CASE COND
%right CONS APPEND
%left OR
%left AND
%left NOT
%nonassoc RELOP
%left ADDOP
%left MULOP
%left EXP
%right STRCONC
%left METALU LU1FUNC LU0FUNC R0FUNC R1FUNC C1FUNC

%%

accept:
         expr
       { $$.eptr = $1.eptr;
          if (errcount==0) {  pass1($1.eptr); }
       }
|        expr ';'
       { $$.eptr = $1.eptr;
          if (errcount==0) {  pass1($1.eptr); }
       }
;

constant:
        CONST         { $$.eptr = f_connode($1.numb); }
|       WORD          { $$.eptr = connode("word",$1.resptr->strg); }
|       STRING_QUOTED { $$.eptr = connode("string",$1.resptr->strg); }
;

list:
        CLSTART listbody CLEND { $$.eptr = $2.eptr; }
|       CLSTART CLEND { $$.eptr = connode("special","nil"); }
;

clist:
       LSTART clistbody LEND { $$.eptr = $2.eptr; }
|      LSTART LEND { $$.eptr = connode("special","nil"); }
;

clistbody:
       constant { $$.eptr = opnode("cons",0,
                        exprlist2($1.eptr,connode("special","nil")),
                                 filenode( in_files[in_index].in_line,
                                 curr_index)); }
|      clist    { $$.eptr = opnode("cons",0,
                                exprlist2($1.eptr,connode("special","nil")),
                                 filenode(in_files[in_index].in_line,
                                 curr_index)); }
|      constant clistbody { $$.eptr = opnode("cons",0,
                                           exprlist2($1.eptr,$2.eptr),
                                 filenode(in_files[in_index].in_line,
                                 curr_index)); }
|      clist    clistbody { $$.eptr = opnode("cons",0,
                                        exprlist2($1.eptr,$2.eptr),
                                 filenode(in_files[in_index].in_line,
                                 curr_index)); }

;

listbody:
        expr { $$.eptr = opnode("cons",0,
                              exprlist2($1.eptr,connode("special","nil")),
                                 filenode(in_files[in_index].in_line,
                                 curr_index)); }
|       expr COMMA listbody {
                      $$.eptr = opnode("cons",0,
                                exprlist2($1.eptr,$3.eptr),
                                 filenode(in_files[in_index].in_line,
                                 $2.resptr->len)); }
;

expr:
        expr
        WHERE
        wherebody END
        { $$.eptr = wherenode($1.eptr,$3.eptr,
                        filenode($2.resptr->line,$2.resptr->len)); }
|       expr
        WHERE
        END
        { $$.eptr = $1.eptr; }
|       IDENT           { $$.eptr = varnode($1.resptr->strg,0,NULL,
                             filenode($1.resptr->line,$1.resptr->len)); }
|       constant        { $$.eptr = $1.eptr; }
|       C1FUNC expr
                         { $$.eptr = opnode($1.resptr->strg, 0,
                                             listnode(NULL,$2.eptr),
                             filenode($1.resptr->line,$1.resptr->len)); }
|       C2FUNC '(' expr COMMA expr ')' {
                $$.eptr = opnode($1.resptr->strg,0,exprlist2($3.eptr,$5.eptr),
                                 filenode($1.resptr->line,$1.resptr->len)); }
|       R0FUNC          { $$.eptr = connode("special",$1.resptr->strg); }
|       R1FUNC expr
                         { $$.eptr = opnode($1.resptr->strg, 0,
                                             listnode(NULL,$2.eptr),
                             filenode($1.resptr->line,$1.resptr->len)); }
|       list            { $$.eptr = $1.eptr; }
|       clist           { $$.eptr = $1.eptr; }
|       '(' expr ')'    { $$.eptr = $2.eptr; }
|       IDENT '('      { exprlistcount[++exprlevel]=0;}
        exprlist ')' {
            $$.eptr = varnode($1.resptr->strg,exprlistcount[exprlevel--]
                                ,$4.eptr,
                            filenode($1.resptr->line,$1.resptr->len)); }
|       LU0FUNC        { $$.eptr =opnode(stripname($1.resptr->strg),
				(int)stripn($1.resptr->strg),
				listnode(NULL,stripnumb($1.resptr->strg)),
                            filenode($1.resptr->line,$1.resptr->len)); }
|       METALU expr {
             $$.eptr = opnode(stripname($1.resptr->strg),
				(int)stripcode($1.resptr->strg),
		listnode(NULL,$2.eptr),
                            filenode($1.resptr->line,$1.resptr->len)); }
|       expr LU2FUNC expr {
             $$.eptr = opnode(stripname($2.resptr->strg),
				(int)stripn($2.resptr->strg),
		exprlist2($1.eptr,$3.eptr),
                            filenode($2.resptr->line,$2.resptr->len)); }
|       expr FBY expr { $$.eptr =
                         opnode(stripname($2.resptr->strg),
				(int)stripn($2.resptr->strg),
			exprlist2($1.eptr,$3.eptr),
                            filenode($2.resptr->line,$2.resptr->len)); }
|       LU1FUNC expr
              {  $$.eptr = opnode(stripname($1.resptr->strg),
				(int)stripn($1.resptr->strg),
			listnode(NULL,$2.eptr),
                             filenode($1.resptr->line,$1.resptr->len)); }
|       LU3FUNC '(' expr COMMA expr COMMA expr ')' {
                $$.eptr =
                 opnode($1.resptr->strg,3,exprlist3($3.eptr,$5.eptr,$7.eptr),
                             filenode($1.resptr->line,$1.resptr->len)); }
|       R3FUNC '(' expr COMMA expr COMMA expr ')' {
                $$.eptr =
                 opnode($1.resptr->strg,3,exprlist3($3.eptr,$5.eptr,$7.eptr),
                             filenode($1.resptr->line,$1.resptr->len)); }
|       REAR '(' expr COMMA expr ')' {
                 $$.eptr = opnode("rear",0,exprlist2($3.eptr,$5.eptr),
                                 filenode($1.resptr->line,$1.resptr->len)); }
|       expr STRCONC expr {
                $$.eptr = opnode("strconc",0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       expr CONS expr {
                $$.eptr = opnode("cons",0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       R2FUNC '(' expr COMMA expr ')' {
                $$.eptr = opnode($1.resptr->strg,0,exprlist2($3.eptr,$5.eptr),
                                 filenode($1.resptr->line,$1.resptr->len)); }
|       expr APPEND expr {
                $$.eptr = opnode("append",0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       expr ADDOP expr {
                $$.eptr =
                        opnode(($2.resptr->strg),0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       ADDOP expr %prec MULOP {
                if(strcmp($1.resptr->strg,"minus")==0){
                        $$.eptr = opnode("uminus",0,listnode(NULL,$2.eptr),
                                 filenode($1.resptr->line,$1.resptr->len)); }
                else{
                        $$.eptr = $2.eptr;
                }
        }
|       expr MULOP expr {
                $$.eptr = opnode($2.resptr->strg,0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       expr EXP expr {
                $$.eptr = opnode($2.resptr->strg,0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       expr RELOP expr {
                $$.eptr = opnode($2.resptr->strg,0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       NOT expr %prec AND {
                $$.eptr = opnode("not",0,listnode(NULL,$2.eptr),
                                 filenode($1.resptr->line,$1.resptr->len)); }
|       expr AND expr {
                $$.eptr = opnode("and",0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       expr OR expr {
                $$.eptr = opnode("or",0,exprlist2($1.eptr,$3.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       ifexpression {
                $$.eptr = $1.eptr;
        }
|       casexpr {
                $$.eptr = $1.eptr;
        }
|       condexpr {
                $$.eptr = $1.eptr;
        }
|       error { my_yyerror("error in expression"," "); }
;

ifexpression:
        IF expr THEN expr ending {
                $$.eptr = opnode("if",0,exprlist3($2.eptr,$4.eptr,$5.eptr),
                                 filenode($1.resptr->line,$1.resptr->len)); }
|       IF expr THEN expr error {
                my_yyerror("else or elsif expected"," ");
        }
|       IF expr error {
                my_yyerror("then expected", " ");
        } expr ending
;

ending:
        ELSIF expr THEN expr ending {
                $$.eptr = opnode("if",0,exprlist3($2.eptr,$4.eptr,$5.eptr),
                                 filenode($1.resptr->line,$1.resptr->len)); }
|       ELSIF expr THEN expr error {
                my_yyerror("else or elsif expected"," ");
        }
|       ELSIF expr error {
                my_yyerror("then expected"," ");
        } expr ending
|       ELSE expr FI {
                $$.eptr = $2.eptr;
        }
|       ELSE expr error {
                my_yyerror("fi expected"," ");
        }
;

casexpr:
   CASE { expvalcnt++; } expr { expval[expvalcnt]=$3.eptr;} OF casebody END {
                $$.eptr = $6.eptr;
                expvalcnt--;
        }
;

casebody:
        expr COLON expr ';' casebody { $$.eptr=opnode("if",0,
                                               exprlist3(opnode("eq",0,
                                               exprlist2(expval[expvalcnt],
                                               $1.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)
                                               ), $3.eptr,$5.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       expr error { my_yyerror(": expected"," "); } expr ';'
|       DEFAULT error { my_yyerror(": expected"," "); } expr ';'
|       expr COLON expr error { my_yyerror("; expected"," "); }
|       DEFAULT COLON expr error { my_yyerror("; expected"," "); }
|       DEFAULT COLON expr ';' {
        $$.eptr = $3.eptr;
        }
;

condexpr:
   COND condbody END
   { $$.eptr = $2.eptr; }
;

condbody:
        expr COLON expr ';' condbody { $$.eptr=opnode("if",0,
                                               exprlist3($1.eptr,
                                               $3.eptr,$5.eptr),
                                 filenode($2.resptr->line,$2.resptr->len)); }
|       expr error { my_yyerror(": expected"," "); } expr ';'
|       DEFAULT error { my_yyerror(": expected"," "); } expr ';'
|       expr COLON expr error { my_yyerror("; expected"," "); }
|       DEFAULT COLON expr error { my_yyerror("; expected"," "); }
|       DEFAULT COLON expr ';' {
        $$.eptr = $3.eptr;
        }
;


wherebody:
        definitions   { $$.eptr = bodynode(NULL,$1.eptr); }
|       declarations  { $$.eptr = bodynode(NULL,$1.eptr); }
|       declarations definitions
                      { $$.eptr = bodynode($1.eptr,$2.eptr); }
;

definitions:
        assertion  { $$.eptr = listnode(NULL,$1.eptr); }
|       definitions assertion
                   { $$.eptr = listnode($1.eptr,$2.eptr); }
;

declarations:
        currenting   { $$.eptr = listnode(NULL,$1.eptr); }
|       declarations currenting
                     { $$.eptr = listnode($1.eptr,$2.eptr);  }
;


currenting:
           IDENT IS CURRENT expr ';'
           { $$.eptr = declnode($1.resptr->strg,$4.eptr,
                        filenode($1.resptr->line,$1.resptr->len)); }
;


identlist:
        IDENT  { $$.eptr =identlistnode(NULL,$1.resptr->strg);
                 idcount++; }
|       identlist COMMA IDENT
               { $$.eptr =identlistnode($1.eptr,$3.resptr->strg); idcount++;}
|       identlist error { my_yyerror(", expected"," "); } IDENT
;

assertion:
        IDENT '=' expr ';' {
                $$.eptr = defnode($1.resptr->strg,0,NULL,$3.eptr,
                        filenode($1.resptr->line,$1.resptr->len)); }
|       IDENT '(' { idcount=0; } identlist ')' '=' expr ';' {
                $$.eptr = defnode($1.resptr->strg,idcount,$4.eptr,$7.eptr,
                        filenode($1.resptr->line,$1.resptr->len)); }

;

exprlist:
        expr {  $$.eptr = listnode(NULL,$1.eptr);
                exprlistcount[exprlevel]++; }
|       exprlist COMMA expr {
                $$.eptr = listnode($1.eptr,$3.eptr);
                exprlistcount[exprlevel]++;
        }
|       exprlist error { my_yyerror(", expected"," "); } expr
;
