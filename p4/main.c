
#include "cmanifs.h"
#include "cglobals.h"

STRING fname,oldname;

printfilter(newname)
STRING newname;
{ int i;
}

main(argc,argv)
int argc;
char ** argv ;
{       int i,temp;
	initialise();
	connect_file(argc,argv);
	/*if ((outfile=fopen(fname,"w")) == NULL) {
	fprintf(stderr,"cannot create %s\n",fname);
	 my_exit(1); } */
	printfilter(fname);
	temp=yyparse();
	if ( temp || errcount )
	 {
	   fprintf(stderr,"Fatal errors: no expression file written.\n");
	   my_exit(1);
	 }
}

accept()
{
    /*if ((outfile=fopen(fname,"w")) == NULL) {
    fprintf(stderr,"cannot create %s\n",fname);
    my_exit(1); } */
}

initialise()
{
	int i;

	/* initialise the simple variables */
	currentlevel=0;
	funclevel=0;
	valoflevel=0;
	fflevel=0;
	ffcount=0;
	errcount = 0;
	peekc = 0;
	cconst = false;
	false = 0;
	true = 1;

	largest = '\0';

	for(i=0; i<100; i++){ new_decls[i]=NULL;
			      new_defs[i] =NULL;
			      defined_list[i]=NULL;
			      used_list[i]=NULL;
			      formals_list[i]=NULL;
			      f_level[i]=0;
			      new_def[i]=NULL;
			    }
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
		oldname = strsave(fname);
		in_index = 0;
		for(j=0; fname[j]; j++);
		if(j<2 || fname[j-1]!='g' || fname[j-2]!='.'){
			fprintf(stderr,"%s:filename ending in .g expected\n",
				fname);
			my_exit(1);
		}
		if( (lexin=fopen(fname,"r")) == NULL ){
			fprintf(stderr,"cannot open %s\n",fname);
			my_exit(1);
		}
		in_files[in_index].in_name = strsave(fname);
		fname[j-1] = 'h';
	}else{
		in_files[in_index].in_name = "stdin";
		fname = "?.h";
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
				 fprintf(stdout," [ const [ strg `%s' ] ",
					  p->arg2.s);
				 fprintf(stdout," ] ");
				 return; }
		 if (eqstring(p->arg1.s,"word")) {
				 fprintf(stdout," [ const [ wrd `%s' ] ",
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
		 fprintf(stdout," [ const [ %s ] ",
					  p->arg2.s);
				 fprintf(stdout," ] ");
		 return;
   case F_VAR:   if (p->arg2.i==0) {
		  fprintf(stdout," [ nullry %s %d ",p->arg1.s,p->arg2.i);
		  if (p->arg3.x!=NULL) output(p->arg3.x);
			  fprintf(stdout," ] "); return; }
		 fprintf(stdout," [ nonnullry %s %d ",p->arg1.s,p->arg2.i);
		 if (p->arg3.x!=NULL) output(p->arg3.x);
			 fprintf(stdout," ] "); return;
   case F_OP:    fprintf(stdout,"[ op [ %s %d ",p->arg1.s,p->arg2.i);
		 if (p->arg3.x!=NULL) output(p->arg3.x);
		 fprintf(stdout," ] ");
		 output(p->arg4.x);
		 fprintf(stdout," ] "); return;
   case F_EVALOF: fprintf(stdout," [ evalof ");
		 output(p->arg1.x);
		 fprintf(stdout," ] ");
		 return;
   case F_VALOF: fprintf(stdout," [ valof ");
		 output(p->arg1.x);
		 fprintf(stdout," ] ");
		 return;
   case F_DEFN:  fprintf(stdout," [ defn %s %d ",p->arg1.s,p->arg2.i);
		 if (p->arg2.i >0 ) { fprintf(stdout," [ frmls ");
				      output(p->arg3.x);
				      fprintf(stdout," ] "); }
		 output(p->arg4.x);
		 fprintf(stdout," ] ");
		 return;
   case F_DECL:  fprintf(stdout," [ decl %s ",p->arg1.s);
		 output(p->arg2.x);
		 fprintf(stdout," ] ");
		 return;
   case F_EGLOBALS:  fprintf(stdout," [ eglobal ");
		 output(p->arg1.x); fprintf(stdout," ] ");
		 return;
   case F_NGLOBALS:  fprintf(stdout," [ nglobal ");
		 output(p->arg1.x); fprintf(stdout," ] ");
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
   case F_FILE:    fprintf(stdout," [ cxfile `%s' %d %d %d ] ",p->arg1.s,
		      p->arg2.i,p->arg3.i,p->arg4.i);
		   return;
   default: fprintf(stderr,"UNKNOWN NODE IN PARSE TREE#%d#\n",p->f);
	    return;
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
