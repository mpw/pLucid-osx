#include "imanifs.h"
#include "iglobals.h"
int countlen;
int comps=0;

char
memsearch(place,n,s,t,p)
MEMPTR *place;
int n;
register STPPTR s,t,p;
{
	register int i;
	char * alloc();
	register char found;
	register MEMPTR m;
	D(30) { 
		bar(ddepth);
		ddepth++;
		fprintf(stderr,"?%s",nametable[n]);
		dumps(s); 
		dumpstp(t); 
		dumpstp(p);
		fprintf(stderr,"\n");
	}

	D(20) { 
		fprintf(stderr,"t=%d p=%d\n",(int)t,(int)p); 
	}


	i = ((59*(int)s)+(39*(int)t)+(7*(int)p)+(n)) % NHASHSIZE;
	if (i<0) i = -i;
	found=0;
	D(19) { 
		fprintf(stderr,"hashval = %d\n",i);    
	}
	countlen=0;
	for(m=nhashtab[i]; m!=NULL; m=m->m_next){
		if(n==m->data.bits.n && s==m->s && t==m->t && p==m->p){
			found = 1;
			D(30) { 
				ddepth--;
				bar(ddepth);
				fprintf(stderr,"*%s",nametable[n]);
				dumps(s); 
				dumpstp(t); 
				dumpstp(p);
				fprintf(stderr," ");
				dumpval2(stderr,m->v);
				fprintf(stderr,"\n");  
			}

			if (m->data.bits.g_mark>=retvec[n]) { 
				retvec[n]++; 
			}
			break;
		}
		countlen++;
	}

	comps=comps+countlen;
	D(21)  { 
		fprintf(stderr,"total comps =%d\n",comps);    
	}
	D(18)  { 
		fprintf(stderr,"chain length = %d\n",countlen);   
	}
	if(!found){   
		memcount++;
		m = (MEMPTR) alloc(BIG_RECORD);
		m->data.bits.u_count=0;
		m->data.bits.d_hd = 0;
		m->data.bits.d_tl = 0;
		m->data.bits.d_size = BIG_RECORD;
		m->data.bits.d_mark=0;
		m->data.bits.n = n;
		m->s = s;
		m->t = t;
		m->p = p;
		m->v.v_type = UNDEFINED;
		m->v.v_value.na.r = 0;
		m->v.v_value.na.i = 0;
		m->m_next = nhashtab[i];
		nhashtab[i] = m;

	} 
	else
	{ 
		if (m->v.v_type == UNDEFINED ) {
			if ( exprtable[n]-> f != F_INPUT && exprtable[n]->f != F_FILE 		    && exprtable[n]->f != F_FILTER )
			{
	fprintf(stderr,"Self-reference ERROR\n");
	fprintf(stderr,"Variable '%s' is defined in terms of its own current or future value \n( the equivalent of writing x=x )\n",nametable[n]);
	fprintf(stderr,"Evaluation aborted\n");
				exit(1); 
			}
		}
	}
	m->data.bits.u_count++;
	m->data.bits.g_mark =0;
	*place = m;
	return(found);
}

/*
 */
STPPTR
ssearch(hd)
COORDS hd;
{
	int found;
	char *alloc();
	int i;
	STPPTR s;

	found = 0;
	i = (39*hd.d1+19*hd.d2+hd.d3) % SHASHSIZE;
	if (i<0) i = -i;
	for(s=shashtab[i]; s!=NULL; s=s->stplink){
		/*
						 */
		if(s->stphd.xyz.d1==hd.d1 &&
		    s->stphd.xyz.d2==hd.d2 && 
		    s->stphd.xyz.d3==hd.d3)  
		{ 
			found = 1;
			break;
		}
	}
	if(!found){   
		stpcount++;
		s = (STPPTR) alloc(SMALL_RECORD);
		s->stplink = shashtab[i];
		shashtab[i] = s;
		s->stphd.xyz.d1= hd.d1;
		s->stphd.xyz.d2= hd.d2;
		s->stphd.xyz.d3= hd.d3;
		s->stptl = NULL;
	}
	return(s);
}
STPPTR
stpsearch(hd,tl,table)
long hd;
STPPTR tl;
STPPTR table[];
{
	int found;
	char *alloc();
	int i;
	STPPTR s;

	found = 0;
	i = (hd) % THASHSIZE;
	if (i<0) i = -i;
	for(s=table[i]; s!=NULL; s=s->stplink){
		/*
						 */
		if(s->stphd.word==hd && s->stptl==tl){
			found = 1;
			break;
		}
	}
	if(!found){   
		stpcount++;
		s = (STPPTR) alloc(SMALL_RECORD);
		s->stplink = table[i];
		table[i] = s;
		s->stphd.word = hd;
		s->stptl = tl;
	}
	return(s);
}

