

#include "cmanifs.h"
#include "cglobals.h"
/**************************************************************/
/**                                                          **/
/**   The directory where all the library files are kept;    **/
/**   this usually needs changing when the system is being   **/
/**   installed at a new site                                **/
/**                                                          **/
/**************************************************************/

#define NSYNWORDS  62

struct {
	  STRING synname;
	  STRING keyname;
       } synwords[NSYNWORDS] =
{
	{"initial",     "initial0"},
	{"original",          "original0"},
	{"noriginal",          "noriginal0"},
	{"next",          "next0"},
	{"nrest",          "nrest0"},
	{"rest",          "rest0"},
	{"ncby",          "ncby0"},
	{"cby",          "cby0"},
	{"fby",          "fby0"},
	{"next",          "next0"},
	{"prev",          "prev0"},
	{"pred",          "pred0"},
	{"sby",          "sby0"},
	{"up",          "succ1"},
	{"down",        "pred1"},
	{"left",     	"pred0"},
	{"front",       "succ2"},
	{"rear",	"pred2"},
	{"back",	"pred2"},
	{"right",       "succ0"},
	{"sindex",      "here0"},
	{"sindex0",     "here0"},
	{"sindex1",     "here1"},
	{"sindex2",     "here2"},
	{"succ",        "succ0"},
	{"first",       "first0"},
	{"upon",        "delay0"},
	{"upon0",        "delay0"},
	{"upon1",        "delay1"},
	{"upon2",        "delay2"},
	{"wvr",         "wvr0"},
	{"whenever",    "wvr0"},
	{"whenever0",   "wvr0"},
	{"whenever1",   "wvr1"},
	{"whenever2",   "wvr2"},
	{"asa",         "asa0"},
	{"wherever",    "whrvr0"},
	{"wherever0",   "whrvr0"},
	{"wherever1",   "whrvr1"},
	{"wherever2",   "whrvr2"},
	{"beside",      "beside0"},
	{"before",      "before0"},
	{"delay",       "delay0"},
	{"whr",         "whr0"},
	{"elt",         "elt0"},
	{"all",       	"all0"},
	{"attime",     	"@t0"},
	{"attime0",    	"@t0"},
	{"attime1",    	"@t1"},
	{"attime2",    	"@t2"},
	{"@t",       	"@t0"},
	{"now",   	"now0"},
	{"index",   	"now0"},
	{"atspace",   	"@s0"},
	{"atspace0",   	"@s0"},
	{"atspace1",   	"@s1"},
	{"atspace2",   	"@s2"},
	{"@s",       	"@s0"},
	{"stretch",    	"stretch0"},
	{"whrvr",      	"whrvr0"},
	{"here",      	"here0"},
	{"isnull",     	"isnil"},
};

struct {
	  STRING keyname;
	  int keyret;
       } keywords[NKEYWORDS] =
{
	{"if",          IF},
	{"then",        THEN},
	{"elseif",      ELSIF},
	{"else",        ELSE},
	{"fi",          FI},
	{"case",        CASE},
	{"of",          OF},
	{"default",     DEFAULT},
	{"where",       WHERE},
	{"end",         END},
	{"first0",     LU1FUNC},
	{"first1",     LU1FUNC},
	{"first2",     LU1FUNC},
	{"next0",      LU1FUNC},
	{"next1",      LU1FUNC},
	{"next2",      LU1FUNC},
	{"pred0",      LU1FUNC},
	{"pred1",      LU1FUNC},
	{"pred2",      LU1FUNC},
	{"prev0",      LU1FUNC},
	{"prev1",      LU1FUNC},
	{"prev2",      LU1FUNC},
	{"fby0",       FBY},
	{"fby1",       FBY},
	{"fby2",       FBY},
	{"before0",       FBY},
	{"before1",       FBY},
	{"before2",       FBY},
	{"asa0",       LU2FUNC},
	{"asa1",       LU2FUNC},
	{"asa2",       LU2FUNC},
	{"whn0",  LU2FUNC},
	{"whn1",  LU2FUNC},
	{"whn2",  LU2FUNC},
	{"wvr0",       LU2FUNC},
	{"wvr1",       LU2FUNC},
	{"wvr2",       LU2FUNC},
	{"delay0",        LU2FUNC},
	{"delay1",        LU2FUNC},
	{"delay2",        LU2FUNC},
	{"upon0",      LU2FUNC},
	{"upon1",      LU2FUNC},
	{"upon2",      LU2FUNC},
	{"now0",	LU0FUNC},
	{"now1",	LU0FUNC},
	{"now2",	LU0FUNC},
	{"@t0",		LU2FUNC},
	{"@t1",		LU2FUNC},
	{"@t2",		LU2FUNC},
	{"initial0",       LU1FUNC},
	{"initial1",       LU1FUNC},
	{"initial2",       LU1FUNC},
	{"succ0",      LU1FUNC},
	{"succ1",      LU1FUNC},
	{"succ2",      LU1FUNC},
	{"sby0",         FBY},
	{"sby1",         FBY},
	{"sby2",         FBY},
	{"whr0",         LU2FUNC},
	{"whr1",         LU2FUNC},
	{"whr2",         LU2FUNC},
	{"whrvr0",         LU2FUNC},
	{"whrvr1",         LU2FUNC},
	{"whrvr2",         LU2FUNC},
	{"stretch0",        LU2FUNC},
	{"stretch1",        LU2FUNC},
	{"stretch2",        LU2FUNC},
	{"here0",       LU0FUNC},
	{"here1",       LU0FUNC},
	{"here2",       LU0FUNC},
	{"@s0",		  LU2FUNC},
	{"@s1",		  LU2FUNC},
	{"@s2",		  LU2FUNC},
	{"original0",       LU1FUNC},
	{"original1",       LU1FUNC},
	{"original2",       LU1FUNC},
	{"noriginal0",       LU1FUNC},
	{"noriginal1",       LU1FUNC},
	{"noriginal2",       LU1FUNC},
	{"rest0",        LU1FUNC},
	{"rest1",        LU1FUNC},
	{"rest2",        LU1FUNC},
	{"nrest0",        LU1FUNC},
	{"nrest1",        LU1FUNC},
	{"nrest2",        LU1FUNC},
	{"cby0",        FBY},
	{"cby1",        FBY},
	{"cby2",        FBY},
	{"aby0",        FBY},
	{"aby1",        FBY},
	{"aby2",        FBY},
	{"swap_t0s0",	METALU},
	{"swap_t0s1",	METALU},
	{"swap_t0s2",	METALU},
	{"swap_t1s1",	METALU},
	{"swap_t1s2",	METALU},
	{"swap_t2s3",	METALU},
	{"swap_t0t1",	METALU},
	{"swap_t0t2",	METALU},
	{"swap_t0t3",	METALU},
	{"swap_t1t2",	METALU},
	{"swap_t1t3",	METALU},
	{"swap_t2t3",	METALU},
	{"swap_s0s1",	METALU},
	{"swap_s0s2",	METALU},
	{"swap_s0s3",	METALU},
	{"swap_s1s2",	METALU},
	{"swap_s1s3",	METALU},
	{"swap_s2s3",	METALU},
	{"rshift",	LU1FUNC},
	{"lshift",	LU1FUNC},
	{"all0", 	LU1FUNC},
	{"all1", 	LU1FUNC},
	{"all2", 	LU1FUNC},
	{"elt0", 	LU1FUNC},
	{"elt1", 	LU1FUNC},
	{"elt2", 	LU1FUNC},
	{"isnumber",    R1FUNC},
	{"div",         MULOP},
	{"mod",         MULOP},
	{"and",         AND},
	{"not",         NOT},
	{"or",          OR},
	{"eq",          RELOP},
	{"ne",          RELOP},
	{"true",        WORD},
	{"false",       WORD},
	{"sin",         R1FUNC},
	{"cos",         R1FUNC},
	{"log",         R1FUNC},
	{"is",          IS},
	{"current",     CURRENT},
	{"iseod",       R1FUNC },
	{"isstring",    R1FUNC},
	{"isword",      R1FUNC},
	{"substr",      R3FUNC},
	{"eod",         R0FUNC},
	{"arg",         R1FUNC},
	{"mkword",      R1FUNC},
	{"mkstring",    R1FUNC},
	{"error",       R0FUNC},
	{"iserror",     R1FUNC},
	{"length",      R1FUNC},
	{"mknumber",    R1FUNC},
	{"tan",         R1FUNC},
	{"log10",       R1FUNC},
	{"abs",         R1FUNC},
	{"sqrt",        R1FUNC},
	{"cond",        COND},
	{"filter",      R3FUNC},
        {"cons",        R2FUNC}, 
        {"nil",         R0FUNC},
        {"islist",      R1FUNC},
        {"isatom",      R1FUNC},
        {"chr",         R1FUNC},
        {"ord",         R1FUNC},
        {"hd",          R1FUNC},
        {"tl",          R1FUNC},
        {"isnil",       R1FUNC},
        {"complex",     C2FUNC},
        {"real",        C1FUNC},
        {"imag",        C1FUNC},
};


yylex()
{
	int k;
	STRING synfind(),strsave(),getident(),getword2(),getstring(),getword();
	float getnum();
	while(iswhite(c=lexgetc()));
	startoflex = curr_index;
	if((isalpha(c)||c=='@' )&& c != EOF) {
		s = getident(c);
		if(strcmp("include",s)==0){
			inclusion();
			return(yylex());
		}else if((k=keyfind(synfind(s)))!=NKEYWORDS && const_list == 0){
			   yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->strg =  keywords[k].keyname;
			   yylval.resptr->line = in_files[in_index].in_line;
			   yylval.resptr->len  = startoflex;
				return(keywords[k].keyret);
		}else{ if (const_list != 0)
			{
			  yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			  yylval.resptr->strg = s;
			  yylval.resptr->line = in_files[in_index].in_line;
			  yylval.resptr->len  =
			  startoflex;
			  return(WORD);
			}
			else
			{
			  yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			  yylval.resptr->strg = s;
			  yylval.resptr->line = in_files[in_index].in_line;
			  yylval.resptr->len  =
			  startoflex;
			  return(IDENT);
			}
		     }
       }else if((isdigit(c) || c == '~') && c !=EOF) {
		yylval.numb = (float) getnum(c);
		return(CONST);
	}else{
		switch(c){
		case '+':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			if (const_list!=0) {
			     yylval.resptr->strg=getword2(c);
			   yylval.resptr->line = in_files[in_index].in_line;
			   yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			yylval.resptr->strg = "plus";
			yylval.resptr->line = in_files[in_index].in_line;
			yylval.resptr->len  =
			  startoflex;
			return(ADDOP);
		case '-':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			if (const_list!=0) {
			     yylval.resptr->strg=getword2(c);
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			yylval.resptr->strg =  "minus";
			   yylval.resptr->line = in_files[in_index].in_line;
			yylval.resptr->len  =
			  startoflex;
			return(ADDOP);
		case '*':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			if (const_list!=0) {
			     yylval.resptr->strg=getword2(c);
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			c=lexgetc();
			yylval.resptr->line = in_files[in_index].in_line;
			if (c=='*') { yylval.resptr->strg =  "exp";
				      yylval.resptr->len  = startoflex;
				      return(EXP); }
			peekc = c;
			yylval.resptr->strg =  "times";
			yylval.resptr->len  = startoflex;
			return(MULOP);
		case '/': c = lexgetc();
			if (const_list!=0) {
			     peekc = c;
			     yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			     yylval.resptr->strg=getword2('/');
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			if (c=='/'){
			while((c=lexgetc())!='\n');
			peekc = c;
			return(yylex()); }
			peekc = c;
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			yylval.resptr->strg =  "fdiv";
			   yylval.resptr->line = in_files[in_index].in_line;
			yylval.resptr->len  =
			  startoflex;
			return(MULOP);
		case '^':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			if (const_list!=0) {
			     yylval.resptr->strg=getword2(c);
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			yylval.resptr->strg ="strconc";
			   yylval.resptr->line = in_files[in_index].in_line;
			yylval.resptr->len  =
			  startoflex;
			return(STRCONC);
		case '#':
			if (const_list!=0) {
			     yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			     yylval.resptr->strg=getword2(c);
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			return(c);
		case '"':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			if (const_list!=0) {
			     yylval.resptr->strg=getword2(c);
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			c = lexgetc() ;
			yylval.resptr->strg = getword(c);
			yylval.resptr->len  =
			  startoflex;
			return(WORD);
		case '`':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			c = lexgetc();
			yylval.resptr->strg =  getstring(c);
			yylval.resptr->len  =
			  startoflex;
			return(STRING_QUOTED);
		case ':':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			   yylval.resptr->len  = startoflex;
			if (const_list!=0) {
			     yylval.resptr->strg = getword(c);
			     return(WORD); }
			c = lexgetc();
			if (c==':') { yylval.resptr->strg = "cons";
					return(CONS); } else {
					peekc=c;
					return(COLON);}
		case '[': c = lexgetc();
			if (c=='%' && const_list!=0) {
			     peekc = c;
			     yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->strg=getword2('[');
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			if (c == '%') { return(CLSTART);
				      }
				      else
				      { peekc = c;
					const_list++;
					return(LSTART);
				      }
		case '%':c = lexgetc();
			if ((c==']' || c==')') && const_list!=0) {
			     peekc = c;
			     yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->strg=getword2('%');
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			  if (c == ']')
				       {  return(CLEND);
				       }
				       else
				       {
				       my_yyerror("incorrect use of %","cannot continue compliation ");
					 my_exit(1);
				       }
		case ']':
				       { const_list--;
					 return(LEND);
				       }
		case '.':
			if (const_list!=0) {
			     yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->strg=getword2(c);
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			return(DOT);
		case '<':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			if (const_list!=0) {
			     yylval.resptr->strg=getword2(c);
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			c=lexgetc();
					if (c=='=') {
						   yylval.resptr->strg =  "le";
						   yylval.resptr->len  =
			  startoflex;
						     return(RELOP); } else
					if (c =='>') {
						 yylval.resptr->strg = "append";
						 yylval.resptr->len  =
			  startoflex;
						   return(APPEND); } else {
						     peekc=c;
						     yylval.resptr->strg = "lt";
						     yylval.resptr->len  =
			  startoflex;
						     return(RELOP);  }
		case '>':
			yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			if (const_list!=0) {
			     yylval.resptr->strg=getword2(c);
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			c=lexgetc();
			if (c=='=') { yylval.resptr->strg =  "ge";
				      yylval.resptr->len  =
			  startoflex;
					return(RELOP); } else {
						 yylval.resptr->strg = "gt";
						 yylval.resptr->len  =
			  startoflex;
						 peekc=c;
						 return(RELOP); }
		case '&':
		case ',':
		case ';':
		case '=':
		case '$':
		case '(':
		case ')':
			if (const_list!=0) {
			     yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			   yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->strg=getword2(c);
			     yylval.resptr->len  =
			  startoflex;
			     return(WORD); }
			 if (c==',') {
			     yylval.resptr = (RESPTR) calloc(1,sizeof(RES));
			     yylval.resptr->line = in_files[in_index].in_line;
			     yylval.resptr->len  = startoflex;
			     return(COMMA); }
		default:
			return(c);
		}
	}
}

inclusion()
{  int i,c;
	while (iswhite(c=lexgetc()));
	if (c != '<' && c != '"') {
	       my_yyerror("< or \" expected after an include statement\n",
			    "cannot continue compilation"); }
    if(c=='"'){ for(s=(char *)buffer; (c=lexgetc())!='"'; *s++ = c ){
				     if (c==EOF) { my_yyerror("reached EOF and have not found \" to close string of include\n","cannot continue compilation");
		   my_exit(1); }
	       };

		*s = '\0';
		while (iswhite(c=lexgetc()));
		if (c != ';') { my_yyerror("; expected after string",
				 "cannot continue compilation");
				my_exit(1); }
	}else{
		s = (char *)buffer;
		t = incdir;
		while(*s++ = *t++);
		s--;
		*s++ = '/';
		while(iswhite(c=lexgetc()));
		if (isalpha(c)) {
		while(isalpha(c) ||isdigit(c) ||c=='.')
			      { *s=c; c=lexgetc(); s++;
				     if (c==EOF) { my_yyerror("reached EOF and have not found >","cannot continue compilation");
		   my_exit(1); }
		 }
		*s = '\0';
		while (iswhite(c)) c=lexgetc();
		if ( c != '>' ) { my_yyerror("> needed to close include statement","cannot continue compilation"); my_exit(1); }
		while (iswhite(c=lexgetc()));
		if (c != ';') { my_yyerror("; expected after include <filename>",
				 "cannot continue compilation");
				my_exit(1); }
		} else { my_yyerror("after < a file name is required",
			"cannot continue compilation"); my_exit(1); }
	}
	if((lexin=fopen(buffer,"r"))==NULL){
		my_yyerror( "cannot open include file"," ");
		fprintf(stderr,"'%s` ",buffer);
		my_exit(1);
	}
		in_index++;
		in_files[in_index].in_fdes = lexin;
		in_files[in_index].in_name = strsave(buffer);
		in_files[in_index].in_line = 1;
			i = 0;
			while ((c=getc(lexin))=='\n') { }
			while ( c !='\n' && c != EOF){
			       linebuf[i]=c;
			       c = getc(lexin);
			       i++; }
			linebuf[i] = c;
			curr_index = 0;
			curr_length = i;


}

lexgetc()
{
	int i,c;
	if(peekc!=0){
		c = peekc;
		peekc = 0;
		return(c);
	}else if((curr_index > curr_length)){

			i = 0;
			while ((c=getc(lexin))=='\n') {
			in_files[in_index].in_line++;
			newline = true;
			      }
			while ( c !='\n' && c != EOF){
			       linebuf[i]=c;
			       c = getc(lexin);
			       i++; }
			linebuf[i] = c;
			curr_index = 0;
			curr_length = i;
			c = linebuf[curr_index];
	}
	 if( ( c=linebuf[curr_index])==EOF){
		if(in_index!=0){
			in_index--;
			lexin = in_files[in_index].in_fdes;
			i = 0;
			c = getc(lexin);
			while ( c !='\n' && c != EOF){
			       linebuf[i]=c;
			       c = getc(lexin);
			       i++; }
			linebuf[i] = c;
			linebuf[i+1] = '\0';
			curr_index = 0;
			curr_length = i;
			c = linebuf[curr_index];

			}
	}
	if(c=='\n'){
			in_files[in_index].in_line++;
			newline = true;
	}
	curr_index++;
	return(c);
}

STRING
getword(c)
char c;
{       int l;
	STRING p;
	char is_sign();
	p = (char *)buffer;
	switch(c) {
	case ';':
	case ',':
	case '.':
	case '"': *p++ = c; l++; c=lexgetc();
		  break;
	case ')': *p++ = c; l++; c=lexgetc();
		  break;
	case '%': *p++ = c; l++; c = lexgetc(); if (c==')'||c==']'){
						*p++ = c; l++;
						c = lexgetc(); break; }
		  my_yyerror("% not a word constant\n","cannot contine with compilation");
		  my_exit(1);
		  break;
	case '(': *p++ = c; l++; c = lexgetc(); if (c == '%') { *p++ = c;
						     l++;  c = lexgetc();
						     }
		  break;
	case '[': *p++ = c; l++; c = lexgetc(); if (c == '%') { *p++ = c;
						     l++;  c = lexgetc();
						     break; }
		  my_yyerror("[ not a word constant\n","cannot continue with compilation\n"); my_exit(1);
		  break;
	case '+':
	case '-':
	case '/':
	case '<':
	case '=':
	case '>':
	case '*':
	case '$':
	case '&':
	case ':':
	case '^':
	case '#': *p++ = c; l++; while (is_sign(c=lexgetc())) {*p++ = c; l++;}
		     break;
	case ']': my_yyerror("] not a word constant\n","cannot continue with compilation\n"); my_exit(1);
	default:
	if ( isalpha(c)) {  *p++ = c;l++; c = lexgetc();
	while ( isalpha(c) || isdigit(c) ) { *p++ = c; l++;
				  c = lexgetc();

			   }
	}
	break;
       }
	if (c != '"') { my_yyerror("error in word constant", "cannot continue with compilation\n"); my_exit(1); }
		/* now at end of word */
	*p = '\0';
	if (l >8 ) p[8] = '\0';
	return((STRING) strsave(buffer));
}


STRING
getword2(c)
int c;
{       int l;
	STRING p;
	char is_sign();
	p = (char *)buffer;
	switch(c) {
	case ';':
	case ',':
	case '.':
	case ')':
	case '"': *p++ = c; l++; c=lexgetc();
		  break;
	case '%': *p++ = c; l++; c = lexgetc(); if(c==']' || c==')') {
						 *p++ = c; l++;
						 c = lexgetc(); break; }
		  my_yyerror("% not a word constant\n","cannot continue with compilation");
		  my_exit(1);
		  break;
	case '(': *p++ = c; l++; c = lexgetc(); if (c == '%') { *p++ = c;
						     l++;  c = lexgetc(); }
		  peekc = c;
		  break;
	case '[': *p++ = c; l++; c = lexgetc(); if (c == '%') { *p++ = c;
						     l++;  c = lexgetc();
						       break; }
		  my_yyerror("[ not a word constant\n","cannot continue with compilation\n");
		  my_exit(1);
		  break;
	case '*':
	case '+':
	case '-':
	case '/':
	case '<':
	case '=':
	case '>':
	case '$':
	case '&':
	case ':':
	case '^':
	case '#': *p++ = c; l++; while (is_sign(c=lexgetc())) {*p++ = c; l++;}
		     break;
	case ']': my_yyerror("] not a word constant\n","cannot continue with compilation"); my_exit(1);
	default:
	if ( isalpha(c)) {  *p++ = c;l++; c = lexgetc();
	while ( isalpha(c) || isdigit(c) ) { *p++ = c; l++;
				  c = lexgetc();
			   }
	}   else { my_yyerror("error in list constant\n","cannot continue with compilation"); my_exit(1); }
	break;
       }
		/* now at end of word */
	peekc = c;
	*p = '\0';
	if (l >8 ) p[8] = '\0';
	return((STRING) strsave(buffer));
}



char
is_sign(c)
char c;
{ switch(c) {
	case ':':
	case '^':
	case '+':
	case '-':
	case '/':
	case '<':
	case '=':
	case '>':
	case '*':
	case '$':
	case '&':
	case '#':  return(true);
	default:   return(false);
     }
}



STRING
getident(c)
int c;
{
	STRING p;
	p = (char *)buffer;
	*p++ = c;
	l = 1;
	for(c = lexgetc(); isalpha(c) || isdigit(c) ||
				c=='_'; c=lexgetc()){
						 *p++ = c;
						 l++;
			   }
	/* now c is not an alpha char, we are beyond the identifier */
	peekc = c;
	*p = '\0';
	return((STRING) strsave(buffer));
}

STRING
getstring(c)
char c;
{
	STRING p;
	char strstrg[200];
	int i,sptr,tlen;
	for (i=0; i<= curr_length; i++){
	 strstrg[i]=linebuf[i]; }
	sptr = curr_index;
	tlen = curr_length;
	p = (char *)buffer;
	l = 0;
	while ( c !='\'' ) {
		   if (c == '\\')  {  c = lexgetc();
				      if (c==EOF) {
			       my_yyerror("EOF reached with no closing quote for string"," ");
				      my_exit(1); }
				      switch (c) {
				      default:
						  *p++ = '\\'; l++;
				      case '\\':
				      case '`':
				      case '\'':
						break;
				       }
				   }
		  switch(c){
		   case '\t': *p++ = '\\'; l++;
			      *p++ = 't';  l++;
			      break;
		   case '\f': *p++ = '\\'; l++;
			      *p++ = 'f';  l++;
			      break;
		   case '\b': *p++ = '\\'; l++;
			      *p++ = 'b';  l++;
			      break;
		   default : *p++ = c;
			     l++;
		 }
		if (l == 255) {
		fprintf(stderr,"WARNING long string\n");
		for (i = 0; i <tlen  ; i++){
		  fprintf(stderr,"%c",strstrg[i]); }
		fprintf(stderr,"\n");
		for ( i = 0; i<sptr-2; i++){
		   if ( isprint(strstrg[i]) ) { putc(' ',stderr); }
		    else putc(strstrg[i],stderr); }
		fprintf(stderr,"^\n");

		 }

		c  =  lexgetc();
				      if (c==EOF) {
			       my_yyerror("EOF reached with no closing quote for string", " "); my_exit(1); }
		}
		/* we are now at the end of the string */
	*p = '\0';
	return((STRING) strsave(buffer));
}

float getnum(c)
char c;
{
	int sign,mansum,digitcount;
	float expsum,expcount;
	if ( c=='~' ) { sign = -1;
			c = lexgetc();
			if ( !isdigit(c) ) {
				  my_yyerror("~ must be followed by a digit",
					      " "); }
	   } else sign = 1;
	digitcount = 1;
	mansum = c - '0';
	expsum = 0;
	expcount = 1;
	for(c=lexgetc(); isdigit(c); c=lexgetc()){ digitcount++;
	       if ( digitcount>7) { my_yyerror("too many digits in numeric constant. Max is 7","\n");
	       while (isdigit(c)) c=lexgetc();
	       if (c=='.') { c=lexgetc();
			    while (isdigit(c)) c=lexgetc(); }
	       return(0);
	       }
	       mansum = mansum * 10 + (c-'0');
	}
	if (c== '.') {
			for (c=lexgetc(); isdigit(c); c=lexgetc()) {
			     expsum=expsum *10 + (c-'0');
			     expcount = expcount*10;
			     }
		     }
		     peekc = c;
		     return(sign*(mansum+expsum/expcount));
}


STRING
synfind(s)
STRING s;
{
  register int i;
  for(i=0; i<NSYNWORDS && (strcmp(s,synwords[i].synname)!=0); i++);
  if (i<NSYNWORDS) return(synwords[i].keyname); 
  return(s);
}

int
keyfind(s)
STRING s;
{
  register int i;
  for(i=0; i<NKEYWORDS && (strcmp(s,keywords[i].keyname)!=0); i++);
  return(i);
}



iswhite(c)
{
	return(c==' ' || c=='\n' || c=='\t');
}
