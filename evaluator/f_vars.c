#include "imanifs.h"
#include "iglobals.h"

#define evalname(x) (*ftable[exprtable[x]->f].apply)(exprtable[x])
#define INMEMORY(x) memsearch(&memplace,x,s,t,p)


dumpname(m,n)
MEMPTR m;
int n;
{  
	ddepth--;
	bar(ddepth);
	fprintf(stderr,"=%s",nametable[n]);
	dumps(m->s); 
	dumpstp(m->t); 
	dumpstp(m->p);
	fprintf(stderr," ");
	dumpval2(stderr,m->v);
	fprintf(stderr,"\n");
}
/*
 *      given a var node, evalaute the proper variable.
 *      until it is determined what types of things must be
 *      done in different situations, most variable
 *      requests are handled with this one node.
 *      Later they may be broken up for efficiency
 */

f_var(e)
EXPRPTR e;
{       
	varcount++;
	if(e->arg3.i==0){
		/* thru no functions */
		f_var1(e);
	}
	else if(e->arg1.i<0){
		/* thru functions to a formal */
		f_var2(e);
	}
	else{
		/* thru functions to a local */
		f_var3(e);
	}
}


/*
 *      the simple case where the variable being requested is defined
 *      in the same function environment that it is being requested from
 */
f_var1(e)
EXPRPTR e;
{
	MEMPTR memplace;
	STPPTR stpsearch(),ssearch();
	rSTPPTR s, t, p;
	register int i,toptime, name;
	register COORDS topspace;
	s = STPSs;
	t = STPSt;
	p = STPSp;

	if(e->arg2.i<0){
		toptime = t->stphd.word;
		topspace = s->stphd.xyz;
		for(i=1; i<-e->arg2.i; i++){
			t = t->stptl;
		}
		t = stpsearch((long)toptime,t->stptl,thashtab);
		s = ssearch(topspace);
	}
	else{
		for(i=1; i<e->arg2.i; i++){
			t = t->stptl;
		}
	}

	STPSpushval(s,t,p);
	name = e->arg1.i;
	if( !INMEMORY(name) ){
		evalname(name);
		notfoundcount++;
		if (VStype==UNDEFINED) fprintf(stderr,"undefined\n");
		copy(memplace->v, *VStop);
		D(30) { 
			dumpname(memplace,name); 
		}
		D(6) {
			fprintf(stderr,"Defining ");
			dumpmem(memplace);
		}
	}
	else{
		VSpush;
		copy(*VStop,memplace->v);
	}
	STPSpop;
}



/*
 *      the variable being accessed is the formal parameter of
 *      a function that is either the function we are in now, or
 *      is a function containing the function we are in now
 */
f_var2(e)
EXPRPTR e;
{
	MEMPTR memplace;
	STPPTR stpsearch(),ssearch();
	rSTPPTR s, t, p;
	register int i , toptime, name;
	register COORDS topspace;
	register DISPLAYPTR env;
	toptime = STPSt->stphd.word;
	topspace = STPSs->stphd.xyz;
	env = d_top;
	for(i=1; i<e->arg3.i; i++){
		env = env->d_envg;
	}
	/*
			 *      "name" is offset in "exprtable" where expression for
			 *      function is stored.
			 */
	name = env->d_pf->stphd.word - e->arg1.i - 1;

	if( env->d_frozen >= 0 )   toptime = env->d_frozen;
	s = env->d_sf; 
	t = env->d_tf;
	p = env->d_pf->stptl;
	env = env->d_envf;
	t = e->arg2.i>0 ?t :stpsearch((long)toptime,t->stptl,thashtab);
	s = e->arg2.i>0 ?s :ssearch(topspace);
	STPSpushval(s,t,p);
	if (d_top+1 >= &d_stack[0]+4*DSIZE) {
		fprintf(stderr,"d_stack overflow\n"); 
		my_exit(1); 
	} 
	else
		d_top++;
	copy(*d_top, *env);
	if( !INMEMORY(name) ){
		evalname(name);
		notfoundcount++;
		if (VStype==UNDEFINED) fprintf(stderr,"undefined\n");
		copy(memplace->v,*VStop);
		D(30) { 
			dumpname(memplace,name); 
		}
		D(6) {
			fprintf(stderr,"Defining ");
			dumpmem(memplace);
		}
	}
	else{
		VSpush;
		copy(*VStop,memplace->v);
	}
	stp_top--;
	d_top--;
}


/*
 *      the variable being accessed is a defined as a local variable in
 *      some function environment containing the one we are presently in
 */
f_var3(e)
EXPRPTR e;
{
	MEMPTR  memplace;
	STPPTR stpsearch(),ssearch();
	rSTPPTR s, t, p;
	register int toptime, i, name;
	register COORDS topspace;
	register DISPLAYPTR env;

	env = d_top;
	toptime = stp_top->stp_t->stphd.word;
	topspace = stp_top->stp_s->stphd.xyz;

	for(i=1; i<e->arg3.i; i++){
		env = env->d_envg;
	}
	if (d_top+1 >= &d_stack[0]+4*DSIZE) {
		fprintf(stderr,"d_stack overflow\n"); 
		my_exit(1); 
	} 
	else
		d_top++;
	copy(*d_top,*env->d_envg);
	s = env->d_sg;
	p = env->d_pg;
	t = env->d_tg;
	if(e->arg2.i<0){
		for(i=1; i<-e->arg2.i; i++){
			t = t->stptl;
		}
		t = stpsearch((long) toptime,t->stptl,thashtab);
		s = ssearch(topspace);
	}
	else{
		for(i=1; i<e->arg2.i; i++){
			t = t->stptl;
		}
	}
	if (stp_top+1 >= &stp_stack[0]+STPSIZE) {
		fprintf(stderr,"stp_stack overflow\n"); 
		my_exit(1); 
	} 
	else
		stp_top++;
	stp_top->stp_t = t;
	stp_top->stp_p = p;
	stp_top->stp_s = s;
	name = e->arg1.i;
	if( !INMEMORY(name) ){
		evalname(name);
		notfoundcount++;
		if (VStype==UNDEFINED) fprintf(stderr,"undefined\n");
		copy(memplace->v,*VStop);
		D(30) { 
			dumpname(memplace,name); 
		}
		D(6) {
			fprintf(stderr,"Defining ");
			dumpmem(memplace);
		}
	}
	else{
		VSpush;
		copy(*VStop,memplace->v);
	}
	stp_top--;
	d_top--;
}


/*
 *      a variable of the same time and place as the current environment
 *      is being requested. this case require no changes on the stacks
 */
f_local(e)
EXPRPTR e;
{
	MEMPTR memplace;
	STPPTR stpsearch();
	rSTPPTR s, t, p;
	varcount++;
	t = STPSt;
	p = STPSp;
	s = STPSs;
	if( !INMEMORY(e->arg1.i) ){
		evalname(e->arg1.i);
		notfoundcount++;
		copy(memplace->v,*VStop);
		D(30) { 
			dumpname(memplace,e->arg1.i); 
		}
		D(6) {
			fprintf(stderr,"Defining ");
			dumpmem(memplace);
		}
	}
	else{
		VSpush;
		copy(*VStop,memplace->v);
	}
}




/*
 *      here is what the parameters mean:
 *      arg1    name of the function invoked
 *      arg2    number of time levels to pop.
 *              (times -1 if nonelementary function)
 *      arg3    number of place levels to pop
 *      arg4    address of actual parameters to substitute
	arg5    # of parameters
 */
f_fcall(e)
EXPRPTR e;
{
	register int i;
	register DISPLAYPTR env;
	STPPTR stpsearch();
	register STPPTR s, t, p;
	varcount++;
	t = stp_top->stp_t;
	s = stp_top->stp_s;
	p = stpsearch((long)e->arg4.i, stp_top->stp_p, phashtab);
	if (stp_top+1 >= &stp_stack[0]+STPSIZE) {
		fprintf(stderr,"stp_stack overflow\n"); 
		my_exit(1); 
	} 
	else
		stp_top++;
	stp_top->stp_t = t;
	stp_top->stp_s = s;
	stp_top->stp_p = p;
	env = d_top;
	if (d_top+1 >= &d_stack[0]+4*DSIZE) {
		fprintf(stderr,"d_stack overflow\n"); 
		my_exit(1); 
	} 
	else
		d_top++;
	/*
			 *      first, where are the parameters of F defined?
			 */
	d_top->d_tf = t; /*stp_top->stp_t;*/
	d_top->d_sf = s; /* stp_top->stp_s;*/
	d_top->d_pf = p;
	d_top->d_envf = d_top-1;

	/*
			 *      now, where is F itself defined
			 *      it is inherited thru N levels, so we
			 *      go back N envirnoments and where the first function
			 *      it was inherited into was defined.
			 *      We then pop off a few times, as required
			 */
	if(e->arg3.i==0){
		/*
						 * function not inherited at all!!
						 */
		d_top->d_envg = env;
		d_top->d_pg = stp_top->stp_p->stptl;
		t = stp_top->stp_t;
		s = stp_top->stp_s;
	}
	else{
		for(i=1; i<e->arg3.i; i++){
			env = env->d_envg;
		}
		d_top->d_envg = env->d_envg;
		d_top->d_pg = env->d_pg;
		t = env->d_tg;
		s = env->d_sg;
	}
	if( e->arg2.i > 0 ){
		/* elementary */
		for(i=1; i< e->arg2.i; i++){
			t = t->stptl;
		}
		d_top->d_frozen = t->stphd.word; /* time frozen at */
		stp_top->stp_t =stpsearch((long)t->stphd.word,
		stp_top->stp_t->stptl,thashtab);

	}
	else{
		/* nonelemntary */
		d_top->d_frozen = -1;
		for(i=1; i< -e->arg2.i; i++){
			t = t->stptl;
		}
	}
	d_top->d_tg = t;
	d_top->d_sg = s;

	/*
			 *      call the function
			 */
	D(5) display();
	e = exprtable[e->arg1.i];
	(*ftable[e->f].apply)(e);

	/*
			 *      restore old environment
			 */
	D(5) fprintf(stderr,"Pop Display\n");
	stp_top--;
	d_top--;
}



f_eres(e)
EXPRPTR e;
{
	MEMPTR memplace;
	STPPTR stpsearch();
	rSTPPTR s, t, p;
	varcount++;
	t = stpsearch((long)STPSt->stphd.word,STPSt,thashtab);
	p = STPSp;
	s = STPSs;
	STPSpushval(s,t,p);
	if( !INMEMORY(e->arg1.i) ){
		evalname(e->arg1.i);
		notfoundcount++;
		copy(memplace->v,*VStop);
		D(30) { 
			dumpname(memplace,e->arg1.i); 
		}
		D(6) {
			fprintf(stderr,"Defining ");
			dumpmem(memplace);
		}
	}
	else{
		VSpush;
		copy(*VStop,memplace->v);
	}
	/*now, get rid of all shit that isnt needed */
	D(26) rm_eres(t->stptl);



	STPSpop;
}


display()
{
	fprintf(stderr,"Push Display:\n");
	fprintf(stderr,"tf:"); 
	dumpstp(d_top->d_tf);
	fprintf(stderr,"pf:"); 
	dumpstp(d_top->d_pf);
	fprintf(stderr,"\n");
	fprintf(stderr,"tg:"); 
	dumpstp(d_top->d_tg);
	fprintf(stderr,"pg:"); 
	dumpstp(d_top->d_pg);
	fprintf(stderr,"\n");
}
