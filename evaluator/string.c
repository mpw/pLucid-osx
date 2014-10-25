
#include "imanifs.h"
#include "iglobals.h"
#include <math.h>

#define EODCASE  VStype = EOD; return


formstring(s,sl)
char *s;
CELLPTR sl;
{ 
CELLPTR temp;
int i;
i = 0;
temp = sl;
while ( temp->hd.swch !='\0' )
{ 
	s[i] = temp->hd.swch;
	i++;
	temp = temp->tl.strg; 
}
s[i] = '\0';
}

CELLPTR
findstring(s)
char *s;
{ 
CELLPTR temp,cns();
char *alloc(),ch;
temp = (CELLPTR) alloc(SMALL_RECORD);
ch = s[0];
temp->data.bits.d_hd = SWCHAR;
temp->hd.swch = ch;
temp->data.bits.d_tl = NIL;
VSpush;
VStype = QSTRING;
VSvalue.strg = temp;
if (s[0] != '\0') {
	temp->tl.strg = findstring(&s[1]);
	temp->data.bits.d_tl = QSTRING;
}
VSpop;
return(temp);
}


void f_substr(e)
rEXPRPTR e;
{ 
int i,type1,type2,strlen();
char *p,*temps,s[200],buff[200];
CELLPTR temp,findstring();
CELLUNION val1,val2;
eval(arg1.x);
type1 = VStype;
val1 = VSvalue;
switch(VStype)
{  
case QSTRING: 
	break;
case ERROR:   
	error("1st arg of substrg is " ,e->arg4.x,ERROR,val1);
	return;
case EOD:     
	return;
default:      
	error("1st arg of substrg is " ,e->arg4.x,VStype,VSvalue);
	VStype=ERROR;
	return;
}
eval(arg2.x);
type1 = VStype;
val2  = VSvalue;
switch(VStype){
case EOD:     
	VSpop; 
	VStype=EOD; 
	return;
case ERROR:   
	error("2nd arg of substrg is " ,e->arg4.x,ERROR,val2);
	VSpop;
	VStype = ERROR; 
	return;
case NUMERIC: 
	break;
default:      
	error("2nd arg of substrg is " ,e->arg4.x,VStype,val2);
	VSpop;
	VStype = ERROR; 
	return;
}
eval(arg3.x);
switch(VStype){
case EOD:     
	VSpop; 
	VSpop; 
	VStype=EOD; 
	return;
case ERROR:   
	error("3rd arg of substrg is " ,e->arg4.x,ERROR,val2);
	VSpop; 
	VSpop;
	VStype = ERROR; 
	return;
case NUMERIC: 
	break;
default:      
	error("3rd arg of substrg is " ,e->arg4.x,VStype,val2);
	VSpop; 
	VSpop;
	VStype = ERROR; 
	return;
}

formstring(s, val1.strg);
temps = s;
if (val2.na.r <= 0 )
{ 
	VSpop; 
	VSpop; 
	VStype = ERROR;
	error("incorrect range for substrg range1 is "
	    ,e->arg4.x,NUMERIC,val2);
	return; 
}
if (VSvalue.na.r > strlen(s))
{ 
	VSpop; 
	VSpop; 
	VStype = ERROR;
	error("incorrect ranges in substr range2 is "
	    ,e->arg4.x,NUMERIC,VSvalue);
	return; 
}
p = buff;
for (i=1;i<=(VSvalue.na.r-val2.na.r)+1;i++)
	*p++ = temps[((int)val2.na.r)+i-2];
*p = '\0';
temp = findstring(buff);
VSpop; 
VSpop;
VSvalue.strg= temp;
VStype = QSTRING;
return;
}

strglen(type,val)
int type;
WORDCELL val;
{    
CELLPTR a,b;
if (val.strg->data.bits.d_tl == NIL) return(0);
return(1+strglen(val.strg->data.bits.d_tl,val.strg->tl));
}

void f_scons(e)
rEXPRPTR e;
{ 
int a_type,b_type;
CELLPTR cns(),temp;
WORDCELL a,b;
eval(arg1.x);
a_type = VStype;
a = VSvalue;
switch(VStype){
case EOD:     
	return;
case SWCHAR:  
	break;
default:      
	error("in string ",e->arg3.x,VStype,VSvalue);
	VStype = ERROR;
	return;
}
eval(arg2.x);
b_type = VStype;
b = VSvalue;
switch(VStype) {
case EOD:     
	VSpop; 
	VStype=EOD; 
	return;
case QSTRING: 
	break;
case NIL:     
	break;
default:      
	error("in string ",e->arg3.x,VStype,VSvalue);
	VSpop;
	VStype = ERROR;
	return;
}
temp = cns(a_type,a,b_type,b);
VSpop;
VStype = QSTRING;
VSvalue.strg = temp;
}

void f_strconc(e)
rEXPRPTR e;
{ 
int t1_type,t2_type;
CELLUNION t1_val,t2_val;
CELLPTR temp,strgapp();
eval(arg1.x);
t1_type = VStype;
t1_val = VSvalue;
switch(VStype){
case EOD:     
	return;
case ERROR: 
	error("left arg of ^ is ", e->arg3.x,ERROR,VSvalue);
	return;
case QSTRING: 
	break;
default: 
	error("left arg of ^ is ", e->arg3.x,VStype,VSvalue);
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
case ERROR: 
	error("right arg of ^ is ", e->arg3.x,ERROR,VSvalue);
	VSpop; 
	VStype = ERROR;
	return;
case QSTRING: 
	break;
default: 
	error("right arg of ^ is ", e->arg3.x,VStype,VSvalue);
	VSpop;
	VStype = ERROR;
	return;
}
temp = strgapp(t1_type,t1_val.dp,t2_type,t2_val.dp);
VSpop;
VStype = QSTRING;
VSvalue.strg = temp;
}

CELLPTR
strgapp(a_type,a,b_type,b)
int a_type,b_type;
CELLPTR a,b;
{ 
CELLPTR temp;
char *alloc();
if ( a->data.bits.d_tl==QSTRING) {
	temp = (CELLPTR) alloc(SMALL_RECORD);
	temp->data.bits.d_hd = SWCHAR;
	temp->hd.swch = a->hd.swch;
	temp->data.bits.d_tl = NIL;
	if(v_top+2>=&v_stack[0]+100) { 
		printf("vstack overflow\n"); 
		exit(1); 
	}
	VSpush;
	VStype = QSTRING;
	VSvalue.strg = temp;
	temp->tl.strg = strgapp(a->data.bits.d_tl,a->tl.strg,b_type,b);
	temp->data.bits.d_tl = QSTRING;
	VSpop;
	return(temp); 
}
return(b);
}

void f_isstring(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{  
case QSTRING: 
	VSvalue.wrd = true;
	VStype = WORD;
	break;
case EOD:     
	break;
case ERROR:   
	error("arg of isstring is ",e->arg2.x,VStype,VSvalue);
	return;
default:      
	VSvalue.wrd = false;
	VStype = WORD;
	break;
}
}

void f_chr(e)
rEXPRPTR e;
{ 
char s[2];
eval(arg1.x);
switch(VStype)
{   
case NUMERIC: 
	if (VSvalue.na.r < 0 ||
	    VSvalue.na.r > 128)
	{ 
		error("integer arg of chr non-ascii ",
		e->arg2.x,VStype,VSvalue);
		VStype = ERROR;
		break;
	}
	s[0] = VSvalue.na.r;
	s[1] = '\0';
	VSvalue.strg = findstring(s); 
	VStype = QSTRING;
	break;  
case EOD:     
	break;
case ERROR:   
	error("arg of chr is ? ",e->arg2.x,VStype,VSvalue);
	return;
default:      
	error("arg of chr must be of type integer, not ",
	e->arg2.x,VStype,VSvalue);
	VStype = ERROR;
}
}

void f_ord(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{   
case QSTRING: 
	if (VSvalue.strg->hd.swch < 0 ||
	    VSvalue.strg->hd.swch > 127)
	{ 
		error("invalid char in ord", e->arg2.x,VStype,
		VSvalue);
		VStype = ERROR;
		return;
	}
	VSvalue.na.r = VSvalue.strg->hd.swch; 
	VStype = NUMERIC; 
	break;

case EOD:     
	break;
case ERROR:   
	error("arg of ord is  ?",e->arg2.x,VStype,VSvalue);
	return;
default:      
	error("arg of ord must be of type string, not ",
	e->arg2.x,VStype,VSvalue);
	VStype = ERROR;
}
}

