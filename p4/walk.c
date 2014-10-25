
#include "cmanifs.h"
#include "cglobals.h"

int lcount = 0;
int varcount= 0;
int wherec = 0;

void add_used(char *s);
void add_defined(char *s);
void add_used_list(EXPRPTR e);
void add_defined_list(EXPRPTR e);

pass4(x)
EXPRPTR x;
{ EXPRPTR remove_decls(),get_globals(),add_valof(),where_to_valof();
  EXPRPTR remove_evalofs(),extract_file_filter();
  output(
	 extract_file_filter(remove_evalofs(
	 get_globals(remove_decls(add_valof(where_to_valof(x)))))));
}

EXPRPTR
append(e1,e2)
EXPRPTR e1,e2;
{ EXPRPTR tmp,listnode();
  tmp = e1;
  if (e2==NULL) return(e1);
  if (e1==NULL) return(e2);
  tmp = listnode(tmp,e2->arg2.x);
  while (e2->arg1.x!=NULL) {
			     e2 = e2->arg1.x;
			     tmp = listnode(tmp,e2->arg2.x);
			    }
  return(tmp);
}


EXPRPTR
extract_file_filter(arg)
EXPRPTR arg;
{
  EXPRPTR valofnode(),opnode(),defnode(),exprlist2(),listnode();
  char is_decl(),name[40],*sv,*strsave();
  FFPTR s;
  EXPRPTR deflist,e,e1,e2,file,tmp;
  EXPRPTR declnode(),varnode(),new_valof(),new_evalof();
  switch(arg->f){
  case F_OP:     file = arg->arg4.x;
		 if (eqstring(arg->arg1.s,"file") ||
		     eqstring(arg->arg1.s,"filter")){
		     s = (FFPTR) calloc(1,sizeof(FFITEM));
		     sprintf(name,"_%d",arg->arg1.s,1000+varcount++);
		     sv = strsave(name);
		     s->name = sv;
		     s->rhs  = opnode(arg->arg1.s,arg->arg2.i,
				      extract_file_filter(arg->arg3.x),file);
		     s->next_d = new_def[fflevel];
		     new_def[fflevel] = s;
		     return(varnode(sv,0,NULL));
		 }
		 return(opnode(arg->arg1.s,arg->arg2.i,
		     extract_file_filter(arg->arg3.x),file));
  case F_VAR:    if (arg->arg2.i==0) return(arg);
		 return(varnode(arg->arg1.s,arg->arg2.i,
			 extract_file_filter(arg->arg3.x)));
  case F_DEFN:    return(defnode(arg->arg1.s,arg->arg2.i,arg->arg3.x,
			  extract_file_filter(arg->arg4.x)));
  case F_NGLOBALS:  return(arg);
  case F_EGLOBALS:  return(arg);
  case F_LISTNODE:  if (arg->arg1.x==NULL)
		    return(listnode(NULL,extract_file_filter(arg->arg2.x)));
		    return(listnode(extract_file_filter(arg->arg1.x),
				    extract_file_filter(arg->arg2.x)));
  case F_CONST:  return(arg);
  case F_VALOF:
		fflevel++;
		tmp = extract_file_filter(arg->arg1.x);
		deflist = NULL;
		for (s=new_def[fflevel];s!=NULL;s=s->next_d){
		   deflist=listnode(deflist,defnode(s->name,0,NULL,s->rhs)); }
		fflevel--;
		return(valofnode(append(tmp,deflist)));
  default:       fprintf(stderr,"error in parse tree\n");
  }
}

EXPRPTR
remove_evalofs(arg)
EXPRPTR arg;
{
  EXPRPTR valofnode(),opnode(),defnode(),exprlist2(),listnode();
  char is_decl(),name[30],*s;
  EXPRPTR e,e1,e2,file;
  EXPRPTR declnode(),varnode(),new_valof(),new_evalof();
  switch(arg->f){
  case F_OP:     file = arg->arg4.x;
		 return(opnode(arg->arg1.s,arg->arg2.i,
		     remove_evalofs(arg->arg3.x),file));
  case F_VAR:    if (arg->arg2.i==0) return(arg);
		 return(varnode(arg->arg1.s,arg->arg2.i,
			 remove_evalofs(arg->arg3.x)));
  case F_DEFN:    return(defnode(arg->arg1.s,arg->arg2.i,arg->arg3.x,
			  remove_evalofs(arg->arg4.x)));
  case F_NGLOBALS:  return(arg);
  case F_LISTNODE:  if (arg->arg1.x==NULL)
			  return(listnode(NULL,remove_evalofs(arg->arg2.x)));
		    return(listnode(remove_evalofs(arg->arg1.x),
				    remove_evalofs(arg->arg2.x)));
  case F_CONST:  return(arg);
  case F_EVALOF:
		return(new_valof(remove_evalofs(arg->arg1.x)));
  case F_VALOF:
		return(valofnode(remove_evalofs(arg->arg1.x)));
  default:       fprintf(stderr,"error in parse tree\n");
  }
}

EXPRPTR
new_valof(e)
EXPRPTR e;
{ EXPRPTR replace_ngs(),valofnode(),listnode(),identlistnode();
  EXPRPTR glbs,tmp,dec,result,idlist,save;
  tmp = e;
  idlist = NULL;
  glbs = NULL;
  result = NULL;
  dec = e->arg2.x;
  while ( tmp !=NULL ) { if (dec->f ==F_NGLOBALS) {
			    glbs = listnode(glbs,dec); }
			 else {
			 if (dec->f==F_DEFN&&!eqstring("_result",dec->arg1.s)){
			   idlist = identlistnode(idlist,dec->arg1.s); }
			 if (dec->f==F_DEFN&&eqstring("_result",dec->arg1.s))
				     { save = dec->arg4.x ; } else
				     result = listnode(result,dec); }
				     tmp = tmp->arg1.x;
				     if (tmp!=NULL) dec = tmp->arg2.x;
				   }
  result = append(glbs,listnode(result,replace_ngs(idlist,save)));
  return(valofnode(result));
}

EXPRPTR
replace_ngs(idlist,e)
EXPRPTR idlist,e;
{ EXPRPTR rremove(),valofnode(),listnode(),eglobnode(),defnode();
  EXPRPTR glbs,tmp, result,dec,rem_t;
  result = NULL;
  tmp = e->arg1.x;
  dec = tmp->arg2.x;
  glbs = listnode(NULL,eglobnode(idlist));
  while ( tmp != NULL) { if (dec->f==F_NGLOBALS)
			    { rem_t  = rremove(dec,idlist);
			      if (rem_t!=NULL)
			      glbs = listnode(glbs,rem_t);        }
			 else result = listnode(result,dec);
				     tmp = tmp->arg1.x;
				     if (tmp!=NULL) dec = tmp->arg2.x;

		       }

  return(defnode("_result",0,NULL,valofnode(append(glbs,result))));
}

EXPRPTR
rremove(e,idlist)
EXPRPTR e,idlist;
{ EXPRPTR oldlist,newlist,nglobnode(),remlist;
  char found,*s ;
  newlist = NULL;
  oldlist = e->arg1.x;
  while( oldlist !=NULL) {
	   s = oldlist -> arg2.s;
	   remlist = idlist;
	   found = false;
	   while (remlist !=NULL ) {
		  if (eqstring(remlist->arg2.s,s))
		       found = true;
		  remlist = remlist -> arg1.x; }
     if (!found) newlist = identlistnode(newlist,s);
	   oldlist = oldlist -> arg1.x ; }
  if (newlist==NULL) return(NULL);
  return(nglobnode(newlist));
}



EXPRPTR
remove_decls(arg)
EXPRPTR arg;
{
  EXPRPTR valofnode(),opnode(),defnode(),exprlist2(),listnode();
  char is_decl(),name[30],*s;
  EXPRPTR e,e1,e2,file;
  EXPRPTR declnode(),varnode(),new_evalof();
  switch(arg->f){
  case F_OP:     file = arg->arg4.x;
		 return(opnode(arg->arg1.s,arg->arg2.i,
		     remove_decls(arg->arg3.x),file));
  case F_VAR:    if (arg->arg2.i==0) return(arg);
		 return(varnode(arg->arg1.s,arg->arg2.i,
			 remove_decls(arg->arg3.x)));
  case F_DEFN:    return(defnode(arg->arg1.s,arg->arg2.i,arg->arg3.x,
			  remove_decls(arg->arg4.x)));
  case F_LISTNODE:  if (arg->arg1.x==NULL)
			  return(listnode(NULL,remove_decls(arg->arg2.x)));
		    return(listnode(remove_decls(arg->arg1.x),
				    remove_decls(arg->arg2.x)));
  case F_CONST:  return(arg);
  case F_VALOF:
		if (is_decl(arg->arg1.x))
		    { return(new_evalof(arg->arg1.x)); }
		return(valofnode(remove_decls(arg->arg1.x)));
  default:       fprintf(stderr,"error in parse tree\n");
  }
}

EXPRPTR
new_evalof(e)
EXPRPTR e;
{ EXPRPTR evalofnode(),defnode(),valofnode(),listnode(),varnode();
  EXPRPTR tmp,dec,ev_list,v_list,v_def,ev_def,remove_decls();
  char *s,name[30];
  tmp = e;
  ev_list = NULL;
  v_list = NULL;
  dec = e->arg2.x;
  while ( tmp !=NULL ) {     if (dec->f==F_DECL){
				     sprintf(name,"_%d",1000+varcount++);
				     s      = strsave(name);
				     ev_def = defnode(s,0,NULL,
						  remove_decls(dec->arg2.x));
				     v_def = defnode(dec->arg1.s,0,NULL,
						     varnode(s,0,NULL));
				     ev_list = listnode(ev_list,ev_def);}
				  else  v_def = remove_decls(dec);
					 v_list = listnode(v_list,v_def);
				     tmp = tmp->arg1.x;
				     if (tmp!=NULL) dec = tmp->arg2.x;
				   }
  ev_def = defnode("_result",0,NULL,valofnode(v_list));
  return(evalofnode(listnode(ev_list,ev_def)));
}

EXPRPTR
get_globals(arg)
EXPRPTR arg;
{
  EXPRPTR valofnode(),opnode(),defnode(),listnode();
  EXPRPTR new_ng_nglobals();
  char name[30],*s;
  EXPRPTR e,e1,e2,file,tmp;
  EXPRPTR declnode(),varnode();
  switch(arg->f){
  case F_OP:   file = arg->arg4.x;
		 return(opnode(arg->arg1.s,arg->arg2.i,
			get_globals(arg->arg3.x),file));
  case F_VAR:
	       add_used(arg->arg1.s);
	       if (arg->arg2.i==0) return(arg);
		 return(varnode(arg->arg1.s,arg->arg2.i,
			 get_globals(arg->arg3.x)));
  case F_DEFN:
		 add_defined(arg->arg1.s);
		 if (arg->arg2.i>0) {  funclevel++;
				       add_formals(arg->arg3.x); }
		 tmp=(defnode(arg->arg1.s,arg->arg2.i,arg->arg3.x,
			  get_globals(arg->arg4.x)));
		 if (arg->arg2.i>0) { remove_formals();
				      funclevel--; }
		 return(tmp);
  case F_NGLOBALS:
  case F_EGLOBALS:   add_defined_list(arg->arg1.x);
		    return(arg);
  case F_LISTNODE:  if (arg->arg1.x==NULL)
			  return(listnode(NULL,get_globals(arg->arg2.x)));
		    return(listnode(get_globals(arg->arg1.x),
				    get_globals(arg->arg2.x)));
  case F_CONST:   return(arg);
  case F_EVALOF:
		  valoflevel++;
		  formals_list[valoflevel]=NULL;
		  used_list[valoflevel]=NULL;
		  defined_list[valoflevel] = NULL;
		  tmp =(new_ng_nglobals(F_EVALOF,
					get_globals(arg->arg1.x)));
		  valoflevel--;
		  return(tmp);
  case F_VALOF:
		  valoflevel++;
		  formals_list[valoflevel]=NULL;
		  used_list[valoflevel]=NULL;
		  defined_list[valoflevel] = NULL;
		  tmp =(new_ng_nglobals(F_VALOF,
					get_globals(arg->arg1.x)));
		  valoflevel--;
		  return(tmp);
  default:        fprintf(stderr,"error in add_globals()\n");
		  return(arg);
  }
}

EXPRPTR
new_ng_nglobals(type,e)
int type;
EXPRPTR e;
{  EXPRPTR valofnode(),listnode(),nglobnode(),tmp,append(),evalofnode();
   EXPRPTR identlistnode();
   char is_defined();
   if (valoflevel==1) return(valofnode(e));
   tmp = used_list[valoflevel];
   new_decls[valoflevel]=NULL;
   if (tmp!=NULL) {
   if (!is_defined(tmp->arg2.s))
       new_decls[valoflevel]=identlistnode(new_decls[valoflevel],tmp->arg2.s);
   while(tmp->arg1.x!=NULL) {
       tmp=tmp->arg1.x;
       if (!is_defined(tmp->arg2.s))
       new_decls[valoflevel]=identlistnode(new_decls[valoflevel],tmp->arg2.s);
       }
   }
   if (new_decls[valoflevel]!=NULL)
   { tmp = append(listnode(NULL,nglobnode(new_decls[valoflevel])),e);}    else tmp=e;
   if (new_decls[valoflevel]!=NULL) export_nglobals();
   new_decls[valoflevel]=NULL;
   if (type == F_EVALOF)
   return(evalofnode(tmp));
   return(valofnode(tmp));
}

export_nglobals()
{  valoflevel--;
   add_used_list(new_decls[valoflevel+1]);
   valoflevel++;
}

void add_used_list(e)
EXPRPTR e;
{ EXPRPTR tmp;
  tmp = e;
  if ( tmp==NULL) return;
  add_used(tmp->arg2.s);
  while (tmp->arg1.x!=NULL){
			     tmp=tmp->arg1.x;
			     add_used(tmp->arg2.s);
			   }
}

char
is_defined(s)
char *s;
{ EXPRPTR tmp;
  tmp = defined_list[valoflevel];
  if (tmp==NULL) return(false);
  if (eqstring(s,tmp->arg2.s)) return(true);
  while ( tmp->arg1.x!=NULL ) {
			  tmp = tmp -> arg1.x;
			  if (eqstring(s,tmp->arg2.s)) return(true);
	      }
  return(false);
}

void add_defined_list(e)
EXPRPTR e;
{ EXPRPTR tmp;
  tmp = e;
  if ( tmp==NULL) return;
  add_defined(tmp->arg2.s);
  while (tmp->arg1.x!=NULL){
			     tmp=tmp->arg1.x;
			     add_defined(tmp->arg2.s);
    }
}

remove_formals()
{ formals_list[valoflevel]=NULL ; }

add_formals(e)
EXPRPTR e;
{ formals_list[valoflevel]=e; }


void add_used(s)
char *s;
{ EXPRPTR tmp,identlistnode();
  tmp = formals_list[valoflevel];
  if ( tmp!=NULL) {
  if (eqstring(s,tmp->arg2.s)) return;
  while (tmp->arg1.x!=NULL) {
	      tmp = tmp->arg1.x;
	      if (eqstring(s,tmp->arg2.s)) return;
	  }
  }
  tmp = used_list[valoflevel];
  if (tmp==NULL) { used_list[valoflevel]=identlistnode(NULL,s);
		   return; }
  if (eqstring(s,tmp->arg2.s)) return;
  while (tmp->arg1.x!=NULL) {
	      tmp = tmp->arg1.x;
	      if (eqstring(s,tmp->arg2.s)) return;
	   }
  used_list[valoflevel]=identlistnode(used_list[valoflevel],s);
}

void add_defined(s)
char *s;
{ EXPRPTR tmp,identlistnode();
  tmp = defined_list[valoflevel];
  if (tmp==NULL) { defined_list[valoflevel]=identlistnode(NULL,s);
		   return;}
  if (eqstring(s,tmp->arg2.s)) return;
  while (tmp->arg1.x!=NULL) {
	      tmp = tmp->arg1.x;
	      if (eqstring(s,tmp->arg2.s)) return;
     }
  defined_list[valoflevel]=identlistnode(defined_list[valoflevel],s);
}

EXPRPTR
mkwhere(e)
EXPRPTR e;
{ EXPRPTR varnode(),listnode(),wherenode(),defnode();
  char name[50];
  sprintf(name,"_%d",1000+varcount++);
  return(wherenode(varnode(name,0,NULL),
		   listnode(NULL,defnode(name,0,NULL,e))
		  )
	);
}

EXPRPTR
where_to_valof(arg)
EXPRPTR arg;
{
  EXPRPTR valofnode(),opnode(),defnode(),exprlist2(),listnode();
  char *s;
  EXPRPTR e,e1,e2,file;
  EXPRPTR declnode(),varnode(),mkwhere();
  char *strsave(),*res, name[100];
  switch(arg->f){
  case F_OP:     file = arg->arg4.x;
		 return(opnode(arg->arg1.s,arg->arg2.i,
		     where_to_valof(arg->arg3.x),file));
  case F_VAR:    if (arg->arg2.i==0) return(arg);
		 return(varnode(arg->arg1.s,arg->arg2.i,
			 where_to_valof(arg->arg3.x)));
  case F_DECL:   return(declnode(arg->arg1.s,where_to_valof(arg->arg2.x)));
  case F_DEFN: if (arg->arg2.i>0 && arg->arg4.x->f != F_WHERE)
		   arg->arg4.x = mkwhere(arg->arg4.x);
	       return(defnode(arg->arg1.s,arg->arg2.i,arg->arg3.x,
			  where_to_valof(arg->arg4.x)));
  case F_LISTNODE:  if (arg->arg1.x==NULL)
			  return(listnode(NULL,where_to_valof(arg->arg2.x)));
		    return(listnode(where_to_valof(arg->arg1.x),
				    where_to_valof(arg->arg2.x)));
  case F_CONST:  return(arg);
  case F_WHERE:
		res = strsave("_result");
		return(valofnode( listnode(where_to_valof(arg->arg2.x),
				   defnode(res,0,NULL,
				   where_to_valof(arg->arg1.x)))
				  ));
  default:       fprintf(stderr,"error in parse tree\n");
  }
}

EXPRPTR add_valof(e)
EXPRPTR e;
{  EXPRPTR file,declnode(),varnode(),listnode(),valofnode();
   char *strsave(),is_decl(),*res,name[100];
   switch(e->f){
    case F_VAR:
		 break;
    case F_OP:   file = e->arg4.x;
		 break;
    case F_CONST:
		 break;
    case F_VALOF:
		  if (is_decl(e->arg1.x)) {
		       res = strsave("_result");
		       return(valofnode( listnode(NULL,
					  defnode(res,0,NULL,e)))); }
		 return(e);
    default:     fprintf(stderr,"error in add_valof():%d:\n",e->f);
		 return(e);
   }
   res = strsave("_result");
   return(valofnode( listnode(NULL,defnode(res,0,NULL,e))));
}

char
is_decl(e)
EXPRPTR e;
{ if (e->arg1.x==NULL) return(e->arg2.x->f==F_DECL);
  return(is_decl(e->arg1.x) || e->arg2.x->f==F_DECL);
}
