#include "imanifs.h"
#include "iglobals.h"

/*
 *      all code for input to the interpreter is local to this file
 */


int inlist = DOTTED_PAIR;
int read_err,readerr,buffer[200];

void killspace();

void f_arg(e)
EXPRPTR e;
{  
	int val;
	STRING code_to_char();
	CELLPTR findstring();
	eval(arg1.x);
	switch(VStype)
	{ 
	case NUMERIC:  
		val = (int)VSvalue.na.r;
		if (val<=0)
		{ 
			error("arg of the function arg is out of range ",
			e->arg2.x,VStype, VSvalue);
			VStype = ERROR;
			return;
		}
		if (val>memargc-active_filters) {
			VSvalue.strg = findstring('\0'); 
		}
		else VSvalue.strg = findstring(
		memargv[active_filters+val]);
		VStype = QSTRING;
		return;
	case EOD:       
		return;
	case ERROR:
		error("argument of function arg is ",e->arg2.x,VStype,VSvalue);
		return;
	default: 
		error("argument of function arg is NUMERIC not ",e->arg2.x,VStype,VSvalue);
		VStype = ERROR;
		return;
	}
}

/*hiaton()
{   extern rupture();
    if (isatty(fileno(stdout))) signal(SIGINT,rupture);
    readerr=true;
    signal(SIGALRM,hiaton);
}*/

hiaton()
{   
	extern rupture();
	EXPRPTR e;
	STPPTR s,t,p;
	IOPTR save_io;
	STPPTR stpsearch();
	VALUE x;
	signal(SIGALRM,hiaton);
	alarm(1);
	t = STPSt;
	p = STPSp;
	s = STPSs;
	e = current_io->expr;
	STPSpush;
	STPSp=p;
	STPSs=s;
	if (isatty(fileno(stdout))) signal(SIGINT,rupture);
	STPSt = stpsearch((long)current_io->outx++,t->stptl, thashtab);
	save_io = current_io;
	eval(arg2.x);
	current_io = save_io;
	x.v_type = VStype;
	x.v_value = VSvalue;
	dumpval2(current_io->out,x);
	if ( current_io->outmode==POP) fprintf(current_io->out,"\n");
	VSpop;
	STPSpop;
}

int
mygetchar()
{ 
	int ch;
	E(13) if (current_io->chin == EOF) return(EOF); 
	if (filterp[filterlevel]&&!current_io->ignore&&!current_io->ptwize)
	{
		alarm(1);
		ch = getc(current_io->in);
		alarm(0);
		if (ch == EOF || (ch =='@' && !cflag))  
		{ 
			ch = EOF; 
			fclose(current_io->in);
			fclose(current_io->out);
		}
		current_io->chin = ch;
		return(ch);
	}
	ch = getc(current_io->in);
	if (ch=='@' && !cflag) ch = EOF;
	current_io->chin = ch;      /* remove this if we have problems with eod
		                                 and filter  */
	if (ch==EOF && current_io->in==stdin
	    && feof(current_io->in)==0) ch = mygetchar();
	if (ch == EOF && current_io->in == stdin)
	{ 
		eodch = ch;
		current_io->chin = EOF; 
	}
	if (ch == EOF && !isatty(fileno(current_io->in))) fclose(current_io->in);
	current_io->chin = ch;
	return(ch) ;
}

/*int
mygetchar()
{ int ch,i;
  EXPRPTR e;
  STPPTR s, t,p;
  IOPTR save_io;
  STPPTR stpsearch();
  VALUE x;
  if (filterp[filterlevel]&&!current_io->ignore&&!current_io->ptwize)
       {if (current_io->chin==EOF) return(EOF);
                  s = STPSs;
		  t = STPSt;
		  p = STPSp;
		  i = current_io->outx;
		  e = current_io->expr;
		  STPSpush;
		  STPSp=p;
		  readerr=false;
                  STPSs=s;
		  alarm(1);
		  ch = getc(current_io->in);
		  alarm(0);
		  while (readerr ) {  clearerr(current_io->in);
				      readerr = false;
				      if (current_io->chout!=EOF)
				       { STPSt = stpsearch((long)i,
                                                  t->stptl,
						  thashtab);
					 save_io = current_io;
					 eval(arg2.x);
					 current_io = save_io;
					 x.v_type = VStype;
					 x.v_value = VSvalue;
					 dumpval2(current_io->out,x);
					 if ( current_io->outmode==POP)
					  fprintf(current_io->out,"\n");
					 i++;VSpop;
				       }
				      readerr = false;
				      alarm(1);
				      ch = getc(current_io->in);
				      alarm(0);
				      if (!readerr) break;
				   }
		if (ch == EOF) fclose(current_io->in);
		current_io->outx=i;
		current_io->chin = ch;
		STPSpop;
		return(ch);
	      }
  if (current_io->chin == EOF) return(EOF);
  ch = getc(current_io->in);
  if (ch=='@' && !cflag) ch = EOF;
  if (ch==EOF && current_io->in==stdin
	      && feof(current_io->in)==0) ch = mygetchar();
  if (ch == EOF && current_io->in == stdin)
				 { eodch = ch;
				   current_io->chin = EOF; }
  if (ch == EOF && !isatty(fileno(current_io->in))) fclose(current_io->in);
  current_io->chin = ch;
  return(ch) ;
}*/

int read_c_item(v)
CELLUNION *v;
{
	CELLPTR read_c_string();
	ch = mygetchar();
	if (ch == EOF ) return(EOD);
	v->strg = read_c_string(ch);
	return(QSTRING);
}

CELLPTR
read_c_string(c)
char c;
{  
	STRING p;
	CELLPTR findstring();
	char buff[5];
	p = buff;
	*p++ = c;
	*p = '\0';
	return(findstring(buff));
}

int
read_p_item(v)
CELLUNION *v;
{
	char *alloc();
	int mygetchar();
	CELLPTR readlist(),readstring();
	char is_word();
	float readnumber();
	int tmp;
	killspace();
	if( ch=='[' ){
		ch = mygetchar();
		if (ch == '%') { 
			char buf[5];
			buf[0] ='[';
			buf[1] ='%';
			buf[2] ='\0';
			ch = mygetchar();
			v->wrd = findword(buf);
			return(WORD);
		}
		/*get rid of [ */
		killspace();
		inlist = DOTTED_PAIR;
		if(ch!=']'){
			v->dp = readlist();
			if (v->dp==NULL) return(inlist);
			if (inlist == ERROR) return(ERROR);
			return(DOTTED_PAIR);
		}
		else{
			ch = mygetchar();
			return(NIL);
		}
	} 
	else if ( ch == EOF ){
		return(EOD);
	}
	else if(isdigit(ch) ){
		v->na.r = readnumber();
		if (read_err) return(ERROR);
		return(NUMERIC);
	}
	else if(ch == '?'){
		ch = mygetchar();
		return(ERROR);
	}
	else if(ch == '@'){
		while ((ch=mygetchar())==' ') ;
		fclose(current_io->in);
		current_io->chin = EOF;
		return(EOD);
	}
	else if(ch =='~'){
		ch = mygetchar();
		if ( isdigit(ch) ) {
			v->na.r = -readnumber();
			if (read_err) return(ERROR);
			return(NUMERIC);  
		}
		return(ERROR);
	}
	else if(is_word(ch)){
		tmp = getword2(ch);
		if (tmp == -1 ) return(EOD);
		if (tmp == -2 ) return(ERROR);
		v->wrd = tmp;
		return(WORD);
	} 
	else if ( ch == '`') {
		v->strg = readstring(mygetchar());
		return(QSTRING);
	} 
	else {
		E(30) fprintf(stderr,
		"\na constant cannot begin with \"%c\"\n",ch);
		ch = mygetchar();
		return(ERROR);
	}
}

int
findword(s)
STRING s;
{
	int i;
	STRING ngc_strsave();
	for(i=0; i<wordquan; i++){
		if(!strcmp(s,wordtable[i])){

			break;
		}
	}
	if(i==wordquan){
		wordquan++;
		if (wordquan > MAXWORDS ) {
			fprintf(stderr,"too many words in program\n"); 
			return(-2); 
		}
		wordtable[i] = (STRING) ngc_strsave(s);
		if ( wordtable[i] == NULL ) {
			fprintf(stderr,"exceeded total dynamic storage for words\n");
			return(-2); 
		}
	}
	return(i);
}

/*CELLPTR
findstring(s)
STRING s;
{       CELLPTR temp;
	char ch,*alloc();
	temp = (CELLPTR) alloc(SMALL_RECORD);
	VSpush;
	VStype = QSTRING;
	VSvalue.strg = temp;
	ch = s[0];
	temp->data.bits.d_hd = SWCHAR;
	temp->hd.swch = ch;
	temp->data.bits.d_tl = NIL;
	if ( s[0] != '\0' ) {
	 temp->data.bits.d_tl = QSTRING;
	 temp->tl.strg = findstring(&s[1]); }
       VSpop;
       return(temp);
} */

CELLPTR
readstring(c)
int c;
{       
	CELLPTR findstring();
	STRING p,code_to_char();
	int l;
	p = (char *)buffer;
	l = 0;
	while ( c !='\'' ) {
		switch(c){
		case '\\': 
			c = mygetchar();
			switch (c){
			case EOF:  
				if (!tflags[9])
					fprintf(stderr,
					"\nEOD in middle reading string\n");
				ch = ' ';
				my_exit(1);
			case '\n': 
				c = mygetchar();
				if (c=='\n') {
					if(!tflags[9])
						fprintf(stderr,
						"\nunexpected newline\n");
					ch = ' '; 
					my_exit(1); 
				}
				*p++ = c; 
				l++; 
				break;
			case '\'':  
				*p++ = c; 
				l++; 
				break;
			default:    
				*p++ = '\\'; 
				l++;
				*p++ = c; 
				l++;
				break; 
			}
			break;
		default:   
			*p++ = c; 
			l++;
		}
		if (l>120) {
			if (!tflags[9]) {
				fprintf(stderr,"\nstring constant is too long. Max. length 120 chars\n"); 
			}   
			ch = ' ';
			my_exit(1);
		}
		c  =  mygetchar();
		if (c == '\n'){
			if ( !tflags[9]) {
				fprintf(stderr,"\nunexpected newline char in string constant\n");
				fprintf(stderr,"\nin this context newline should be preceeded by \\\n");        
			}
			ch = ' ';
			my_exit(1); 
		}
		if (c == EOF) my_exit(1);
	}
	/* we are now at the end of the string */
	ch = mygetchar();
	*p = '\0';
	return(findstring(code_to_char(&buffer[0])));
}

char
is_word(c)
int c;
{  
	switch(c){
	case ';':
	case ',':
	case '.':
	case ')':
	case '"':
	case '%':
	case '(':
	case '[':
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
	case '#':  
		return(true);
	default:   
		if ( isalpha(c)) { 
			return(true); 
		}
		return(false);
	}
}


int
getword2(c)
char c;
{       
	int l;
	STRING p;
	char buffer[120],is_sign();
	l = 0;
	p = buffer;
	switch(c) {
	case ';':
	case ',':
	case '.':
	case ')':
	case '"': 
		*p++ = c; 
		l++; 
		ch=mygetchar();
		break;
	case '%': 
		*p++ = c; 
		l++; 
		ch = mygetchar(); 
		if(ch==']' || ch==')') {
			*p++ = ch; 
			l++;
			ch = mygetchar(); 
			break; 
		}
		if (ch == EOF) { 
			return(-1); 
		}
		E(30) fprintf(stderr,"% not a word constant\n");
		return(-2);
	case '(': 
		*p++ = c; 
		l++; 
		ch = mygetchar(); 
		if (ch == '%') { 
			*p++ = ch;
			l++;  
			ch = mygetchar(); 
		}
		break;
	case '[': 
		*p++ = c; 
		l++; 
		ch = mygetchar(); 
		if (ch == '%') { 
			*p++ = ch;
			l++;  
			ch = mygetchar();
			break;  
		}
		if (ch == EOF) { 
			return(-1); 
		}
		E(30) fprintf(stderr,"[ not a word constant\n");
		return(-2);
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
	case '#': 
		*p++ = c; 
		l++; 
		while (is_sign(ch=mygetchar())) {
			*p++ = ch; 
			l++;
		}
		break;
	case ']': 
		E(30) fprintf(stderr,"] not a word constant\n");
		return(-2);
	default:
		/* NOTE isalpha may not be portable */
		if ( isalpha(c)) {  
			*p++ = ch;
			l++; 
			ch = mygetchar();
			while ( isalpha(ch) || isdigit(ch) ) { 
				*p++ = ch; 
				l++;
				ch = mygetchar();
			}
		}   
		else {
			if (c == EOF) { 
				return(-1); 
			}
			E(30) fprintf(stderr,"error in list constant\n");
			return(-2); 
		}
		break;
	}
	/* now at end of word */
	*p = '\0';
	return(findword(buffer));
}


int
makeword(s)
char *s;
{       
	int l,i;
	STRING p;
	char buffer[200],is_sign();
	l = 8;
	for (i =0; i<9 ; i++){
		buffer[i] = s[i];
		if (s[i] == '\0') { 
			l = i;
			break; 
		}
	}
	switch(buffer[0]) {
	case ';':
	case ',':
	case '.':
	case ')':
	case '"': 
		if (l == 1) break;
		return(-2);
	case '%': 
		if((buffer[1]==']' || buffer[1]==')') && l == 2) { 
			break; 
		}
		return(-2);
	case '(': 
		if (buffer[1] == '%' && l ==2 ) { 
			break;
		}
		return(-2);
	case '[': 
		if (buffer[1] == '%' && l == 2) { 
			break;
		}
		return(-2);
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
	case '#': 
		i = 1;
		while (is_sign(buffer[i])) { 
			i++; 
			if (i == 8) break; 
		}
		if ( i < l ) return(-2);
		break;
	default:  
		i = 1;
		if ( isalpha(buffer[0])) {
			while ( isalpha(buffer[i]) || isdigit(buffer[i]) ) { 
				i++;
				if ( i == 8) break;
			}
		}   
		else return(-2);
		if (i < l ) return(-2);
		break;
	}
	/* now at end of word */
	return(findword(buffer));
}

char
is_sign(c)
char c;
{ 
	switch(c) {
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
	case '#':  
		return(true);
	default:   
		return(false);
	}
}


CELLPTR
readlist()
{       
	char listerr;
	CELLPTR v;
	listerr = false;
	v = (CELLPTR) alloc(SMALL_RECORD);
	v->data.bits.d_mark=1;
	v->data.bits.d_hd = read_p_item(&v->hd);
	if (v->data.bits.d_hd == EOD) { 
		inlist = EOD; 
		return(NULL); 
	}
	if (v->data.bits.d_hd == ERROR) { 
		listerr = true;  
	}
	killspace();
	if(ch==']'){
		v->data.bits.d_tl = NIL;
		ch = mygetchar();
	}
	else{
		v->tl.dp = readlist();
		if (v->tl.dp == NULL) return(NULL);
		v->data.bits.d_tl = DOTTED_PAIR;
	}
	if (listerr) inlist = ERROR;
	return(v);
}
float
readnumber()
{
	int sum,sign,digitcount;
	float rsum, cnt;
	sum = 0;
	digitcount = 1;
	sum = ch - '0';
	while(isdigit(ch=mygetchar())){
		digitcount++;
		if (digitcount >= 8) {
			if (!tflags[9]) {
				fprintf(stderr,"the number input has too many decimal digits. Max is 7\n"); 
			}
			read_err = true;
			while(isdigit(ch)) ch = mygetchar();
			if (ch == '.') { 
				ch = mygetchar();
				while(isdigit(ch)) ch = mygetchar();
			}
			return(0); 
		}
		sum = sum*10 + ch - '0';
	}
	if (ch!='.') {  
		return(sum); 
	}
	cnt=10;
	ch=mygetchar();
	rsum=ch - '0';
	if (isdigit(ch)) {
		while (isdigit(ch=mygetchar())) {
			rsum = rsum*10 + ch -'0';
			cnt=cnt*10;
		}
	} 
	else { 
		return(sum); 
	}
	return((sum+(rsum/cnt)));
}

int
readword()
{
	char *p;
	int i;
	char buffer[120];
	int l;
	STRING ngc_strsave();

	p = buffer;
	for(*p++=ch; isalnum(*p=mygetchar()); p++);
	ch = *p;
	*p = NULL;
	for(i=0; i<wordquan; i++){
		if(!strcmp(wordtable[i],buffer)){
			break;
		}
	}
	if(i==wordquan){
		wordtable[i] = ngc_strsave(buffer);
		wordquan++;
	}
	return(i);
}

void killspace()
{       
	int mygetchar();
	if (ch == EOF ) return;
	while(isspace(ch)){
		ch = mygetchar();
		if (ch == EOF) return;
	}
}

STRING
strsave(s)
char *s; 
{
	int n;
	char *p, *calloc();
	n = strlen(s);
	if ((p = calloc(1,n+1))!=NULL)
		strcpy(p,s);
	return((STRING) p);
}

STRING
ngc_strsave(s)
char *s; 
{
	int n;
	char *p, *ngc_calloc();
	n = strlen(s);
	if ((p = ngc_calloc(n+1))!=NULL)
		strcpy(p,s);
	return((STRING) p);
}

char *ngc_calloc(n)
int n;
{      
	if (ngc_allocp +n <= ngc_allocbuf + ngc_allocsize)
	{  
		ngc_allocp += n;
		return(ngc_allocp-n);
	} 
	else return(NULL);
}
