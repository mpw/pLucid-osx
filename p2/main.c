
#include "cmanifs.h"
#include "cglobals.h"
STRING fname;
main(argc,argv)
int argc;
char ** argv ;
{       int i,temp;
	initialise();
	connect_file(argc,argv);
	temp=yyparse();
	if ( temp || errcount )
	 {
	   fprintf(stderr,"Fatal errors: no expression file written.\n");
	   my_exit(1);
	 }
}

accept()
{
}

initialise()
{
	int i;

	/* initialise the simple variables */

	errcount = 0;
	peekc = 0;
	cconst = false;
	false = 0;
	true = 1;
}

connect_file(argc,argv)
int argc;
char **argv;
{
	STRING calloc();
	STRING strsave();
	int i,j;
	extern FILE *lexin;

	/* connect to source file */
	if(argc>1){
		fname = argv[1];
		in_index = 0;
		for(j=0; fname[j]; j++);
		if(j<2 || fname[j-1]!='e' || fname[j-2]!='.'){
			fprintf(stderr,"%s: not a legal luthid source file\n",
				fname);
			my_exit(1);
		}
		if( (lexin=fopen(fname,"r")) == NULL ){
			fprintf(stderr,"cannot open %s\n",fname);
			my_exit(1);
		}
		in_files[in_index].in_name = strsave(fname);
		fname[j-1] = 'f';
	}else{
		in_files[in_index].in_name = "stdin";
		fname = "?.f";
		lexin = stdin;
	}
	savelex=lexin;
	in_files[in_index].in_line = 0;
	in_files[in_index].in_fdes = lexin;

   }

void output(p)
EXPRPTR p;
{
 EXPRPTR tmp;
 switch(p->f){
   case F_CONST: if (eqstring(p->arg1.s,"string")) {
				 fprintf(stdout," [ const [ string '%s` ] ",
					  p->arg2.s);
				 fprintf(stdout," ] ");
				 return; }
		 if (eqstring(p->arg1.s,"word")) {
				 fprintf(stdout," [ const [ word '%s` ] ",
					  p->arg2.s);
				 fprintf(stdout," ] ");
				 return; }
		 if (eqstring(p->arg1.s,"numb")) {
			  if (p->arg2.r<0) {
			  fprintf(stdout," [ const [ numb ~%-10.5f ] ",
					  -p->arg2.r); } else
			  fprintf(stdout," [ const [ numb %-10.5f ] ",
					  p->arg2.r);
				 fprintf(stdout," ] ");
				 return; }
		 fprintf(stdout," [ const [ special %s ] ",
					  p->arg2.s);
				 fprintf(stdout," ] ");
		 return;
   case F_VAR:   fprintf(stdout," [ var %s %d ",p->arg1.s,p->arg2.i);
		 if (p->arg3.x!=NULL) output(p->arg3.x);
			 fprintf(stdout," ] "); return;
   case F_OP:    fprintf(stdout,"[ op %s %d ",p->arg1.s,p->arg2.i);
		 if (p->arg3.x!=NULL) output(p->arg3.x);
		 output(p->arg4.x);
			 fprintf(stdout," ] "); return;
   case F_WHERE: fprintf(stdout," [ where ");
		 output(p->arg1.x);
		 output(p->arg2.x);
		 fprintf(stdout," ] ");
		 return;
   case F_DEFN:  fprintf(stdout," [ defn %s %d ",p->arg1.s,p->arg2.i);
		 if (p->arg2.i >0 ) output(p->arg3.x);
		 output(p->arg4.x);
		 fprintf(stdout," ] ");
		 return;
   case F_DECL:  fprintf(stdout," [ decl %s ",p->arg1.s);
		 output(p->arg2.x);
		 fprintf(stdout," ] ");
		 return;
   case F_IDENTLISTNODE: if (p->arg1.x==NULL) {
				  fprintf(stdout," %s ",p->arg2.s);
				  return; }
			 output(p->arg1.x);
			 fprintf(stdout," %s ",p->arg2.s);
			 return;
   case F_LISTNODE: if (p->arg1.x==NULL) {  output(p->arg2.x);
					    return;
					 }
			 output(p->arg1.x);
			 output(p->arg2.x);
			 return;
   case F_FILE:    fprintf(stdout," [ file '%s` %d %d %d ] ",p->arg1.s,
		      p->arg2.i,p->arg3.i,p->arg4.i);
		   return;
   default: fprintf(stderr,"UNKNOWN NODE IN PARSE TREE\n"); return;
 }
}

STRING
strsave(s)
char *s;
    {    char  *p;
	 STRING calloc();
	 if ( ( p = calloc(1,strlen(s)+1))==NULL)
	      fprintf(stderr,"ran out of space\n");
	      else strcpy(p,s);
	 return(p);

    }

eqstring(a,b)
STRING a,b;
{
	while( *a++ == *b++ ){
		if ( *a == '\0' && *b == '\0' ) {
			return(1);
		} else if (*a == '\0' || *b == '\0') break;
	}
	return(0);
}

my_exit(n)
int n;
{
    fprintf(stdout,"%c\n",'\032');
    exit(n);
}
