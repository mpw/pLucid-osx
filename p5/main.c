#include "cmanifs.h"
#include "cglobals.h"
#include "ftable.h"

STRING fname;

void echoexpr(EXPRPTR p);
void writeexpr(EXPRPTR p);

STRING
strsave(s)
char *s;
    {   int n;
	 STRING p, calloc();
	 n=strlen(s);
	  if ( ( p= calloc(1,n+1))==NULL)
	      fprintf(stderr,"ran out of space\n");
	    else
	    {
	     strcpy(p,s);
       }
	  return(p);

}



main(argc,argv)
int argc;
char ** argv;
{
	int temp;
	initialise(argc,argv);
	    temp=yyparse();
	if(temp||errcount){
		fprintf(stderr,"Fatal errors: no expression file written.\n");
		my_exit(1);
	}else{
		accept();
	}
}

initialise(argc,argv)
int argc;
char **argv;
{
	   STRING calloc();
	STRING strsave();
	int i, j;
	extern FILE *lexin;
	char *temp;

	/* connect to source file */
	if(argc>1){
		fname = argv[1];
		in_index = 0;
		for(j=0; fname[j]; j++);
		/*if(j<2 || fname[j-1]!='h' || fname[j-2]!='.'){
			fprintf(stderr,"%s: not a legal luthid source file\n",
				fname);
			my_exit(1);
		} */
		/* sprintf(fname,"%s.i",fname); */
                fname = malloc(strlen(fname)+3);
                sprintf(fname,"%s.i",argv[1]);
		/*if( (lexin=fopen(fname,"r")) == NULL ){
			fprintf(stderr,"cannot open %s\n",fname);
			my_exit(1);
		} */
		lexin = stdin;
		in_files[in_index].in_name = strsave(fname);
		/*fname[j-1] = 'i'; */
	}else{
		in_files[in_index].in_name = "stdin";
		fname = "stdin.i";
		lexin = stdin;
	}
	savelex=lexin;
	in_files[in_index].in_line = 0;
	in_files[in_index].in_fdes = lexin;

	/* hashtable is all NULLS */
	for(i=0;i<HASHSIZE;i++){
		hashtable[i] = NULL;
	}

	/* initialise various things */
	cconst=false;
	wordval=false;
	stringcount=0;
	wordcount=0;
	filecount=0;
	errcount = 0;
	lexlevel = 0;
	funclevel = 0;
	identcount = 0;
	idusage = (int) U_NORMAL;
	peekc = 0;

	/* predefined symbolic atoms */
	false = findword("false");
	true = findword("true");

	temp = calloc(1, sizeof(P_STACK));
	p_stack = (P_STACKPTR) temp;
	p_stack->p_idno = -1;
	p_stack->p_iselem = 0;
	p_stack->p_tl = NULL;

	parm_stack = (E_STACKPTR) calloc(1,sizeof(E_STACK));
	parm_stack->es_list = NULL;
	parm_stack->es_tl = NULL;

	f_stack = (F_STACKPTR) calloc(1,sizeof(F_STACK));
	f_stack->f_ll = -1;
	f_stack->f_el = -1;
	f_stack->f_tl = NULL;

	parm_list = (E_LISTPTR) calloc(1,sizeof(E_LIST));
	parm_list->el_expr = NULL;
	parm_list->el_tl = NULL;

}


accept()
{
	int i;
	char flag;
	flag = false;
	if((outfile=fopen(fname,"w"))==NULL){
		fprintf(stderr,"cannot create %s\n",fname);
		my_exit(1);
	}
	putw(identcount,outfile);
	if (flag)
	fprintf(stderr,"number of expressions is %d\n",identcount);
	for(i=0; i<identcount; i++){
		writename(nametable[i]);
		if (flag)
		{ fprintf(stderr,"%d : %s :", i , nametable[i]);
		  echoexpr(exprtable[i]);  fprintf(stderr,"\n");
		}
		writeexpr(exprtable[i]);
	}
	putw(wordcount,outfile);
	if (flag)
	fprintf(stderr,"number of words is %d\n",wordcount);
	for(i=0; i<wordcount; i++){
		putw(strlen(wordtable[i]),outfile);
		writename(wordtable[i]);
		if (flag)
		fprintf(stderr,"%d : `%s\'\n",i,wordtable[i]);
	}
	if (flag)
	fprintf(stderr,"Compilation complete\n");
}

writename(s)
STRING s;
{
	STRING p, calloc();
	for(p=s;*p!=NULL;p++){
		fputc(*p,outfile);
	}
	fputc('\n',outfile);
}

void writeexpr(p)
EXPRPTR p;
{
	FPTR q;
	int n,dim;
	if (p==NULL) { return; }
	putw(p->f,outfile);
	q = &ftable[p->f];
	n = q->nargs;
	switch(q->type){
	case INTERIOR:
		dim = p->dim;
		putw(dim,outfile);
		writeexpr(p->arg1.x);
		if(n>1) writeexpr(p->arg2.x);
		if(n>2) writeexpr(p->arg3.x);
		if(n>3) writeexpr(p->arg4.x);
		if(n>4) writeexpr(p->arg5.x);
		break;
	default:
		putw(p->arg1.i,outfile);
		if(n>1) putw(p->arg2.i,outfile);
		if(n>2) putw(p->arg3.i,outfile);
		if(n>3) putw(p->arg4.i,outfile);
		if(n>4) putw(p->arg5.i,outfile);
	}
}

void echoexpr(p)
EXPRPTR p;
{
	FPTR q;
	int n,dim;
	if (p==NULL) return;
	q = &ftable[p->f];
	n = q->nargs;
	switch(q->type){
	case INTERIOR:
		dim = p->dim;
		if (dim==0)
		fprintf(stderr,"%s(",q->name);
		else
		fprintf(stderr,"%s%d(,",q->name,dim);
		echoexpr(p->arg1.x);
		if(n>1){
			fprintf(stderr,", ");
			echoexpr(p->arg2.x);
		}
		if(n>2){
			fprintf(stderr,", ");
			echoexpr(p->arg3.x);
		}
		if(n>3){
			fprintf(stderr,", ");
			echoexpr(p->arg4.x);
		}
		if(n>4){
			fprintf(stderr,", ");
			echoexpr(p->arg5.x);
		}
		fprintf(stderr,")");
		break;
	default:
		fprintf(stderr,"%s( ",q->name);
		if ( p->f==F_CONST) {
		fprintf(stderr,"%f ",p->arg1.r); } else
		fprintf(stderr,"%d ",p->arg1.i);
		if(n>1) fprintf(stderr,", %d",p->arg2.i);
		if(n>2) fprintf(stderr,", %d",p->arg3.i);
		if(n>3) fprintf(stderr,", %d",p->arg4.i);
		if(n>4) fprintf(stderr,", %d",p->arg5.i);
		fprintf(stderr,")");
	}
}

my_exit(n)
int n;
{
    exit(n);
}
