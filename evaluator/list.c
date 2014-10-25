
#include "imanifs.h"
#include "iglobals.h"
#include <math.h>

#define ERRCASE  VStype = ERROR; return;


#define EODCASE  VStype = EOD; return

void f_append(e)
rEXPRPTR e;
{ 
	char t1_type,t2_type;
	CELLUNION t1_val,t2_val;
	CELLPTR temp,t1,t2,cns(),app();
/* printf("ENtering append\n"); */
	eval(arg1.x);
/* return 0; */
	t1_type = VStype;
	t1_val = VSvalue;
	switch(VStype){
	case EOD:           
		return;
	case NIL:
	case DOTTED_PAIR:   
		break;
	default:            
		error("1st arg of <> is ",e->arg3.x,VStype,VSvalue);
		VStype = ERROR;
		return;
	}
	eval(arg2.x);
	t2_type = VStype;
	t2_val = VSvalue;
	switch(VStype){
	case EOD:           
		VSpop; 
		VStype=EOD; 
		return;
	case NIL:           
		VSpop; 
		return;
	case DOTTED_PAIR:   
		if (t1_type==NIL) { 
			VSpop; 
			VStype=t2_type;
			VSvalue = t2_val; 
			return; 
		}
		break;
	default:            
		error("2nd arg of <> is ",e->arg3.x,VStype,VSvalue);
		VSpop; 
		VStype = ERROR; 
		return;
	}
	temp = app(t1_type,t1_val.dp,t2_type,t2_val.dp);
	VSpop;
	VStype = DOTTED_PAIR;
	VSvalue.dp = temp;
}

CELLPTR
app(a_type,a,b_type,b)
char a_type,b_type;
CELLPTR a,b;
{ 
	CELLPTR cns(),temp;
	char *alloc();
	if ( a->data.bits.d_tl == NIL)
		return((CELLPTR) cns(a->data.bits.d_hd,a->hd,b_type,b));
	temp = (CELLPTR) alloc(SMALL_RECORD);
	temp->data.bits.d_mark =  0;
	temp->data.bits.d_hd = a->data.bits.d_hd;
	temp->hd.dp = a->hd.dp;
	temp->data.bits.d_tl = NIL;
	VSpush;
	VStype = DOTTED_PAIR;
	VSvalue.dp = temp;
printf("Recursing...\n");
	temp->tl.dp = app(a->data.bits.d_tl,a->tl.dp,b_type,b);
	temp->data.bits.d_tl = DOTTED_PAIR;
	VSpop;
	return((CELLPTR)temp);
}

listlen(type,val)
int type;
WORDCELL val;
{    
	CELLPTR a,b;
	if (type == NIL) return(0);
	if (val.dp->data.bits.d_tl == NIL) return(1);
	return(1+listlen(val.dp->data.bits.d_tl,val.dp->tl));
}


void f_cons(e)
rEXPRPTR e;
{ 
	int a_type,b_type;
	CELLPTR cns(),temp;
	WORDCELL a,b;
	eval(arg1.x);
	a_type = VStype;
	a = VSvalue;
	switch(VStype) {
	case EOD:   
		return;
	case ERROR: 
		error("1st arg of :: or arg of list is "
		    ,e->arg3.x,VStype,VSvalue);
		return;
	default:    
		break;
	}
	eval(arg2.x);
	b_type = VStype;
	b = VSvalue;
	switch(VStype) {
	case EOD:   
		VSpop; 
		VStype=EOD; 
		return;
	case ERROR: 
		error("2nd arg of :: or arg of list is ",
		e->arg3.x,VStype,VSvalue);
		VSpop; 
		VStype=ERROR; 
		return;
	case DOTTED_PAIR:
	case NIL:          
		break;
	default:    
		error("2nd arg of :: or arg of list is ",
		e->arg3.x,VStype,VSvalue);
		VSpop; 
		VStype=ERROR; 
		return;
	}
	temp = cns(a_type,a,b_type,b);
	VSpop;
	VStype = DOTTED_PAIR;
	VSvalue.dp = temp;
}

CELLPTR
cns(a_type,a,b_type,b)
char  a_type,b_type;
WORDCELL a,b;
{
	char *alloc();
	CELLPTR temp;
	temp = (CELLPTR) alloc(SMALL_RECORD);
	temp->data.bits.d_tl = b_type;
	temp->tl = b ;
	temp->data.bits.d_hd = a_type;
	temp->hd = a;
	return((CELLPTR)temp);
}

void f_hd(e)
rEXPRPTR e;
{
	eval(arg1.x);
	switch(VStype)
	{  
	case DOTTED_PAIR: 
		VStype = VSvalue.dp->data.bits.d_hd;
		VSvalue = VSvalue.dp->hd;
		break;
	case EOD:     
		VSvalue.eod = EOD;
		break;
	case NIL:     
		error("arg of hd is ",e->arg2.x,VStype,VSvalue);
		VStype = ERROR;
		return;
	case ERROR:   
		error("arg of hd is ",e->arg2.x,VStype,VSvalue);
		return;
	default:
		error("arg of hd must be a list, not  ",e->arg2.x,
		VStype,VSvalue);
		VStype = ERROR;
		break;
	}
}

void f_tl(e)
rEXPRPTR e;
{
	eval(arg1.x);
	switch(VStype)
	{  
	case DOTTED_PAIR: 
		VStype = VSvalue.dp->data.bits.d_tl;
		VSvalue = VSvalue.dp->tl;
		break;
	case EOD:     
		VSvalue.eod = EOD;
		break;
	case ERROR:   
		error("arg of tl is ",e->arg2.x,VStype,VSvalue);
		return;
	case NIL:     
		error("arg of tl is ",e->arg2.x,VStype,VSvalue);
		VStype = ERROR;
		return;
	default:
		error("arg of tl must be a list, not  ",e->arg2.x,
		VStype,VSvalue);
		VStype = ERROR;
		break;
	}
}

void f_islist(e)
rEXPRPTR e;
{ 
	eval(arg1.x);
	switch(VStype)
	{ 
	case NIL:
	case DOTTED_PAIR: 
		VStype = WORD;
		VSvalue.wrd = true;
		return;
	case EOD:         
		return;
	case ERROR:       
		return;
	default:          
		VStype = WORD;
		VSvalue.wrd = false;
		return;
	}
}

void f_mknumber(e)
rEXPRPTR e;
{ 
	char s[30];
	eval(arg1.x);
	switch(VStype)
	{  
	case QSTRING:
		formstring(&s[0],VSvalue.strg);
		VStype = NUMERIC;
		VSvalue.na.r  = atof(s);
		if (abs(VSvalue.na.r) >= 10000000.0) {
			error("numeric value return by mknumber is out of the numeric range.\ni.e it should be between +100000000 and -1000000000\n"
			    ,e->arg2.x,
			VStype,VSvalue);
			ERRCASE; 
		}
		break;
	case EOD:     
		EODCASE;
	default:      
		error("arg of mknumber should a string, not  ",e->arg2.x,
		VStype,VSvalue);
		ERRCASE;
	}
}

void f_mkstring(e)
rEXPRPTR e;
{ 
	CELLPTR findstring();
	eval(arg1.x);
	switch(VStype)
	{  
	case WORD:
		VSvalue.strg  = findstring(wordtable[VSvalue.wrd]);
		VStype = QSTRING;
		break;
	case EOD:     
		EODCASE;
	default:      
		error("arg of mkstring should be a word, not  "
		    ,e->arg2.x,
		VStype,VSvalue);
		ERRCASE;
	}
}

f_mkword(e)
rEXPRPTR e;
{ 
	CELLUNION val;
	char s[100];
	eval(arg1.x);
	val = VSvalue;
	switch(VStype)
	{  
	case QSTRING: 
		VStype = WORD;
		formstring(&s[0],VSvalue.strg);
		VSvalue.wrd=findword(s);
		if (VSvalue.wrd >= 0 ) { 
			break; 
		} 
		else
		{ 
			error("cannot convert this string into a word : ",
			e->arg2.x,QSTRING,val);
			break;
		}
	default:
		error("arg of mkword should be a string, not  "
		    ,e->arg2.x,
		VStype,VSvalue);
		VStype = ERROR;
		break;
	case EOD:     
		VSvalue.eod = EOD;
		break;
	}
}
