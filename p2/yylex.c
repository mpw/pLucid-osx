

#include "cmanifs.h"
#include "cglobals.h"


#define NKEYWORDS 15
struct {
	STRING keyname;
	int keyret;
} keywords[NKEYWORDS] =
{
	{"var",         VAR},
	{"const",       CONST},
	{"op",          OP},
	{"where",       WHERE},
	{"decl",        DECL},
	{"defn",        DEFN},
	{"string",      F_STRING},
	{"word",        F_WORD},
	{"numb",        F_NUMB},
	{"special",     F_SPECIAL},
	{"file",        ERRFILE},
	{"ident",       IDENTL},
	{"expr",        EXPRL},
	{"stat",        STATL},
	{"body",        BODY},
};

yylex()
{
	int k;
	STRING strsave();
	STRING getstring(),getword();
	float getnum();

	while(iswhite(c=lexgetc()));
	if ( (isalpha(c)||c=='@') && c!=EOF ) { s = getword(c);
				      if ((k=keyfind(s))!=NKEYWORDS && cconst)
				       {  yylval.strg = keywords[k].keyname;
					  cconst=false;
					  return(keywords[k].keyret);  }
				      yylval.strg = s;
				      return(WORD); }

	if ( isdigit(c)||c=='~' ){ yylval.numb=(float)getnum(c);
				   return(NUMB);}
	if ( c=='`') { c = lexgetc();
			yylval.strg=getstring(c);
			return(STRING_QUOTED); }
	if ( c == '[' ) cconst=true;
	return(c);
}

lexgetc()
{
	int c;
	if(peekc!=0){
		c = peekc;
		peekc = 0;
		return(c);
	}else if (( c = getc(lexin))==EOF){
	      if(in_index!=0){
		      in_index--;
		      lexin = in_files[in_index].in_fdes;
		      c = getc(lexin);
	       }
      }
      if (c=='\n') { in_files[in_index].in_line++;
	}
      if ( c=='\032' ) my_exit(1);
      return(c);
}

STRING
getword(c)
char c;
{       int l;
	STRING p;
	char is_sign();
	p = buffer;
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
		  fprintf(stderr,"ERROR1");
		  my_exit(1);
	case '(': *p++ = c; l++; c = lexgetc(); if (c == '%') { *p++ = c;
						     l++;  c = lexgetc(); }
		  peekc = c;
		  break;
	case '[': *p++ = c; l++; c = lexgetc(); if (c == '%') { *p++ = c;
						     l++;  c = lexgetc();
						     break; }
		  fprintf(stderr,"ERROR2");
		  my_exit(1);
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
	default:
	if ( isalpha(c)||c=='@') {  *p++ = c;l++; c = lexgetc();
	while ( isalpha(c) || isdigit(c) || c=='_') { *p++ = c; l++;
				  c = lexgetc();
			   }
	}   else {
		  fprintf(stderr,"ERROR3");
		  my_exit(1); }
	break;
       }
		/* now at end of word */
	peekc = c;
	*p = '\0';
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
getstring(c)
char c;
{
	STRING p;
	char strstrg[200];
	int i,sptr,tlen;
	p = buffer;
	l = 0;
	while ( c !='\'' ) {
		   if (c == '\\')  {  c = lexgetc();
				      if (c==EOF) {
			       yyerror("EOF reached with no closing quote for string"," ");
				      my_exit(1); }
				      switch (c) {
				      default:
						*p++ = '\\';
						l++;
				      case '\\':
				      case '`':
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

		 }

		c  =  lexgetc();
				      if (c==EOF) {
			       yyerror("EOF reached with no closing quote for string", " "); my_exit(1); }
		}
		/* we are now at the end of the string */
	*p = '\0';
	return((STRING) strsave(buffer));
}

float getnum(c)
char c;
{
	int sign,mansum;
	float expsum,expcount;
	if ( c=='~' ) { sign = -1;
			c = lexgetc();
			if ( !isdigit(c) ) {
				  yyerror("~ must be followed by a digit",
					      " "); }
	   } else sign = 1;
	mansum = c - '0';
	expsum = 0;
	expcount = 1;
	for(c=lexgetc(); isdigit(c); c=lexgetc()){
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
int
keyfind(s)
STRING s;
{
	register int i;
	for(i=0; i<NKEYWORDS && strcmp(s,keywords[i].keyname); i++);
	return(i);
}

iswhite(c)
{
	return(c==' ' || c=='\n' || c=='\t');
}
