#include "cmanifs.h"
#include "cglobals.h"

void dumpattr(SYMPTR x);

SYMPTR
handle_ident(name,length)
STRING name; char length;
{
	SYMPTR find_ident();
	SYMPTR glo_ident();
	SYMPTR for_ident();
	switch(idusage){
	case U_NORMAL:
		return( find_ident(name,length) );
	case U_FORMAL:
		return( for_ident(name,length) );
	case U_EGLOBAL:
		return( glo_ident(name,length,GLOB_A|ELEM_A) );
	case U_NGLOBAL:
		return( glo_ident(name,length,GLOB_A) );
	default:
		return((SYMPTR)-1);

	}
}

SYMPTR
find_ident(name,length)
STRING name;
char length;
{
	STRING calloc();
	STRING strsave();
	SYMPTR scan;
	SYMPTR new;
	int i;
	char found;
	i = hash(name);
	found = 0;
	scan = hashtable[i];
	while(scan && scan->lexlevel==lexlevel){
		if(eqstring(scan->name,name)){
			found = 1;
			break;
		}else{
			scan = scan->next;
		}
	}
	if(found){
		return(scan);
	}else{
		new = (SYMPTR) calloc(1, sizeof(SYMBOL));
		new->name = strsave(name);
		new->next = hashtable[i];
		new->lexlevel = lexlevel;
		if(f_stack->f_ll == lexlevel){
			new->fldef = funclevel-1;
		}else{
			new->fldef = funclevel;
		}
		new->elemdef = elemlevel;
		new->idno = identcount++;
		new->attr = LOC_A;
		hashtable[i] = new;
		return(new);
	}
}



SYMPTR
glo_ident(name,length,attr)
STRING name;
int length;
char attr;
{
	STRING calloc();
	SYMPTR scan, new,temp,prev;
	int i;
	char found;
	i = hash(name);
	scan = hashtable[i];
	found = 0;
	while(scan){
		if(eqstring(scan->name,name)){
			if(scan->lexlevel==lexlevel){
			 yyerror(scan->name,
				 " already declared in this scope");
				return(scan);
			}else{
				found = 1;
			}
			break;
		}else{
			scan = scan->next;
		}
	}
	if(!found){
		if(lexlevel==1){
			yyerror("no containing scope for %s\n",name);
			return(scan);
		}
		new = (SYMPTR) calloc(1, sizeof(SYMBOL));
		new->idno = identcount++;
		new->name = strsave(name);
		new->lexlevel = lexlevel-1;
		new->elemdef = elemlevel - p_stack->p_iselem;
		if(f_stack->f_ll==lexlevel-1){
			new->fldef = funclevel-1;
		}else{
			new->fldef = funclevel;
		}
		new->attr = LOC_A;
	 /*       new -> next = hashtable[i];
		hashtable[i] = new;
	   */
		scan = hashtable[i];
		if(scan==NULL){
			hashtable[i] = new;
			new->next = NULL;
		}else{  char insert_at_begin;
			insert_at_begin = false;
			if (scan->lexlevel!=lexlevel){
			   insert_at_begin = true;
			   hashtable[i] = new; }
			while(scan && scan->lexlevel==lexlevel){
				prev = scan;
				scan = scan->next;
			}
			if(!insert_at_begin) prev->next = new;
			new->next = scan;
		}
		scan = new;
	}

	/*
	 *      scan points to the record for the variable in the
	 *      previous scope.
	 *
	 *      if the global variable we have inherited has been
	 *      inherited in the past, we must make sure that it is being
	 *      inherited in the same way this time. if it is not, we
	 *      have an error.
	 */
	if( IS_USED(scan) ){
		if( !(scan->attr&UELEM_A) != !(attr&ELEM_A) ){
			yyerror("%s inherited inconsistently",name);
		} else {  }
		/*printf("%o, %o\n",scan->attr&UELEM_A,attr&ELEM_A); */
	}else{
		scan->attr |= USED_A; /*now its used */
		if( attr&ELEM_A ){
			scan->attr |= UELEM_A;
		}
	}
	new = (SYMPTR) calloc(1, sizeof(SYMBOL));
	new->next = hashtable[i];
	new->name = scan->name;
	new->idno = scan->idno;
	new->attr = attr | scan->attr;
	new->lexlevel = lexlevel;
	new->elemdef = scan->elemdef; /* elem level where ident is defined */
	new->fldef = scan->fldef;
	hashtable[i] = new;
	return(new);
}



SYMPTR for_ident(name,length)
STRING name;
int length;
{
	STRING calloc();
	SYMPTR scan, new;
	char found;
	int i;

	i = hash(name);
	found = 0;
	scan = hashtable[i];
	while(scan && scan->lexlevel==lexlevel){
		if(eqstring(scan->name,name) && IS_FORM(scan) && !IS_GLOB(scan)){
			found = 1;
			break;
		}else{
			scan = scan->next;
		}
	}
	if(found){
		yyerror("more than one formal of the same name");
		return(scan);
	}else{
		new = (SYMPTR) calloc(1,sizeof(SYMBOL));
		new->name = strsave(name);
		new->idno = formalcount++;
		new->attr = FORM_A;
		new->lexlevel = lexlevel;
		new->elemdef = elemlevel;
		/* strictly speaking, formal is declared outside
		/* the funtion def, and then inherited (in practice, as
		/* an actual parameter
		 */
		new->fldef = funclevel-1;
		new->next = hashtable[i];
		hashtable[i] = new;
		return(new);
	}
}

eqstring(a,b)
STRING a,b;
{
	while( *a++ == *b++ ){
		if ( *a == '\0' && *b == '\0' ) {
			return(1);
		} else if (*a == '\0' || *b == '\0') break;
	}
	return(0);
}

hash(s)
STRING s;
{
	int hashval;
	/*fprintf(stderr,"####%s####",s); */
	hashval = *s;
	while(*s){
		  hashval = (hashval+(*s & 127));
		  s++;
	}
	/*fprintf(stderr,"####%d####\n",hashval%HASHSIZE); */
	return(hashval%HASHSIZE);
}



enter_phrase()
{
	STRING calloc();
	P_STACKPTR temp;
	SYMPTR new;
	int i;

	lexlevel++;
	/*
	 * push the new phrase environment onto the phrase stack
	 */
	temp = (P_STACKPTR) calloc(1, sizeof(P_STACK));
	temp->p_tl = p_stack;
	temp->p_idno = identcount++;
	temp->p_iselem = 0;
	p_stack = temp;

	/* enter the RESULT, which must occur in each scope */
	i = hash("_result");
	new = (SYMPTR) calloc(1,sizeof(SYMBOL));
	new->name = strsave("_result");
	new->lexlevel = lexlevel;
	new->idno = p_stack->p_idno;
	new->attr = LOC_A;
	new->next = hashtable[i];
	hashtable[i] = new;
}

EXPRPTR
exit_phrase()
{
	P_STACKPTR old;
	SYMPTR temp, scan;
	EXPRPTR new;
	int i;

	for(i=0; i<HASHSIZE; i++){
		scan = hashtable[i];
		while(scan && (scan->lexlevel==lexlevel)){
			temp = scan;
			scan = scan->next;
			if(IS_LOCAL(temp) && !IS_DEF(temp)){
				/* local is not defined. define it to be
				/* input variable.
				 */
				new = (EXPRPTR) calloc(1, sizeof(U_EXPR));
				new->dim = 0;
				new->f = F_INPUT;
				new->arg1.i = temp->idno;
				exprtable[temp->idno] = new;
				nametable[temp->idno] = strsave(temp->name);
			}
			free_ident(temp);
		}
		hashtable[i] = scan;
	}
	/* p_stack has at the top the id number of the result variable
	/* for this phrase. this enables us to build an expression which
	/* is the value of the phrase
	 */
	if(p_stack->p_iselem==1){
		new = (EXPRPTR) calloc(1, sizeof(U_EXPR));
		new->dim = 0;
		new->f = F_ERES;
		elemlevel--;
	}else{
		new = (EXPRPTR) calloc(1, sizeof(T_EXPR));
		new->dim = 0;
		new->f = F_VAR;
		new->arg2.i = 0; /* dont pop off anything */
		new->arg3.i = 0; /* place stack remains unchanged */
	}
	new->arg1.i = p_stack->p_idno;
	lexlevel--;
	old = p_stack;
	p_stack = p_stack->p_tl;
	free(old);
	return(new);
}




enter_function(){
	STRING calloc();
	F_STACKPTR new;

	idusage = (int) U_FORMAL;
	formalcount = 0;
	funclevel++;

	new = (F_STACKPTR) calloc(1,sizeof(F_STACK));
	new->f_ll = lexlevel;
	new->f_el = elemlevel;
	new->f_tl = f_stack;
	f_stack = new;

}

exit_function()
{
	int i;
	SYMPTR temp, scan;
	F_STACKPTR stemp;

	for(i=0; i<HASHSIZE; i++){
		scan = hashtable[i];
		/* remove pure formals. this means that if the
		/* function is defined in an outer function, we
		/* do not remove its formals too, since they
		/* must have been inherited into the phrase that
		/* the present function is declared in, and we
		/* are still in that phrase, so the earlier functions'
		/* formals are globals in this scope.
		 */
		while(scan && IS_FORM(scan) && (!IS_GLOB(scan)) && scan->lexlevel==lexlevel){
			temp = scan;
			scan = scan->next;
			free_ident(temp);
		}
		/* scan is now the first element in the linked list
		/* that is not a formal parameter.
		 */
		hashtable[i] = scan;
	}
	funclevel--;
	stemp = f_stack;
	f_stack = f_stack->f_tl;
	free(stemp);
}

/* free the identifier symbol pointed to by p
 */
free_ident(p)
SYMPTR p;{
	free(p);
}
/* dump an identifiers information to the terminal
 */
dumpsym(p)
SYMPTR p;{
	printf("name:%s, lexlevel:%d, idno:%d, \n",
		p->name, p->lexlevel, p->idno);
	dumpattr(p);
}

void dumpattr(x)
SYMPTR x;
{
	if(x==NONE_A){
		printf("No attributes\n");
		return;
	}
	if( IS_DEF(x) ) printf("Defined ");
	if( IS_GLOB(x) ) printf("Global ");
	if( IS_LOCAL(x) ) printf("Local ");
	if( IS_FORM(x) ) printf("Formal ");
	if( IS_ELEM(x) ) printf("Elementary ");
	if( IS_USED(x) ) printf("Used Before as Glob");
	printf("\n");
}
