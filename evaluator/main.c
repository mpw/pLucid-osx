#include "imanifs.h"
#include "iglobals.h"

main(argc,argv)
int argc;
char **argv;
{
	int i,t1,j,udc1,ldc1,udc2,ldc2,udc3,ldc3;
	COORDS xyz;
	extern rupture(),hiaton();
	char ok_to_print,dimension_err;
	memargc = argc;
	memargv = argv;

	/* first, set up files and read in the expression table */
	initialise(argc,argv);
	ok_to_print = !sflag && !pflag && isatty(fileno(stdout));

	if(dflag&&isatty(fileno(stdout))){
		for(i=0; i<exprquan; i++){
			fprintf(stderr,"%d: %s: ",i,nametable[i]);
			echoexpr(exprtable[i]);
			fprintf(stderr,"\n");
		}
	}

	/* get the result */
	if(isatty(fileno(stdout)))
		fprintf(stderr,"Evaluation begins .........\n");

	udc1=ldc1=0;
	udc2=ldc2=0;
	udc3=ldc3=0;
	i=0; t1=1000;
	dimension_err = true;
	D(13) while (dimension_err) 
	{ 
		if (!sflag) fprintf(stderr,
		"Ranges for t(t0,t1) x(n0,n1) y(m0,m1) z(o0,01)  respectively\n");
		scanf(" t(%d,%d) x(%d,%d) y(%d,%d) z(%d,%d)",&i,&t1,&ldc1,&udc1, 
		&ldc2,&udc2,
		&ldc3,&udc3);
		if ( ldc1>udc1||ldc2>udc2||ldc3>udc3){
			fprintf(stderr,"unaccaptable dimensions\n");
			dimension_err = true;  
		} 
		else 
		    dimension_err = false;
	}                
	if(isatty(fileno(stdout)))
	{ 
		if ( signal(SIGINT,SIG_IGN) != SIG_IGN)
			signal(SIGINT,rupture); 
	} 
	else
		signal(SIGINT,SIG_IGN);
	signal(SIGALRM,hiaton);
	while (true){ 
		c3= udc3;
		while(true){
			if (!sflag) D(13) {
				fprintf(stderr,"Slice|\n");
				fprintf(stderr,"%5d|",c3);
				for(j=ldc1;j<=udc1;j++)
					fprintf(stderr,"%10d ",j);
				fprintf(stderr,"\n");
				for(j=1; j<11*abs(udc1-ldc1)+17;j++)
					fprintf(stderr,"-");
				fprintf(stderr,"\n");
			}
			c2= udc2;
			while(true){
				if (!sflag) D(13) fprintf(stderr,"%5d|",c2);
				c1= ldc1;
				while(true){
					xyz.d1=c1;
					xyz.d2=c2;
					xyz.d3=c3;
					D(13) if (c1>udc1) { 
						break; 
					}
					elaborate(0,xyz,i);
					E(13) {
						if (((VALUEPTR)(&v_stack[1]))->v_type==
						    EOD) { 
							my_exit(1); 
						}
						if( !sflag) {
							printf("\n");
							fprintf(stderr,"output");
							dumpt(STPSt);
							fprintf(stderr," : "); 
						}
					}
					D(13) if (((VALUEPTR)(&v_stack[1]))->v_type
					    ==(long)EOD) break; 
					dumpval(stdout,&v_stack[1]);
					E(13) break;
					c1++;
				} 
				E(13) break;
/* newlines separating outp */	if(!sflag) printf("\n");
				if (((VALUEPTR)(&v_stack[1]))->v_type
				    ==(long)EOD&&c1==0) break; 
				if (c2-- <=ldc2) { 
					break; 
				}
			} 
			E(13) { 
				break; 
			}
			if (!sflag) {
				for(j=1; j<11*abs(udc1-ldc1)+17;j++)
					fprintf(stderr,"-");
				fprintf(stderr,"\n");
				fprintf(stderr," Time|");
				for(j=ldc1;j<=udc1;j++)
					fprintf(stderr,"%10d ",j);
				fprintf(stderr,"\n%5d|",i);
				fprintf(stderr,"\n\n\n");
			}
			if (((VALUEPTR)(&v_stack[1]))->v_type
			    ==(long)EOD&&c2==0) break; 
			if (c3-- <=ldc3) { 
				break; 
			}
		}
		D(13) { 
			if (((VALUEPTR)(&v_stack[1]))->v_type
			    ==(long)EOD&&c3==0) my_exit(1); 
		}
		i++;
if (i==t1) break;
		newout = true;
	}
}


initialise(argc,argv)
int argc;
char **argv;
{

	STRING strsave(),calloc();
	EXPRPTR readexpr();
	int i, num, k,nwords;
	STRING p,code_to_char();
	int  buffer[500];
	int flag;


	/*
			 *      set up environment. read in expressions, words, etc.
			 */
	pid = getpid();
	if(argc==1){
		usage();
	}
	for(i=0; i<TRACEQUAN; tflags[i++]=0);
	cflag = 0;
	flag  = 0;
	sflag = 0;
	dflag = 0;
	pflag = 0;
	infile = NULL;
	current_in = NULL;
	current_out = NULL;
	current_var = -1;
	current_inx = -1;
	current_outx = -1;
	notfoundcount = 0;
	errcount = 0;
	varcount = 0;
	stpcount = 0;
	memcount = 0;
	brecycle = 0;
	srecycle = 0;
	open_file = 3; /* stdin,stdout,stderr */
	ch = -2;
	for(i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			switch(argv[i][1]){
			case 't':
				num = atoi(&argv[i][2]);
				if( 0<=num && num<TRACEQUAN){
					tflags[num] = 1;
				}
				else{
					fprintf(stderr,"bad option %s\n",
					argv[i]);
					my_exit(-1);
				}
				break;
			case 'd':
				dflag = 1;
				break;
			case 'c':
				cflag = 1;
				break;
			case 's':
				sflag = 1;
				break;
			case 'p':
				pflag = 1;
				break;
			default:
				fprintf(stderr,"bad option %s\n",argv[i]);
				my_exit(1);
			}
		}
		else{
			if(testfile(argv[i])){
				infilename = argv[i];
				if((infile=fopen(argv[i],"r"))==NULL){
					fprintf(stderr,"cannot open %s\n",
					argv[i]);
					my_exit(1);
				}
				active_filters = i; 
				break;
			}
			else{
				if(argv[i]==NULL){
					fprintf(stderr,"silly");
					my_exit(2);
				}
				fprintf(stderr,"%s: not a legal lucid expression file\n",argv[i]);
				my_exit(1);
			}
		}
	}

	if(infile==NULL){
		fprintf(stderr,"no input file\n");
		my_exit(1);
	}
	for (i=0; i<MAXVARS; i++) io_info[i] = NULL;
	exprquan =  (unsigned) getw(infile);
	if (dflag)  fprintf(stderr,"\n%d expressions\n",exprquan);
	if ( exprquan > MAXVARS) { 
		fprintf(stderr,"program too large\n");
		my_exit(1); 
	}
	nametable = (STRING *) calloc(exprquan, sizeof(STRING));
	exprtable = (EXPRPTR *) calloc(exprquan, sizeof(EXPRPTR));
	for(i=0;i<exprquan;i++){
		for(p=(char *)buffer; (*p=fgetc(infile))!='\n'; p++);
		*p = NULL;
		nametable[i] = strsave(buffer);
		exprtable[i] = readexpr(i);
		if (dflag){ 
			fprintf(stderr,"%d : %s :",i,nametable[i]);
			echoexpr(exprtable[i]);
			fprintf(stderr,"\n");
		}
	}
	wordquan = (unsigned) getw(infile);
	if ( wordquan > MAXWORDS ) {
		fprintf(stderr,"too many words in compiled program\n");
		my_exit(1); 
	}
	wordtable = (STRING *) calloc(MAXWORDS, sizeof(STRING));
	for(i=0; i<MAXWORDS; wordtable[i++]=NULL);
	for(i=0; i<wordquan; i++){
		nwords = (unsigned) getw(infile);
		p = (char *) buffer;
		for(k=0; k < nwords; k++) { 
			*p = fgetc(infile); 
			p++; 
		};
		fgetc(infile);
		*p = NULL;
		wordtable[i] = strsave(buffer);
	}
	fclose(infile);
	ngc_allocsize=(MAXSTRINGS)*20+
	    (MAXWORDS-wordquan)*AVGWRDLEN+
	    MAXFILTERSPACE;
	ngc_allocbuf=(char *)calloc(ngc_allocsize,1);
	ngc_allocp = ngc_allocbuf;

	/* the memory is initially empty, too */
	for(i=0; i<NHASHSIZE; nhashtab[i++]=NULL);
	for(i=0; i<THASHSIZE; thashtab[i++]=NULL);
	for(i=0; i<PHASHSIZE; phashtab[i++]=NULL);
	for(i=0; i<SHASHSIZE; shashtab[i++]=NULL);

	/*
			 *      initialise run time stacks
			 */
	v_top = &v_stack[0];
	stp_top = &stp_stack[0];
	d_top = &d_stack[0];
	d_top->d_envg = NULL;
	d_top->d_envf = NULL;
	d_top->d_sf = NULL;
	d_top->d_sg = NULL;
	d_top->d_tg = NULL;
	d_top->d_tf = NULL;
	d_top->d_pg = NULL;
	d_top->d_pf = NULL;
	/*  dont buffer output */
	setbuf(stdout,(char *)NULL);
	setbuf(stdin,(char *)NULL);

	/* and, lets set up our dynamic allocation system */

	s_recquan=2000;
	b_recquan=2000;
	initspace();

	/* predefined words */
	false = 0;
	true = 1;
	newout = true;
	evalerr = false;
	filterlevel = 0;
	eodch = EOF;
	for (i=0; i<30; i++) filterp[i]=false;

}

testfile(p)
STRING p;
{
	int j;
	for(j=0; p[j]; j++);
	return(j>2 && p[j-1]=='i' && p[j-2]=='.');
}


EXPRPTR
readexpr(i)
int i;
{
	int n,temp;
	EXPRPTR new;
	float getfloat();
	FPTR q;

	temp = (FUNCTION) getw(infile);
	q = &ftable[temp];
	n = q->nargs;
	if ( temp ==F_FILTER ) n++;
	switch(n){
	case 1:
		new = (EXPRPTR) calloc(1, sizeof(U_EXPR));
		break;
	case 2:
		new = (EXPRPTR) calloc(1, sizeof(B_EXPR));
		break;
	case 3:
		new = (EXPRPTR) calloc(1, sizeof(T_EXPR));
		break;
	case 4:
		new = (EXPRPTR) calloc(1, sizeof(Q_EXPR));
		break;
	case 5:
		new = (EXPRPTR) calloc(1, sizeof(QU_EXPR));
		break;
	default:
		fprintf(stderr,"error: illegal arg count\n");
		my_exit(1);
	}
	new->f = temp;
	switch(q->type){
	case TERMINAL:
		new->arg1.i = getw(infile);
		if(n>1) new->arg2.i = getw(infile);
		if(n>2) new->arg3.i = getw(infile);
		if(n>3) new->arg4.i = getw(infile);
		if(n>4) new->arg5.i = getw(infile);
		break;
	case INTERIOR:
		new->dim = (int)getw(infile);
		new->arg1.x = readexpr(i);
		if(n>1) new->arg2.x = readexpr(i);
		if(n>2) new->arg3.x = readexpr(i);
		if(n>3) new->arg4.x = readexpr(i);
		if(n>4) new->arg5.i=i;
		break;
	default:
		fprintf(stderr,"error: illegal type of fnode\n");
		fprintf(stderr,"%s is fvalue\n",q->name);
		my_exit(1);
	}
	return(new);
}


elaborate(n,xyz,e_time)
register int n, e_time;
COORDS xyz;
{
	STPPTR ssearch(),stpsearch();
	rSTPPTR s,t;
	outside = t = stpsearch((long)e_time,
	(STPPTR)NULL,thashtab);
	s = ssearch(xyz);
	d_top = &d_stack[0];
	VStop = &v_stack[0];
	VSvalue.na.r = 0;
	VSvalue.na.i = 0;
	VStype = 0;
	STPStop = &stp_stack[0];
	STPSs = s;
	STPSt = t;
	STPSp = NULL;
	(*ftable[exprtable[n]->f].apply)(exprtable[n]);
}

usage()
{
	fprintf(stderr,"usage: fluval [-d] [-c] [-s] [-t?] <fname.i>\n");
	my_exit(1);
}

echoexpr(p)
EXPRPTR p;
{
	FPTR q;
	int n,dim;
	q = &ftable[p->f];
	n = q->nargs;
	switch(q->type){
	case TERMINAL:
		if (p->f==F_CONST) 
			fprintf(stderr,"%s(%f",q->name,p->arg1.c.r);
		else {  if (p->dim==0) 
			fprintf(stderr,"%s(%d",q->name,p->arg1.i);
		        else 
		fprintf(stderr,"%s%d(%d",q->name,p->dim,p->arg1.i);
	        	}	
		if(n>1) fprintf(stderr,", %d",p->arg2.i);
		if(n>2) fprintf(stderr,", %d",p->arg3.i);
		if(n>3) fprintf(stderr,", %d",p->arg4.i);
		if(n>4) fprintf(stderr,", %d",p->arg5.i);
		fprintf(stderr,")");
		break;
	case INTERIOR:
		if (p->dim==0) { fprintf(stderr,"%s(",q->name); }
		else{ fprintf(stderr,"%s%d(",q->name,p->dim); }
		echoexpr(p->arg1.x);
		if(n>1){
			echoexpr(p->arg2.x);
		}
		if(n>2){
			echoexpr(p->arg3.x);
		}
		if(n>3){
			echoexpr(p->arg4.x);
		}
		if(n>4){
			echoexpr(p->arg5.x);
		}
		fprintf(stderr,")");
	}
}
