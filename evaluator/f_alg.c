#include "imanifs.h"
#include "iglobals.h"
#include <math.h>


#define EODCASE  VStype = EOD; return
#define LARGEST 10000000.0

void numeric_err(val,e,s)
float val;
EXPRPTR e;
char *s;
{ 
char err[120];
if (val >= 10000000000.0 || val <= -10000000000.0) {
	sprintf(err,"numeric value return by %s is out of the numeric range.\ni.e it should be between +1000000000 and -10000000000 and not ",
	s);
	error( (char*)err,e->arg3.x, VStype,VSvalue);
	VStype = ERROR;
}
}

void f_cxfile(e)
EXPRPTR e;
{
}


void f_eod()
{   
VSpush; 
VStype = EOD;  
}

void f_error()
{  
VSpush; 
VStype = ERROR; 
}

void f_nil()
{   
VSpush; 
VStype = NIL;  
}



void f_length(e)
rEXPRPTR e;
{ 
int strlen(),listlen(),type;
eval(arg1.x);
type = VStype;
switch(VStype)
{
case NIL:
case DOTTED_PAIR:    
	VStype = NUMERIC;
	VSvalue.na.r = listlen(type,VSvalue);
	break;
case QSTRING:        
	VStype = NUMERIC;
	VSvalue.na.r = strglen(type,VSvalue);
	break;
case WORD:    
	VStype = NUMERIC;
	VSvalue.na.r = strlen(wordtable[VSvalue.wrd]);
	return;
case EOD:     
	return;
case ERROR:   
	error("arg of length is ",e->arg2.x,VStype,VSvalue);
	return;
default:
	error("arg of length must be either a string, a word or a list, not  ",e->arg2.x,VStype,VSvalue);
	VStype = ERROR;
	break;
}

}

void f_swchar(e)
rEXPRPTR e;
{
VSpush;
VStype = SWCHAR;
VSvalue.swch = e->arg1.i;
}

void f_isword(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{  
case WORD:    
	VSvalue.wrd = true;
	break;
case EOD:     
	break;
case ERROR:   
	error("arg of isword is ",e->arg2.x,VStype,VSvalue);
	return;
default:      
	VSvalue.wrd = false;
	VStype = WORD;
	break;
}
}


void f_iseod(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{
case EOD:     
	VSvalue.wrd = true;
	VStype = WORD;
	break;
case ERROR:   
	error("arg of iseod is ",e->arg2.x,VStype,VSvalue);
	return;
default:      
	VSvalue.wrd = false;
	VStype = WORD;
	break;
}
}

void f_iserror(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{
case ERROR:   
	VSvalue.wrd = true;
	VStype = WORD;
	break;
case EOD:     
	break;
default:      
	VSvalue.wrd = false;
	VStype = WORD;
	break;
}
}

void f_isatom(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{  
case NIL:
case DOTTED_PAIR: 
	VSvalue.wrd = false;
	VStype = WORD;
	break;
case EOD:     
	break;
case ERROR:   
	error("arg of isatom is ",e->arg2.x,VStype,VSvalue);
	return;
default:      
	VSvalue.wrd = true;
	VStype = WORD;
	break;
}
}

void f_isnumber(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{  
case NUMERIC: 
	VSvalue.wrd = true;
	VStype = WORD;
	break;
case EOD:     
	VSvalue.eod = EOD;
	break;
case ERROR:   
	error("arg of isnumber is ",e->arg2.x,VStype,VSvalue);
	return;
default:      
	VSvalue.wrd = false;
	VStype = WORD;
	break;
}
}

void f_isnil(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype){
case NIL:     
	VSvalue.wrd = true;
	VStype = WORD;
	break;
case DOTTED_PAIR: 
	VSvalue.wrd = false;
	VStype = WORD;
	break;
case ERROR:   
	error("arg of isnull is ",e->arg2.x,VStype,VSvalue);
	return;
default:      
	error("arg of isnull must be a list, not  "
	    ,e->arg2.x,VStype,
	VSvalue);
	VStype = ERROR;
	break;
case EOD:     
	VSvalue.eod = EOD;
	break;
}
}



void f_uminus(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{  
case NUMERIC: 
	VSvalue.na.r = -VSvalue.na.r;
	if (fabs(VSvalue.na.r) >= LARGEST) {
		error("numeric value return by unary minus is out of the numeric range.\ni.e it should be between +100000000 and -1000000000\n"
		    ,e->arg2.x,
		VStype,VSvalue);
		VStype = ERROR;
	}
	break;
case EOD:     
	VSvalue.eod = EOD;
	break;
case ERROR:   
	error("arg of unary minus is ",e->arg2.x,
	VStype,VSvalue);
	return;
default:
	error("arg of negate must be numeric, not ",
	e->arg2.x,VStype,VSvalue);
	VStype = ERROR;
	break;
}
}


void f_not(e)
rEXPRPTR e;
{
eval(arg1.x);
switch(VStype)
{  
case WORD:    
	if (VSvalue.wrd==true || VSvalue.wrd==false)
	{ 
		VSvalue.wrd = VSvalue.wrd ? 0:1; 
	} 
	else
	{ 
		error("trying to apply not to the word "
		    ,e->arg2.x,VStype,VSvalue);
		VStype = ERROR; 
		return; 
	}
	VStype = WORD;
	break;
case EOD:     
	VSvalue.eod = EOD;
	break;
case ERROR:   
	error("arg of not is ",e->arg2.x,VStype,VSvalue);
	return;
default:
	error("arg of not must be the word true or false, not "
	    ,e->arg2.x,
	VStype,VSvalue);
	VStype = ERROR;
	break;
}
}

void f_and(e)
rEXPRPTR e;
{ 
int type,wval;
CELLUNION val;
eval(arg1.x);
D(30) { 
	fprintf(stderr,"and \n"); 
}
type = VStype;
val = VSvalue;
if (type ==WORD) { 
	wval = VSvalue.wrd; 
} 
else wval = -1;
switch(VStype)
{ 
case EOD:
case ERROR:
case WORD:    
	if(VStype==WORD && VSvalue.wrd==false) return;
	VSpop;
	eval(arg2.x);
	switch(VStype)
	{ 
	case ERROR:  
		switch(type){
		case EOD: 
			VStype = EOD; 
			return;
		case ERROR:
			error("both args of and are ",e->arg3.x,
			ERROR,VSvalue);
			return;
		case WORD: 
			if (wval!=true) {
				error("left arg of and the word true or false, not the word ",
				e->arg3.x,WORD,val); 
			}
		default:
			error("right arg of and is ",e->arg3.x,ERROR,
			VSvalue);
			return;
		}

	case WORD:    
		if ( VSvalue.wrd == false ) return;
		if (VSvalue.wrd==true&&type==EOD){
			VStype = EOD;
			return; 
		}
		if (VSvalue.wrd==true && type==ERROR){
			error("left arg of and is "
			    ,e->arg3.x,ERROR,VSvalue);
			VStype = ERROR;
			return; 
		}
		if (VSvalue.wrd ==true && wval==true){
			return; 
		}
		if (VSvalue.wrd != true &&
		    type == ERROR)
		{ 
			error("left arg of and is ",e->arg3.x,ERROR,VSvalue);
			error("right arg must be logical,not "
			    ,e->arg3.x,VStype,VSvalue); 
		}
		if (wval==true && VSvalue.wrd!=true)
			error("right arg of and must be logical, not ",e->arg3.x,
			VStype,VSvalue);
		if (wval!=true && VSvalue.wrd==true)
			error("left arg of and must be logical, not ",e->arg3.x,type,val);
		if (wval!=true && VSvalue.wrd!=true) {
			error("left arg of and must be logical, not "
			    ,e->arg3.x,type,val);
			error("right arg of and must be logical, not ",e->arg3.x,
			VStype,VSvalue);        
		}
		VStype =ERROR;
		return;
	default:
		error("right arg of and must be logical, not ",
		e->arg3.x,VStype,VSvalue);
		VStype = ERROR;
		break;
	case EOD:     
		VSvalue.eod = EOD;
		return;
	}
	return;
default:
	error("first arg of and must be logical, not ",e->arg3.x,
	VStype,VSvalue);
	VStype = ERROR;
	break;
}
}


void f_or(e)
rEXPRPTR e;
{ 
int type;
CELLUNION val;
eval(arg1.x);
D(30) { 
	fprintf(stderr,"or \n"); 
}
val = VSvalue;
type =VStype;
switch(VStype)
{  
case EOD:
case ERROR:
case WORD:    
	if(VSvalue.wrd==false || type == ERROR || type == EOD){
		VSpop;
		eval(arg2.x);
		switch(VStype)
		{ 
		case EOD:
		case ERROR:
		case WORD:    
			if (VStype == WORD &&
			    VSvalue.wrd == true ) return;
			if (VStype == WORD &&
			    VSvalue.wrd ==false )
			{ 
				VStype = type;
				if (type==ERROR)
					error("right arg of or is ",
					e->arg3.x,ERROR,VSvalue);
				VSvalue = val; 
				return; 
			}
			if (VStype == EOD || type == EOD) {
				VStype = EOD;
				return; 
			}
			if (VStype == ERROR && type == ERROR){
				error("both args of or are of type ",
				e->arg3.x,ERROR,VSvalue);
				return; 
			}
			if (VStype ==ERROR)
				error("right arg of or is ",
				e->arg3.x,ERROR,VSvalue);
			if (type == ERROR)
				error("left arg of or is ",
				e->arg3.x,ERROR,VSvalue);
			VStype = ERROR;
			return;
		default:
			error("right arg of or must be logical, not ",
			e->arg3.x,VStype,VSvalue);
			VStype = ERROR;
			return;
		}
	}
	if(VSvalue.wrd==true) return;
	error("left arg of or must be the word true or false, not ",
	e->arg3.x,VStype,VSvalue);
	VStype = ERROR;
	return;
default:
	error("left arg of or must be logical, not ",e->arg3.x,
	VStype,VSvalue);
	VStype = ERROR;
	return;
}
}


void f_num2(e)
EXPRPTR e;
{
register float temp,temp1,temp2;
double r1,r2,e2,e1;
CELLUNION val;
int type;
eval(arg1.x);
D(30) { 
	fprintf(stderr,"arith \n"); 
}
if (VStype== NUMERIC) { 
	e1 = VSvalue.na.r;
	e2 = VSvalue.na.i; 
}
type = VStype;
val = VSvalue;
switch(VStype)
{  
case ERROR:
case NUMERIC:
	VSpop;
	eval(arg2.x);
	switch(VStype)
	{ 
	case ERROR:
	case NUMERIC: 
		if (type==ERROR&&VStype==ERROR){
			error("both args of numeric binary operator are "
			    ,e->arg3.x,ERROR,VSvalue);
			return; 
		}
		if (type == ERROR) { 
			VStype = ERROR;
			error("left arg of numeric binary operator \nor condition of wvr or upon is ",e->arg3.x,ERROR,VSvalue);
			return; 
		}
		if (VStype==ERROR) {
			error("right arg of numeric binary operator is ",e->arg3.x,ERROR,VSvalue);
			return; 
		}
		temp1 = e1;
		temp2 = e2;
		switch(e->f){
		case F_PLUS:  
			VSvalue.na.r += temp1;
			VSvalue.na.i += temp2;
			numeric_err(VSvalue.na.i,e,"+");
			numeric_err(VSvalue.na.r,e,"+");
			break;
		case F_MINUS: 
			VSvalue.na.r= temp1-VSvalue.na.r;
			VSvalue.na.i= temp2-VSvalue.na.i;
			numeric_err(VSvalue.na.r,e,"-");
			numeric_err(VSvalue.na.i,e,"-");
			break;
		case F_TIMES: 
			if (temp2==0){ 
				VSvalue.na.r *= temp1;
				numeric_err(VSvalue.na.r,e,"*"); 
			} 
			else
			{
				VSvalue.na.r =
				    VSvalue.na.r*temp1-VSvalue.na.i*temp2;
				VSvalue.na.i =
				    VSvalue.na.r*temp2+VSvalue.na.i*temp1;
				numeric_err(VSvalue.na.r,e,"*"); 
				numeric_err(VSvalue.na.i,e,"*"); 
			} 
			break;
		case F_DIV: 
			if ((int)VSvalue.na.r==0)
			{
				error("div by "
				    ,e->arg3.x,VStype,VSvalue);
				VStype = ERROR;
				break;
			}
			r2 = VSvalue.na.r;
			r1 = temp1;
			VSvalue.na.r = (int)(r1/r2);
			numeric_err(VSvalue.na.r,"div");
			break;
		case F_MOD: 
			if(VSvalue.na.r == 0) {
				break; 
			}
			r2 = VSvalue.na.r;
			r1 = temp1;
			VSvalue.na.r = (int)(r1 - ((int)(r1 / r2))*r2);
			numeric_err(VSvalue.na.r,e,"mod");
			break;
		case F_RDIV: 
			if (VSvalue.na.r==0)
			{
				error("div by "
				    ,e->arg3.x,VStype,VSvalue) ;
				VStype = ERROR;
				break;
			}
			r2 = VSvalue.na.r;
			r1 = temp1;
			VSvalue.na.r = r1 / r2 ;
			numeric_err(VSvalue.na.r,e,"/");
			break;
		default:

			error("most interesting bin. op ",
			e->arg3.x,VStype,VSvalue);
			VStype = ERROR;
			break;
		}
		break;
	case EOD:       
		VStype = EOD;
		break;
	default:
		if (type==ERROR){
			error("left arg of numeric \n or condition of wvr or upon is ",e->arg3.x,ERROR,VSvalue);
			error("right arg must be numeric, not ",e->arg3.x,VStype,VSvalue);
			VStype = ERROR;
			return; 
		}
		error("right arg of arith op must be numeric, not "
		    ,e->arg3.x,VStype,VSvalue);
		VStype = ERROR;
		return;
	}
	break;
case EOD:     
	VSvalue.eod = EOD;
	break;
default:      
	VSpop;
	eval(arg2.x);
	switch(VStype){
	case NUMERIC:  
		VStype = ERROR;
		error("left arg of arith op must be numeric, not "
		    ,e->arg3.x,type,val);
		return;
	case ERROR:
		error("left arg of arith op must be numeric, not ",e->arg3.x,type,val);
		error("right arg of arith op is ",e->arg3.x,ERROR,VSvalue);
		return;
	case EOD:      
		return;
	default:
		error("left arg of arith op must be numeric, not ",e->arg3.x,type,val);
		error("right arg of arith op must be numeric, not ",e->arg3.x,VStype,VSvalue);
		VStype = ERROR;
		break;
	}
}
}

void f_log2(e)
rEXPRPTR e;
{ 
int type;
CELLUNION val;
float temp,e1,e2;
eval(arg1.x);
type = VStype;
val = VSvalue;
if (VStype == NUMERIC) { 
	e1 = VSvalue.na.r;
	e2 = VSvalue.na.i; 
}
/*if (e2!=0) { 
	VStype = ERROR; 
	error("relational op on type complex ",e->arg3.x,
	ERROR,VSvalue); 
	return; 
}*/
switch(VStype)
{ 
case ERROR:
case NUMERIC:
	VSpop;
	eval(arg2.x);
	switch(VStype)
	{  
	case NUMERIC:  
		if (type == ERROR) {
			VStype = ERROR;
			error("left arg of comparison op is ",e->arg3.x,
			ERROR,VSvalue);
			return; 
		}
		switch(e->f){
		case F_LE: 
			VSvalue.wrd =
			    e1 <= VSvalue.na.r;
			break;
		case F_GE: 
			VSvalue.wrd =
			    e1 >= VSvalue.na.r;
			break;
		case F_LT: 
			VSvalue.wrd =
			    e1 < VSvalue.na.r;
			break;
		case F_GT: 
			VSvalue.wrd =
			    e1 > VSvalue.na.r;
			break;
		}
		VStype = WORD;
		break;
	case EOD:     
		VSvalue.eod = EOD;
		break;
	case ERROR:   
		if (type == ERROR){
			error("both args of comparison op are "
			    ,e->arg3.x,ERROR,VSvalue);
			return; 
		}
		error("right arg of comparison op is "
		    ,e->arg3.x,ERROR,VSvalue);
		return;
	default:
		if ( type == ERROR){
			error("left arg of logigal op is ",e->arg3.x,ERROR,VSvalue);
			error("right arg must be numeric",e->arg3.x,VStype,VSvalue);
			return; 
		}
		error("right arg of comparison op must be numeric, not "
		    ,e->arg3.x,VStype,VSvalue);
		VStype = ERROR;
		return;
	}
	break;
case EOD:     
	VSvalue.eod = EOD;
	break;
default:      
	VSpop;
	eval(arg2.x);
	switch(VStype){
	case NUMERIC:
		error("left arg of comparison op must be numeric, not "
		    ,e->arg3.x,type,val);
		VStype = ERROR;
		return;
	case ERROR:
		error("left arg of comparison op must be numeric, not "
		    ,e->arg3.x,type,val);
		error("right arg is ",e->arg3.x,ERROR,VSvalue);
		return;
	case EOD: 
		return;
	default:
		error("left arg of comparison op must be numeric, not "
		    ,e->arg3.x,type,val);
		error("right arg of comparison op must be numeric, not "
		    ,e->arg3.x,VStype,VSvalue);
		VStype = ERROR;
		return ;
	}
}
}

void f_eq(e)
rEXPRPTR e;
{
VALUEPTR under;
int type;
CELLUNION val;
char b1,b2,f_eqlist(),f_eqstrg();
eval(arg1.x);
D(30) { 
	fprintf(stderr,"eq \n"); 
}
type = VStype;
val = VSvalue;
switch(VStype){
case EOD:    
	return;
case ERROR:  
	error("1st arg of eq is  ",e->arg3.x,VStype,VSvalue);
	return;
default:     
	break;
}
eval(arg2.x);
switch(VStype){
case EOD:   
	VSpop; 
	VStype=EOD; 
	return;
case ERROR: 
	error("right arg of eq is ",e->arg3.x,
	ERROR,VSvalue);
	VSpop;
	VStype=ERROR; 
	return;
default:    
	break;
}
if(VStype==type){
	switch(VStype){
	case WORD:         
		b1 = VSvalue.wrd==val.wrd ;
		break;
	case QSTRING  :    
		b1 = f_eqstrg(VSvalue.strg,val.strg) ;
		break;
	case NUMERIC  :    
		b1 = VSvalue.na.r==val.na.r;
		break;
	case NIL:          
		b1 = true;
		break;
	case DOTTED_PAIR:  
		b1 = f_eqlist(VSvalue.dp,val.dp);
		break;
	default: fprintf(stderr,"unknown type\n");
	}
}  
else      b1 = false;
VSpop;
VStype = WORD;
VSvalue.wrd = b1;
}

void f_ne(e)
rEXPRPTR e;
{
VALUEPTR under;
int type;
CELLUNION val;
char b1,b2,f_eqlist(),f_eqstrg();
eval(arg1.x);
D(30) { 
	fprintf(stderr,"ne \n"); 
}
type = VStype;
val = VSvalue;
switch(VStype){
case EOD:    
	return;
case ERROR:  
	error("1st arg of ne is  ",VStype,VSvalue);
	return;
default:     
	break;
}
eval(arg2.x);
switch(VStype){
case EOD:   
	VSpop; 
	VStype=EOD; 
	return;
case ERROR: 
	error("right arg of ne is ",e->arg3.x,
	ERROR,VSvalue);
	VSpop;
	VStype=ERROR; 
	return;
default:    
	break;
}
if(VStype==type){
	switch(VStype){
	case WORD:         
		b1 = VSvalue.wrd!=val.wrd ;
		break;
	case QSTRING  :    
		b1 = !f_eqstrg(VSvalue.strg,val.strg) ;
		break;
	case NUMERIC  :    
		b1 = VSvalue.na.r!=val.na.r;
		break;
	case NIL:          
		b1 = false;
		break;
	case DOTTED_PAIR:  
		b1 = !f_eqlist(VSvalue.dp,val.dp);
		break;
	}
}  
else      b1 = true;
VSpop;
VStype = WORD;
VSvalue.wrd = b1;
}


char
f_eqlist(l1,l2)
CELLPTR l1,l2;
{  
char f_eqstrg();
if (l1->data.bits.d_hd != l2->data.bits.d_hd) { 
	return(false); 
}
else
	switch(l1->data.bits.d_hd)
	{ 
	case NUMERIC: 
		if (l1->hd.na.r   != l2->hd.na.r  ) { 
			return(false); 
		}
		break;
	case WORD:    
		if (l1->hd.wrd  != l2->hd.wrd ) { 
			return(false); 
		}
		break;
	case QSTRING: 
		if (!f_eqstrg(l1->hd.strg,l2->hd.strg)) { 
			return(false); 
		}
		break;
	case DOTTED_PAIR: 
		if (!f_eqlist(l1->hd.dp,l2->hd.dp)) return(false);
		break;
	case NIL:     
		break;
	case EOD:     
		return(EOD);
	default:      
		return(ERROR);
	}
if (l1->data.bits.d_tl != l2->data.bits.d_tl) { 
	return(false); 
}
else
	switch(l1->data.bits.d_tl)
	{ 
	case NUMERIC: 
		if (l1->tl.na.r   != l2->tl.na.r  ) { 
			return(false); 
		}
		break;
	case WORD:    
		if (l1->tl.wrd  != l2->tl.wrd ) { 
			return(false); 
		}
		break;
	case QSTRING: 
		if (!f_eqstrg(l1->tl.strg,l2->tl.strg)) { 
			return(false); 
		}
		break;
	case DOTTED_PAIR: 
		if (!f_eqlist(l1->tl.dp,l2->tl.dp)) return(false);
		break;
	case NIL:     
		break;
	case EOD:     
		return(EOD);
	default:      
		return(ERROR);
	}

return(true);

}

char
f_eqstrg(l1,l2)
CELLPTR l1,l2;
{
if (l1->hd.swch != l2->hd.swch) { 
	return(false); 
}
if (l1->data.bits.d_tl != l2->data.bits.d_tl) { 
	return(false); 
}
if (l1->data.bits.d_tl == NIL) { 
	return(true); 
}
return(true && f_eqstrg(l1->tl.strg,l2->tl.strg));
}

f_const(e)
rEXPRPTR e;
{
VSpush;
VStype = NUMERIC;
VSvalue.na.r = e->arg1.c.r;
}


void f_word(e)
rEXPRPTR e;
{
VSpush;
VStype = WORD;
VSvalue.wrd = e->arg1.i;
}

void f_if(e)
rEXPRPTR e;
{ 
int ptype,xtype;
WORDCELL xval;
eval(arg1.x);
D(30) { 
	fprintf(stderr,"if \n"); 
}
ptype = VStype;
switch(VStype)
{
case WORD:    
	if(VSvalue.wrd==true){
		VSpop;
		eval(arg2.x);
		return; 
	}
	if(VSvalue.wrd==false){
		VSpop;
		eval(arg3.x);
		return; 
	}
	error("condtion of if-then-else, : , wvr or upon must the word true or false, not "
	    ,e->arg4.x,VStype,VSvalue);
	VStype = ERROR;
	break;
case EOD:
	break;
case ERROR:
	error("condition of if-then-else, :, wvr or upon is ",e->arg4.x,ERROR,VSvalue);
	break;
default:
	error("condition of if-then-else, :, wvr or upon must be logical, not ",e->arg4.x,
	VStype,VSvalue);
	VStype = ERROR;
	break;
}

}
