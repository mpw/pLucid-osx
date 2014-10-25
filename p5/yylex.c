
/*******************************************************

		    Flucid Interpreter

		     Copyright 1986
		   All Rights Reserved

		    Dr. Antony Faustini
		  Arizona State University


********************************************************/


#include "cmanifs.h"
#include "cglobals.h"

yylex()
{
	int k;
	STRING strsave();
	STRING getstring(),getword();
	SYMPTR handle_ident();
	float getnum();
	while(iswhite(c=lexgetc()));
	if ((c=='@'||c== '_'||isalpha(c))&&c!=EOF)
	 { s = getword(c);
	   if ((k=keyfind(s))!=MAXFUNCS&&cconst)
	    {
	      cconst=false;
	      if (ftable[k].type==WRD) wordval=true;
              if (ftable[k].nargs==0) 
		{ 
		  yylval.value=ftable[k].type;
		} else	
		  yylval.value=k;
              return(ftable[k].type); 
            }
      	  yylval.ident = handle_ident(s,l);
          return(IDENT); 
	 }
	if ( isdigit(c)||c=='~' )
	 { yylval.numb=(float)getnum(c);
	   return(NUMB);
	 }
	if ( c=='`') 
	 { c = lexgetc();
  	   if (wordval) 
	    {
	      yylval.value=findword(getstring(c));
	      wordval=false; 
	    } else
	      yylval.value=findword(getstring(c));
	      return(STRING_QUOTED); 
	  }
	if (c=='[') cconst=true;
	return(c);
}

lexgetc()
{
	int c;
	if(peekc!=0){
		c = peekc;
		peekc = 0;
		return(c);
	}else if ((c = getc(lexin))==EOF){
	      if(in_index!=0){
		      in_index--;
		      lexin = in_files[in_index].in_fdes;
		      c = getc(lexin);
	       }
      }
      if (c=='\n') { in_files[in_index].in_line++;
	}
      if ( c == '\032') my_exit(1);
      return(c);
}


findstring(s)
STRING s;
{
	int i;
	STRING strsave();
	for(i=0; i<stringcount; i++){
		if(!strcmp(s,stringtable[i])){

			break;
		}
	}
	if(i==stringcount){
		stringcount++;
		stringtable[i] = strsave(s);
	}
	return(i);
}

findword(s)
STRING s;
{
	int i;
	STRING strsave();
	for(i=0; i<wordcount; i++){
		if(!strcmp(s,wordtable[i])){

			break;
		}
	}
	if(i==wordcount){
		wordcount++;
		wordtable[i] = strsave(s);
	}
	return(i);
}

STRING
getword(c)
char c;
{
	STRING p;
	char is_sign();
	p = buffer;
	l = 1;
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
	if ( isalpha(c) || c=='_'||c=='@') {  *p++ = c;l++; c = lexgetc();
	while ( isalpha(c) || isdigit(c) || c=='_') { *p++ = c; l++;
				  c = lexgetc();
			   }
	}   else {
		  fprintf(stderr,"ERROR3");
		  my_exit(1); }
       }
		/* now at end of word */
	peekc = c;
	*p = '\0';
	return((STRING) buffer);
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
code_to_char(s)
STRING s;
{ char c,newstring[200];
  int j,i,l,sum;
  i = 0;
  l = 0;
  c = s[i];
  while (c != '\0')
  { if (c == '\\')
     { i++; c = s[i];
       switch(c){
       case 'n': newstring[l]='\n'; l++;
		 break;
       case 't': newstring[l]='\t'; l++;
		 break;
       case 'f': newstring[l]='\f'; l++;
		 break;
       case 'b': newstring[l]='\b'; l++;
		 break;
       case 'r': newstring[l]='\r'; l++;
		 break;
       case '\\': newstring[l]='\\'; l++;
		 break;
       case '\'': newstring[l]='\''; l++;
		 break;
       default: if ( c >= '0' && c <= '7' )
		{ sum =0;
		  for(j=1; j<=3 && c<='7'&& c>='0' ; j++)
		     { i++;
		       sum = sum*8+ c-'0';
		       c = s[i];
		     }
		   newstring[l]=sum; l++;
		}

		newstring[l]=c; l++;

       }
     } else { newstring[l]=c; l++; }
     i++;
     c = s[i];
  }
  newstring[l] = '\0';
  strcpy(s,newstring);
  return(s);
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
	return((STRING) strsave(code_to_char(buffer)));
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
	int i;
	for(i=0; i<MAXFUNCS && strcmp(s,ftable[i].name); i++);
	return(i);
}

iswhite(c)
{
	return(c==' ' || c=='\n' || c=='\t');
}
