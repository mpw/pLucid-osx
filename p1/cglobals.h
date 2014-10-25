
STRING   incdir;

EXPRPTR  expval[NESTINGDEPTH];
int      expvalcnt;
	 /* Used to keep track of the current expression part
	    of a case statement i.e. case <expr> of ... end */

int      exprlevel,exprlistcount[NESTINGDEPTH];
	 /* Used to keep count of the number of arguments applied
	    to a function */

int      c,peekc;
	 /* c     - used to hold current character
	    peekc - used by input routine to allow a one character
		    lookahead in yylex.c */

int     newline;

int      startoflex,curr_index,curr_length,in_index;
	 /* Used to store information about the current line and
	    cursor postion. Used to give  pLucid runtime error
	    messages */

int      const_list;
	 /* used throughout yylex.c to indicate where or not a list
	    constant is being read or not */

int      errcount,false,true;

int      l;
	 /*    */

int      idcount;
	 /* Keeps count of the number of parameters a function is defined
	    to have. */

int     linebuf[BUFFERLENGTH];
	 /* Used to buffer the current line of characters being
	    lexically analysed, from current input file. */

FILE     *lexin;
	 /* used to hold the value of the current input file */

YYSTYPE  yylval;

struct   { FILE   *in_fdes;
	   STRING in_name;
	   int    in_line;
	 } in_files[NOOFFILES];

STRING   s,t;

int     buffer[500];
