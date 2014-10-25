#include "imanifs.h"
#include "iglobals.h"
#include <sys/types.h>
#include <sys/times.h>
char charcode[5];

void dumpstring(CELLPTR x,int channel,char flag);
void char_to_code(char c);
void dumpfile(char *x,EXPRPTR e);

bar(x)
int x;
{ 
	int i;
	if (x>80) { 
		x= 80; 
	}
	for (i=0; i<x; i++) { 
		fprintf(stderr,"|"); 
	}
}

dump_uctable(s)
char *s;
{  
	int i;
	FILE *ucout;
	char t[100];
	sprintf(t,"%s.uc",s); 
	if ( (ucout=fopen(t,"w"))==NULL){
		fprintf(stderr,"cannot write to file %s\n",s); 
	}
	else
	{
		for(i=0; i<exprquan;i++)
		{ 
			fprintf(ucout,"%d\n",u_countvec[i]); 
		}
	}
}

my_exit(n)
int n;
{   
	if (isatty(fileno(stdout))){
		stats(); 
	}
	dump_uctable(infilename);
	exit(2);
}

dumpspaces(n)
int n;
{ 
	int i;
	for (i=0; i<n; i++) fprintf(stderr," "); 
}

stats()
{   
	struct tms buff;
	time_t syst,ut;
	times(&buff);
	syst = (time_t)((buff.tms_stime)/60);
	ut =   (time_t)((buff.tms_utime)/60);
	fprintf(stderr,"\n\n");
	fprintf(stderr,"Statistical Information about the evaluation\n");
	D(7) {
		fprintf(stderr,"# of small records recycled    = %10d (%10d bytes)\n"
		    ,srecycle,12*srecycle);
		fprintf(stderr,"# of large records recycled    = %10d (%10d bytes)\n"
		    ,brecycle,24*brecycle);
		fprintf(stderr,"Total # of records recycled    = %10d (%10d bytes)\n"
		    ,srecycle+brecycle,12*srecycle+24*brecycle);
		fprintf(stderr,"# of small records used        = %10d (%10d bytes) \n"
		    ,s_count,12*s_count);
		fprintf(stderr,"# of large records used        = %10d (%10d bytes) \n"
		    ,b_count,24*b_count);
		fprintf(stderr,"Total # of records used        = %10d (%10d bytes) \n"
		    ,b_count+s_count,12*s_count+24*b_count);
		fprintf(stderr,"# of demands for variables     = %10d\n"
		    ,varcount);
		fprintf(stderr,"# demands with computation     = %10d\n"
		    ,notfoundcount);
		fprintf(stderr,"user time                      = %10d secs\n"
		    ,(int)ut);
		fprintf(stderr,"system time                    = %10d secs\n"
		    ,(int)syst);
		fprintf(stderr,"total CPU time                 = %10d secs\n"
		    ,(int)(ut+syst)); 
        fprintf(stderr,"%f eductions per second including system calls\n",(double)varcount/(syst+ut));
        fprintf(stderr,"%f eductions per second\n",(double)varcount/ut);
	}
	fprintf(stderr,"# of memory buckets created    = %10d\n"
	    ,memcount);
	fprintf(stderr,"# of space/time/place tags created   = %10d\n"
	    ,stpcount);
}

void dumpfile(char *x,EXPRPTR e)
{  
	int i,lineno,firstline,lastline,cursorposition;
	char ch,*filename;
	FILE *readfile;
	filename = wordtable[e->arg1.i] ;
	firstline = e->arg2.i;
	lastline = e->arg3.i;
	cursorposition = e->arg4.i;
	fprintf(stderr,"\nEvaluation time error ");
	fprintf(stderr,"on line %d, ",firstline);
	fprintf(stderr,"of file %s\n ",filename);
	if ((readfile= fopen(filename,"r"))==NULL)
	{ 
		fprintf(stderr,"\nfile %s cannot be opened to read\n",filename);
		return;
	}
	lineno=1;
	while (lineno < firstline ) { 
		ch = dump_getc(readfile,filename);
		if (ch=='\n') lineno++;
	}
	ch = dump_getc(readfile,filename);
	while (ch !='\n') { 
		fprintf(stderr,"%c",ch);
		ch = dump_getc(readfile,filename);
	}
	fprintf(stderr,"\n");
	for (i=0; i<cursorposition; i++) fprintf(stderr,".");
	fprintf(stderr,"^");
	fprintf(stderr,"\n%s",x);
	fclose(readfile);
}

dump_getc(readfile,filename)
FILE *readfile;
char *filename;
{
	int ch;
	ch = getc(readfile);
	if (ch == EOF) { 
		fprintf(stderr,"unexpected end to file %s\n",
		filename);
		my_exit(1);
	}
	return(ch);
}

dumpstp(x)
STPPTR x;
{
	STPPTR s;
	fprintf(stderr,"(");
	s = x;
	while( s!=NULL ){
		fprintf(stderr,"%d",s->stphd.word);
		s = s->stptl;
		if( s!=NULL ){
			fprintf(stderr,",");
		}
	}
	fprintf(stderr,")");
}
dumps(x)
STPPTR x;
{
	STPPTR s;
	fprintf(stderr,"(");
	s = x;
	while( s!=NULL ){
		fprintf(stderr,"<%d,%d,%d>",s->stphd.xyz.d1,
		s->stphd.xyz.d2,
		s->stphd.xyz.d3);
		s = s->stptl;
		if( s!=NULL ){
			fprintf(stderr,",");
		}
	}
	fprintf(stderr,")");
}

dumpt(x)
STPPTR x;
{
	fprintf(stderr,"(");
	fprintf(stderr,"%3d",x->stphd.word);
	fprintf(stderr,")");
}



dumpstphash(i,table)
int i;
STPPTR table[];
{
	STPPTR p;
	fprintf(stderr,"Bucket %d ",i);
	for(p=table[i]; p!=NULL; p=p->stplink){
		dumpstp(p);
	}
	fprintf(stderr,"\n");
}
dumpnhash(i)
int i;
{
	register MEMPTR s;
	if(nhashtab[i]!=NULL){
		fprintf(stderr,"bucket %d\n",i);
		for(s=nhashtab[i];s!=NULL;s=s->m_next){
			dumpmem(s);
		}
	}
}

dumpmem(x)
register MEMPTR x;
{
	int crcint,icrcint;
	float rlval,ilval;
	fprintf(stderr,"name: %d %s ",x->data.bits.n,nametable[x->data.bits.n]);
	fprintf(stderr,"time: "); 
	dumpstp(x->t);
	fprintf(stderr,"place: "); 
	dumpstp(x->p);
	fprintf(stderr,"value: ");
	switch(x->v.v_type){
	case NUMERIC:
		icrcint=x->v.v_value.na.i;
		crcint=x->v.v_value.na.r;
		ilval=icrcint;
		rlval=crcint;
		if (x->v.v_value.na.i == 0 ) 
		{
			if (rlval==x->v.v_value.na.r ) {
				if ((int)(x->v.v_value.na.r)<0) {
					fprintf(stderr,
					"~%-d ",-(int)(x->v.v_value.na.r)); 
				} 
				else
					fprintf(stderr,"%-d ",(int)(x->v.v_value.na.r)); 
			}
			else { 
				if (x->v.v_value.na.r <0) {
					fprintf(stderr,
					"~%-10.5f ",-x->v.v_value.na.r); 
				} 
				else
					fprintf(stderr,"%-10.5f ",x->v.v_value.na.r); 
			}
		} 
		else
		{
			if (x->v.v_value.na.r <0) {
				fprintf(stderr,
				"~(%-10.5f,%-10.5f)",-x->v.v_value.na.r,
				x->v.v_value.na.i); 
			} 
			else
				fprintf(stderr,"(%-10.5f,%-10.5f)",x->v.v_value.na.r,
				x->v.v_value.na.i); 
		}
		break;
	case WORD:
		dumpword(x->v.v_value.wrd,stderr);
		break;
	case QSTRING:
		dumpstring(x->v.v_value.strg,stderr,false);
		break;
	case NIL: 
		fprintf(stderr,"[]\n");
		break;
	case ERROR: 
		fprintf(stderr,"?");
		break;
	case DOTTED_PAIR:
		dumplist(x->v.v_value.dp,stderr);
		break;
	case EOD:
		fprintf(stderr,"@ \n");
		break;
	}
}

dumpmemry(type,value)
int type;
WORDCELL value;
{
	int crcint;
	float rlval;
	switch(type){
	case NUMERIC:
		crcint=value.na.r;
		rlval=crcint;
		if (rlval==value.na.r ) {
			if ((int)(value.na.r)<0) {
				fprintf(stderr,"~%-d\n",-(int)(value.na.r)); 
			} 
			else
				fprintf(stderr,"%-d\n",(int)(value.na.r)); 
		}
		else { 
			if (value.na.r <0) {
				fprintf(stderr,"~%-10.5f\n",-value.na.r); 
			} 
			else
				fprintf(stderr,"%-10.5f\n",value.na.r); 
		}
		break;
	case WORD:
		dumpword(value.wrd,stderr);
		fprintf(stderr,"\n");
		break;
	case QSTRING:
		dumpstring(value.strg,stderr,false);
		fprintf(stderr,"\n");
		break;
	case NIL: 
		fprintf(stderr,"[]\n");
		break;
	case ERROR: 
		fprintf(stderr,"?\n");
		break;
	case DOTTED_PAIR:
		dumplist(value.dp,stderr);
		fprintf(stderr,"\n");
		break;
	}
}


dumpval(stream,x)
FILE *stream;
VALUEPTR x;
{       
	int crcint,icrcint;
	float rlval,ilval;
	switch(x->v_type){
	case NUMERIC:
		icrcint=x->v_value.na.i;
		crcint=x->v_value.na.r;
		ilval=icrcint;
		rlval=crcint;
		if (x->v_value.na.i==0 ) 
		{
			if (rlval==x->v_value.na.r ) 
			{
				if ((int)(x->v_value.na.r)<0) 
				{
					E(13) fprintf(stream,"~%-9d",
					-(int)(x->v_value.na.r)); 
else fprintf(stream,"%d",
(int)(x->v_value.na.r)); 
} 
				else
					E(13) fprintf(stream,"%d",(int)(x->v_value.na.r)); 
				else fprintf(stream,"%d",(int)(x->v_value.na.r)); 
			}
			else  
			    if (x->v_value.na.r <0) 
			{
				E(13) fprintf(stream,"~%-9f",-x->v_value.na.r); 
				else fprintf(stream,"%10f",x->v_value.na.r); 
			} 
			else
				E(13) fprintf(stream,"%-10f",x->v_value.na.r); 
			else  fprintf(stream,"%-10f",x->v_value.na.r); 
		} 
		else
		{
			E(13) fprintf(stream,"(%-10f,%-10f)",
			x->v_value.na.r,x->v_value.na.i); 
			else fprintf(stream,"(%10f,%10f)",
			x->v_value.na.r,x->v_value.na.i); 
		}
		break;
	case WORD:
		dumpword(x->v_value.wrd,stream);
		break;
	case QSTRING:
		dumpstring(x->v_value.strg,stream,sflag);
		break;
	case NIL: 
		fprintf(stream,"        []");
		break;
	case ERROR: 
		fprintf(stream,"         ?");
		break;
	case DOTTED_PAIR:
		dumplist(x->v_value.dp,stream);
		break;
	case EOD:
		fprintf(stream,"        @");	
		break;
	}
	if (!sflag) fprintf(stream," ");
}

STRING
code_to_char(s)
STRING s;
{ 
	char c,newstring[200];
	int j,i,l,sum;
	i = 0;
	l = 0;
	c = s[i];
	while (c != '\0')
	{ 
		if (c == '\\')
		{ 
			i++; 
			c = s[i];
			switch(c){
			case 'n': 
				newstring[l]='\n'; 
				l++;
				break;
			case 't': 
				newstring[l]='\t'; 
				l++;
				break;
			case 'f': 
				newstring[l]='\f'; 
				l++;
				break;
			case 'b': 
				newstring[l]='\b'; 
				l++;
				break;
			case 'r': 
				newstring[l]='\r'; 
				l++;
				break;
			case '\\': 
				newstring[l]='\\'; 
				l++;
				break;
			case '\'': 
				newstring[l]='\''; 
				l++;
				break;
			default: 
				if ( c >= '0' && c <= '7' )
				{ 
					sum =0;
					for(j=1; j<=3 && c<='7'&& c>='0' ; j++)
					{ 
						i++;
						sum = sum*8+ c-'0';
						c = s[i];
					}
					newstring[l]=sum; 
					l++;
				}

				newstring[l]=c; 
				l++;

			}
		} 
		else { 
			newstring[l]=c; 
			l++; 
		}
		i++;
		c = s[i];
	}
	newstring[l] = '\0';
	strcpy(s,newstring);
	return(s);
}

void char_to_code(char c)
{
	charcode[0]='\\';
	switch(c){
	case '\n': 
		charcode[1]='n';
		break;
	case '\t': 
		charcode[1]='t';
		break;
	case '\f': 
		charcode[1]='f';
		break;
	case '\b': 
		charcode[1]='b';
		break;
	case '\r': 
		charcode[1]='r';
		break;
	case '\\': 
		charcode[1]='\\';
		break;
	case '\'': 
		charcode[1]='\'';
		break;
	default: 
		if (!isprint(c)&&!isspace(c))
		{ 
			sprintf(charcode,"\\%o",c);
			return; 
		} 
		else
		{
			charcode[0]=c;
			charcode[1]='\0';
			return;
		}
	}
	charcode[2] = '\0';
}

pstring(s,channel)
char *s;
FILE *channel;
{ 
	char c;
	int i;
	i = 0;
	c = s[i];
	while (c != '\0')
	{ 
		fprintf(channel,"%c",c);
		i++;
		c = s[i];
	}
}

dumpval2(stream,x)
FILE *stream;
VALUE x;
{       
	int crcint;
	float rlval;
	switch(x.v_type){
	case NUMERIC:
		crcint=x.v_value.na.r;
		rlval=crcint;
		if (rlval==x.v_value.na.r ) {
			if ((int)(x.v_value.na.r)<0) {
				fprintf(stream,"~%-d",-(int)(x.v_value.na.r)); 
			} 
			else
				fprintf(stream,"%-d",(int)(x.v_value.na.r)); 
		}
		else { 
			if (x.v_value.na.r <0) {
				fprintf(stream,"~%-10.5f",-x.v_value.na.r); 
			} 
			else
				fprintf(stream,"%-10.5f",x.v_value.na.r); 
		}
		break;
	case WORD:
		dumpword(x.v_value.wrd,stream);
		break;
	case QSTRING:
		dumpstring(x.v_value.strg,stream,current_io->outmode==UNIX);
		break;
	case ERROR: 
		fprintf(stream,"?");
		break;
	case NIL:
		fprintf(stream,"[]");
		break;
	case DOTTED_PAIR:
		dumplist(x.v_value.dp,stream);
		break;
	case EOD:
		fprintf(stream,"EOD");
		break;
	}
}

dumplist(x,channel)
CELLPTR x;
int channel;
{       
	float rlval;
	int crcint;
	fprintf(channel,"[");
	while(1){
		if(x->data.bits.d_hd==NUMERIC){
			crcint=x->hd.na.r;
			rlval=crcint;
			if (rlval==x->hd.na.r)  {
				if ((int)(x->hd.na.r)<0){
					fprintf(channel,"~%d",(int)(-x->hd.na.r)); 
				} 
				else
					fprintf(channel,"%d",(int)(x->hd.na.r)); 
			}
			else {
				if (x->hd.na.r < 0 ) {
					fprintf(channel,"~%10.5f",-x->hd.na.r); 
				} 
				else
					fprintf(channel,"%10.5f",x->hd.na.r); 
			}
		}
		else if(x->data.bits.d_hd==WORD){
			dumpword(x->hd.wrd,channel);
		}
		else if(x->data.bits.d_hd==QSTRING){
			dumpstring(x->hd.strg,channel,sflag);
		}
		else if(x->data.bits.d_hd==NIL){
			fprintf(channel,"[]");
		}
		else if(x->data.bits.d_hd==DOTTED_PAIR){
			dumplist(x->hd.dp,channel);
		} 
		else {
			fprintf(stderr,"messed up list");
			break;
		}
		if(x->data.bits.d_tl==NUMERIC){
			crcint=x->tl.na.r;
			rlval=crcint;
			if (rlval==x->tl.na.r)  {
				if ((int)(x->tl.na.r)<0){
					fprintf(channel,"~%d",(int)(-x->tl.na.r)); 
				} 
				else
					fprintf(channel,"%d",(int)(x->tl.na.r)); 
			}
			else {
				if (x->tl.na.r < 0 ) {
					fprintf(channel,"~%10.5f",-x->tl.na.r); 
				} 
				else
					fprintf(channel,"%10.5f",x->tl.na.r); 
			}
			break;
		}
		else if(x->data.bits.d_tl==NIL){
			break;
		}
		else if(x->data.bits.d_tl==WORD){
			dumpword(x->tl.wrd,channel);
			break;
		}
		else if(x->data.bits.d_tl==QSTRING){
			dumpstring(x->tl.strg,channel,sflag);
			break;
		}
		else if(x->data.bits.d_tl==DOTTED_PAIR){
			fprintf(channel," ");
			x = x->tl.dp;
		} 
		else  {
			fprintf(stderr,"messed up list two");
			break;
		}
	}
	fprintf(channel,"]");
}

dumpword(x,channel)
int x;
int channel;
{
	fprintf(channel,"%10s",wordtable[x]);
}

void dumpstring(CELLPTR x,int channel,char flag)
{   
	char c;
	if (!flag) fprintf(channel,"`");
	while(1) { 
		c = x->hd.swch;
		if(x->data.bits.d_tl==NIL) {
			if (!flag) fprintf(channel,"\'");
			return; 
		}
		if (flag) { 
			fprintf(channel,"%c",c); 
		}
		else    { 
			char_to_code(c);
			fprintf(channel,"%s",charcode); 
		}
		x = x->tl.strg;
	}
}

dumphashes()
{
	int i;
	/*
			fprintf(stderr,"\n\n\n\n*****NAMES******\n");
			for(i=0; i<NHASHSIZE; i++){
				if(nhashtab[i]!=NULL) dumpnhash(i);
			}
		*/
	fprintf(stderr,"\n\n\n\n\n*****TIMES******\n");
	for(i=0; i<STPHASHSIZE; i++){
		if(thashtab[i]!=NULL) dumpstphash(i,thashtab);
	}
	fprintf(stderr,"\n\n\n\n****PLACES******\n");
	for(i=0; i<STPHASHSIZE; i++){
		if(phashtab[i]!=NULL) dumpstphash(i,phashtab);
	}
}
