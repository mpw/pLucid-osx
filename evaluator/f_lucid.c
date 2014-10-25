#include "imanifs.h"
#include "iglobals.h"

IOPTR
get_io_item(type,e,space,time,place)
int type;
EXPRPTR e;
STPPTR space,time,place;
{ 
	char *ngc_calloc();
	IOPTR s;
	s = (IOPTR) ngc_calloc(16*4);
	if (s==NULL) { 
		fprintf(stderr,"\nngc space exhausted\n");
		my_exit(1); 
	}
	s->type  = type;
	s->expr  = e;
	s->s     = space;
	s->t     = time;
	s->p     = place;
	s->out   = NULL;
	s->in    = NULL;
	if (type==INPUT)
		s->in    = stdin;
	s->chin  = ' ';
	s->chout = ' ';
	s->inx   = 0;
	s->outx  = 0;
	s->inmode = POP;
	s->outmode = POP;
	s->ignore  = false;
	s->ptwize  = false;
	s->next  = NULL;
	return(s);
}

f_input(e)
rEXPRPTR e;

{
	MEMPTR memplace;
	int i;
	STPPTR t1,stpsearch();
	int savefilter;
	IOPTR get_io_item();

	savefilter           = filterp[filterlevel];
	filterp[filterlevel] = false;
	if (io_info[e->arg1.i]==NULL)
		io_info[e->arg1.i]   = get_io_item(INPUT,e,NULL,NULL,NULL);
	current_var          = e->arg1.i;
	current_io           = io_info[e->arg1.i];
	ch                   = current_io->chin;
	varcount++;
	if (isatty(fileno(current_io->in))) {

		(void)memsearch(&memplace,e->arg1.i,STPSs,STPSt,STPSp);
		if( memplace->v.v_type==UNDEFINED )
		{
			if (!sflag){
				fprintf(stderr,"\n%7s",nametable[e->arg1.i]);
				D(13) dumps(STPSs);
				dumpt(STPSt);
				fprintf(stderr," : ");
			}
			if (cflag) {
				memplace->v.v_type = read_c_item(&memplace->v.v_value);
			}
			else
				memplace->v.v_type = read_p_item(&memplace->v.v_value);
		}
	} 
	else 
	    for (i = current_io->inx; i<=STPSt->stphd.word; i++){
		t1 = stpsearch(i,STPSt->stptl,thashtab);
		(void)memsearch(&memplace,e->arg1.i,STPSs,t1,STPSp);
		if( memplace->v.v_type==UNDEFINED )
		{
			notfoundcount++;
			if ( ch != EOF) {
				if (cflag) {
					memplace->v.v_type = read_c_item(&memplace->v.v_value);
				}
				else
					memplace->v.v_type = read_p_item(&memplace->v.v_value);
			} 
			else memplace->v.v_type = EOD;

		}
	}

	current_io->inx      = STPSt->stphd.word+1;
	current_io->chin     = ch;
	filterp[filterlevel] = savefilter;

	VSpush;
	copy(*VStop,memplace->v);
}


void f_filter(e)
rEXPRPTR e;

{
	MEMPTR memplace;
	VALUE x;
	int i,iox,ii;
	STPPTR t1,stpsearch(),s,t,p,s1,p1,ss,tt,pp;
	int pipe_a[2],pipe_b[2];
	int *iostring,found,sx[100],ops[20];
	STRING formstring();
	FILE *tin,*tout;
	int tinx,tch,save_ch,savei;
	IOPTR m,get_io_item(),save_io;
	s = STPSs;
	t = STPSt;
	p = STPSp;
	filterlevel++;
	varcount++;
	filterp[filterlevel]=true;
	found = 0;
	for(m=io_info[e->arg5.i];m!=NULL;m=m->next)
		if (p==m->p) { 
			found=1; 
			break; 
		}
	if ( !found )
	{ 
		eval(arg1.x);
		switch(VStype){
		case QSTRING:     
			formstring(&sx[0],VSvalue.strg);
			VSpop;
			m = get_io_item(IOFILTER,e,s,t->stptl,p);
			m->next=io_info[e->arg5.i];
			io_info[e->arg5.i] = m;
			eval(arg3.x);
			switch(VStype)
			{ 
			case QSTRING: 
				formstring(&ops[0],VSvalue.strg);
				for(iostring=ops;
				      *iostring!='\0'; *iostring++)
					    switch(iostring[0])
					{ 
					case 'i': 
						m->ignore = true;
						break;
					case 'p': 
						m->ptwize = true;
						break;
					case 'c': 
						m->inmode = UNIX;
						break;
					case 's': 
						m->outmode = UNIX;
						break;
					}
				VSpop;
				break;
			case EOD: 
				m->chin = EOF;
				VStype = EOD;
				return;
			default: 
				error("3rd arg of filter must be a string not ",e->arg4.x,VStype,VSvalue);
				m->chin = '?'; /* not enough */
				VStype = ERROR;
				return;
			}

			if ((pipe(pipe_a) == SYSERR) ||
			    (pipe(pipe_b) == SYSERR) ) {
				perror(sx);
				my_exit(1); 
			}
			switch(fork()){
			case SYSERR:
				perror(sx);
				my_exit(1);
			case CHILD:
				signal(SIGINT,SIG_IGN);
				signal(SIGALRM,SIG_IGN);
				dup2(pipe_a[READ_END],CHILD_IN);
				dup2(pipe_b[WRITE_END],CHILD_OUT);
				close(pipe_a[WRITE_END]);
				close(pipe_b[WRITE_END]);
				close(pipe_a[READ_END]);
				close(pipe_b[READ_END]);
				execlp("/bin/sh","sh","-c",
				sx,0);
				perror(sx);
				my_exit(1);
			}
			m->out = fdopen(pipe_a[WRITE_END],"w");
			m->in = fdopen(pipe_b[READ_END],"r");
			setbuf(m->in,(char *)NULL);
			setbuf(m->out,(char *)NULL);
			close(pipe_b[WRITE_END]);
			close(pipe_a[READ_END]);
			ch = m->chin;
			break;
		case EOD:
			VStype = EOD;
			return;
		default: 
			error("1st arg of filter must be a string, not ",e->arg4.x,
			VStype,VSvalue);
			VStype = ERROR;
			return;
		}
	}
	ch = m->chin;
	current_io = m;
	for (i = current_io->inx; i<=STPSt->stphd.word; i++){
		s1 = stpsearch(0,NULL,shashtab);
		t1 = stpsearch(i,STPSt->stptl,thashtab);
		(void)memsearch(&memplace,e->arg5.i,s1,t1,STPSp);
		if( memplace->v.v_type==UNDEFINED ) {
			if (current_io->ptwize )
			{ 
				tt = STPSt;
				pp = STPSp;
				ss = STPSs;
				STPSpush;
				STPSp=pp;
				STPSs=ss;
				save_io = current_io;
				STPSt = stpsearch((long)i,tt->stptl,thashtab);
				eval(arg2.x);
				current_io = save_io;
				x.v_type = VStype;
				x.v_value = VSvalue;
				dumpval2(current_io->out,x);
				if (current_io->outmode==POP)
					fprintf(current_io->out,"\n");
				VSpop;
				STPSpop;
			}
			if (current_io->inmode==UNIX) {
				memplace->v.v_type =
				    read_c_item(&memplace->v.v_value);
			}
			else
				memplace->v.v_type =
				    read_p_item(&memplace->v.v_value);
		}
	}
	current_io->inx  = STPSt->stphd.word+1;
	ch = current_io->chin;
	VSpush;
	copy(*VStop,memplace->v);
	filterp[filterlevel]=false;
	filterlevel--;
}


f_now(e)
rEXPRPTR e;
{  
	int time;
	time = STPSt->stphd.word;
	VSpush;
	VStype = NUMERIC;
	VSvalue.na.r = time;
}


f_first(e)
rEXPRPTR e;
{
	rSTPPTR s, t, p;
	STPPTR stpsearch();
	s = STPSs;
	t = STPSt;
	p = STPSp;
	t = stpsearch((long) 0,t->stptl,thashtab);
	STPSpushval(s,t,p);
	eval(arg1.x);
	STPSpop;
}


f_prev(e)
rEXPRPTR e;
{
	rSTPPTR  s, t, p;
	STPPTR stpsearch();
	s = STPSs;
	t = STPSt;
	p = STPSp;
	t = stpsearch(t->stphd.word-1,t->stptl,thashtab);
	STPSpushval(s,t,p);
	eval(arg1.x);
	STPSpop;
}

f_next(e)
rEXPRPTR e;
{
	rSTPPTR  s, t, p;
	STPPTR stpsearch();
	s = STPSs;
	t = STPSt;
	p = STPSp;
	t = stpsearch(t->stphd.word+1,t->stptl,thashtab);
	STPSpushval(s,t,p);
	eval(arg1.x);
	STPSpop;
}

f_before(e)
rEXPRPTR e;
{
	STPPTR stpsearch();
	rSTPPTR s, t, p;

	s = STPSs;
	p = STPSp;
	t = STPSt;
	if(t->stphd.word>=0){
		eval(arg2.x);
	}
	else{
		t = stpsearch(t->stphd.word+1,t->stptl,thashtab);
		STPSpushval(s,t,p);
		eval(arg1.x);
		STPSpop;
	}
}

f_fby(e)
rEXPRPTR e;
{
	STPPTR stpsearch();
	rSTPPTR s, t, p;

	s = STPSs;
	p = STPSp;
	t = STPSt;
	if(t->stphd.word<=0){
		eval(arg1.x);
	}
	else{
		t = stpsearch(t->stphd.word-1,t->stptl,thashtab);
		STPSpushval(s,t,p);
		eval(arg2.x);
		STPSpop;
	}
}


void f_asa(e)
EXPRPTR e;
{
	STPPTR stpsearch();
	rSTPPTR s, t, p;
	register int i;

	s = STPSs;
	p = STPSp;
	t = STPSt;
	STPSpush;
	STPSs = s;
	STPSp = p;
	for(i=0;;i++){
		STPSt = stpsearch((long)i,t->stptl,thashtab);
		eval(arg2.x);
		D(30) { 
			fprintf(stderr," asa\n"); 
		}
		switch(VStype){
		case WORD:  
			if(VSvalue.wrd==true){
				VSpop;
				eval(arg1.x);
				STPSpop;
				return;
			}
			VSpop;
			break;
		case ERROR:
			error("right arg of asa is ",e->arg3.x,
			ERROR,VSvalue);
			VStype = ERROR;
			STPSpop;
			return;
		case EOD:
			VStype = EOD;
			STPSpop;
			return;
		default:    
			error("right arg of asa must be logical, not ",e->arg3.x,VStype,VSvalue);
			STPSpop;
			VStype = ERROR;
			return;
		}
	}
}

void f_attime(e)
rEXPRPTR e;
{  
	int type;
	CELLUNION value;
	rSTPPTR s,t,p;
	STPPTR stpsearch();
	s = STPSs;
	t = STPSt;
	p = STPSp;
	eval(arg2.x);
	D(30) { 
		fprintf(stderr,"attime \n"); 
	}
	switch(VStype){
	case NUMERIC: 
		t = stpsearch((long)VSvalue.na.r,t->stptl,thashtab);
		STPSpushval(s,t,p);
		eval(arg1.x);
		type = VStype;
		value = VSvalue;
		switch(VStype){
		case ERROR: 
			error("left arg of attime \nor 1st arg of wvr or upon is ",
			e->arg3.x,VStype,VSvalue);
		default:    
			break;
		}
		STPSpop;
		VSpop;
		VStype = type;
		VSvalue = value;
		return;
	case EOD:     
		VStype = EOD;
		return;
	case ERROR:   
		error("right arg of attime \nor condition of wvr or upon is ",e->arg3.x,ERROR,VSvalue);
		return;
	default:      
		error("right arg of attime must be numeric, not ",
		e->arg3.x,VStype,VSvalue);
		VStype = ERROR;
		return;
	}
}
