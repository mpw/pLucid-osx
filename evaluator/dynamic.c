

#include "imanifs.h"
#include "iglobals.h"

#define needed(x) (((*x)->data.bits.g_mark)<= retvec[(*x)->data.bits.n])

SPACEITEM bspace, espace; /* these describe the memory space */
SPACEITEM unused;         /* unused space pointer            */
SPACEITEM free_smalls;    /* small records pointer           */
SPACEITEM free_bigs;      /* big   records pointer           */
int used;                 /* count of amount of space used   */
int marking;
int sfound,bfound;

void markdp(CELLPTR p);
void markstrg(CELLPTR p);

initspace()
{       
	int i,rage;
	char  *sbrk(), *morefrees();
	dynasizes[0] = 24;
	dynasizes[1] = 36;
	rage = 1;
	D(25) rage = 1000;
	for (i=0; i<exprquan; i++)
	{ 
		if (exprtable[i]->f == F_INPUT || exprtable[i]->f == F_FILTER )
		{ 
			retvec[i] = 1000;
		} 
		else retvec[i]=rage; 
		u_countvec[i]=0;
	}
	bspace.sp = espace.sp = unused.sp = sbrk(0);
	used = 0;
	/* start out with a few little records and a few big records */
	s_count = s_recquan;
	b_count = b_recquan;
	free_smalls.sp =  morefrees(s_recquan,SMALL_RECORD);
	free_bigs.sp =  morefrees(b_recquan,BIG_RECORD);
}

char *
alloc(size)
int size;
{
	SPACEITEM x;
	SPACEITEM *freeptr, newptr;
	char *morefrees();
	int rec_count,r_found,recquan;

	freeptr = size==BIG_RECORD ?&(free_bigs) :&(free_smalls);
	if( (*freeptr).sp == NULL){
		/*
								 attempt to get some stuff back
									       */
		bfound = sfound = 0;
		E(10) collect();
		recquan= size==BIG_RECORD? b_recquan:s_recquan;
		r_found = size == BIG_RECORD?bfound :sfound;
		rec_count=size == BIG_RECORD?b_count:s_count;
		D(17) { 
			fprintf(stderr,"big recs=%d small recs=%d\n",b_count,s_count);
		}
		if((*freeptr).sp==NULL || r_found < (rec_count / 3) ){
			D(1) if (size==BIG_RECORD) {
				fprintf(stderr,"creating big records\n"); 
			} 
			else
				fprintf(stderr,"creating small records\n");
			/* we couldnt collect any space, so ask for more */
			if( (newptr.sp= morefrees(recquan,size))==NULL ){
				fprintf(stderr,"Out of space\n");
				fprintf(stderr,"Desperate measures!\n");
				desparate();
			}
			/* tack on the old free list to the new one */
			if (size==BIG_RECORD){ 
				b_count=b_count+b_recquan;
			} 
			else
			{ 
				s_count=s_count+s_recquan;
			}
			espace.no->f_next = (*freeptr).sp;
			(*freeptr).sp = newptr.sp;
		}
	}
	x.sp = (*freeptr).sp;
	(*freeptr).sp =  x.no->f_next;
	return(x.sp);
}


char *
morefrees(quan,size)
int quan,size;
{
	SPACEITEM list, p;
	int i, recsize, amount;

	recsize = dynasizes[size];
	amount = quan*dynasizes[size];
	if( -1 == (int) (list.sp = sbrk(amount)) ){
		list.sp = NULL;
	}
	else{
		p.sp = list.sp;
		for(i=1; i<quan; i++){
			p.no->data.word = 0;
			p.no->data.bits.d_size = size;
			p.no->f_next = p.sp + recsize;
			p.sp += recsize;
		}
		p.no->data.bits.d_size = size;
		p.no->f_next = NULL;
		espace.sp = p.sp;
	}
	return(list.sp);
}

collect()
{
	SPACEITEM x;
	VALUEPTR x1;
	STPPAIR *x3;
	rMEMPTR *ppl;

	int i, size;
	D(1) fprintf(stderr,"collected ");
	/*
				 *      only one free list is necessarily empty, so
				 *      we must mark the elements of the other list
				 */
	{
		for(x.sp=free_bigs.sp; x.sp!=NULL; x.sp=x.no->f_next) x.no->data.bits.d_mark = 1;
		for(x.sp=free_smalls.sp; x.sp!=NULL; x.sp=x.no->f_next) x.no->data.bits.d_mark = 1; 
	}

	/*
				 *      all conses on the value stack must be saved
				 */

	for(x1=VStop; x1>=&v_stack[0]; x1--){
		if(x1->v_type==DOTTED_PAIR){
			if(x1->v_value.dp==NULL){
				fprintf(stderr,"value stack has null dp %d\n",x1);
			}
			markdp(x1->v_value.dp);
		}
	}
	for(x1=VStop; x1>=&v_stack[0]; x1--){
		if(x1->v_type==QSTRING ){
			if(x1->v_value.strg==NULL){
				fprintf(stderr,"value stack has null strg %d\n",x1);
			}
			markstrg(x1->v_value.strg);
		}
	}

	/*      save stpstack */
	x3 = stp_top;
	for(x3++; x3>=&stp_stack[0]; x3--){
		markstp(x3->stp_s);
		markstp(x3->stp_t);
		markstp(x3->stp_p);
	}
	/*
				 *      go thru nhashtab, removing uneeded items and
				 *      marking everthing that makes up the needed items
				 */
	for(i=0; i<NHASHSIZE; i++){
		ppl = &nhashtab[i];
		while( *ppl!=NULL ){
			if( needed(ppl)){
				(**ppl).data.bits.d_mark = 1;
				(**ppl).data.bits.g_mark++;
				markstp((**ppl).s);
				markstp((**ppl).t);
				markstp((**ppl).p);
				if((**ppl).v.v_type==DOTTED_PAIR ){
					marking = 0;
					markdp((**ppl).v.v_value.dp);
					if(marking) dumplist((**ppl).v.v_value.dp);
				}
				if( (**ppl).v.v_type==QSTRING ){
					marking = 0;
					markstrg((**ppl).v.v_value.strg);
					if(marking)
						dumpstring((**ppl).v.v_value.strg,
						stderr,false);
				}
				ppl =  &(**ppl).m_next;
			}
			else{
				(**ppl).data.bits.d_mark = 1;
				(**ppl).s = (STPPTR) free_bigs.sp;
				free_bigs.sp = (char *) *ppl;
				bfound++;
				*ppl = (**ppl).m_next;

			}
		}
	}

	/*
				 *      now that needed times and places are marked, we
				 *      can free all of the unmarked ones and delete
				 *      them from their hashtables
				 */
	cleanup(phashtab);
	cleanup(thashtab);
	cleanup(shashtab);

	/*
				 *      all of the needed stuff is marked, so
				 *      all unmarked records can be placed on
				 *      the appropriate free list.
				 */

	x.sp = bspace.sp;


	while(x.sp<espace.sp){
		size = x.no->data.bits.d_size;

		if(x.no->data.bits.d_mark==0){
			/* its not used. let it go */
			switch(size){
			case SMALL_RECORD:
				x.no->f_next = free_smalls.sp;
				free_smalls.sp = x.sp;
				sfound++;
				break;
			case BIG_RECORD:
				x.no->f_next = free_bigs.sp;
				free_bigs.sp = x.sp;
				bfound++;
				break;
			default:
				fprintf(stderr,"collect:size is %d\n",size);
				fprintf(stderr,"%d,%d\n",x.sp,
				x.no->f_next);
				my_exit(-1);
			}

		}
		else{
			x.no->data.bits.d_mark = 0;

		}
		x.sp += dynasizes[size];
	}

	D(1) fprintf(stderr,"%d records\n",sfound+bfound);
	brecycle = brecycle+bfound;
	srecycle = srecycle+sfound;
	D(2) dumphashes();
	for (i=0;i<exprquan;i++) if (retvec[i]>0&&retvec[i]<1000) retvec[i]--;

}

void markdp(p)
CELLPTR p;
{
	if(p==NULL) {
		fprintf(stderr,"null\n"); 
		marking = 1; 
		return;
	}
	if(p->data.bits.d_mark == 0) p->data.bits.d_mark = 1;
	if(p->data.bits.d_hd==DOTTED_PAIR) markdp(p->hd.dp);
	if(p->data.bits.d_hd==QSTRING) markstrg(p->hd.strg);
	if(p->data.bits.d_tl==DOTTED_PAIR) markdp(p->tl.dp);
	if(p->data.bits.d_tl==QSTRING) markstrg(p->tl.strg);
}


void markstrg(CELLPTR p)
{
	if(p==NULL) {
		fprintf(stderr,"null\n"); 
		marking = 1; 
		return;
	}
	if(p->data.bits.d_mark == 0) p->data.bits.d_mark = 1;
	if( p->data.bits.d_tl==QSTRING) markstrg(p->tl.strg);
}

markstp(x)
STPPTR x;
{       
	if(x!=NULL && x->stphd.bits.d_mark==0) { 
		x->stphd.bits.d_mark = 1; 
		markstp(x->stptl);
	} 
}

memusage()
{
	unsigned used, i, j;
	SPACEITEM p;

	used = espace.sp - bspace.sp;
	fprintf(stderr,"total dynamic usage: %u\n",used);
	fprintf(stderr,"split up as:\n");
	i = 0;
	for(p.sp=free_smalls.sp; p.sp!=NULL; p.sp=p.no->f_next){
		i++;
	}
	j = i*dynasizes[SMALL_RECORD];
	fprintf(stderr,"small records free: %u (%u)\n",i,j);
	used -= j;
	i = 0;
	for(p.sp=free_bigs.sp; p.sp!=NULL; p.sp=p.no->f_next){
		i++;
	}
	j = i*dynasizes[BIG_RECORD];
	fprintf(stderr,"big records free: %u (%u)\n",i, j);
	used -= j;
	fprintf(stderr,"actually used dynamic storage: %u\n",used);
}


cleanup(table)
STPPTR table[];
{
	int i;
	STPPTR *ppl;
	for(i=0; i<STPHASHSIZE; i++){
		ppl = &table[i];
		while( *ppl!=NULL ){
			if((**ppl).stphd.bits.d_mark==0){
				/* we can delete it */
				sfound++;
				(**ppl).stphd.bits.d_mark = 1;
				(**ppl).stptl = (STPPTR) free_smalls.sp;
				free_smalls.sp = (char *) *ppl;
				*ppl = (**ppl).stplink;
			}
			else{
				/* must save it */
				ppl = &(**ppl).stplink;
			}
		}
	}
}
rm_eres(stptl)
STPPTR stptl;
{
	int i;
	register MEMITEM **ppl;
	bfound = 0;
	for(i=0; i<NHASHSIZE; i++){
		ppl = &nhashtab[i];
		while( *ppl != NULL ) {
			if((STPPTR)((**ppl).t->stptl)==stptl ){
				(**ppl).data.bits.d_mark = 1;
				(**ppl).s = (STPPTR)free_bigs.sp;
				free_bigs.sp = (char *) *ppl;
				bfound++;
				*ppl = (**ppl).m_next;
			}
			else{
				ppl = &(**ppl).m_next;
			}
		}
	}
	D(1) fprintf(stderr,"Found %d big records\n",bfound);
	brecycle = brecycle+bfound;
}

desparate()
{
	int i;
	for (i=0; i<exprquan; i++) { 
		retvec[i]=1; 
	}
	collect();
}
/*countfree()
{ int bc,sc;
  SPACEITEM x;
	bc = sc = 0;
	for(x.sp=free_bigs.sp; x.sp!=NULL; x.sp=x.no->f_next) bc++;
	for(x.sp=free_smalls.sp; x.sp!=NULL; x.sp=x.no->f_next) sc++;
	fprintf(stderr,"FREE RECORD SMALL = %d LARGE = %d\n",sc,bc);
} */
