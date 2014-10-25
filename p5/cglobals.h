
int filecount,l,c,peekc,errcount,in_index,true,false;

char wordval,cconst,linebuf[200],buffer[500];

FILE *lexin, *outfile,*savelex;

STRING s,t;
#define MAXFUNCS        104
#define copy(x,y) x = y

FITEM ftable[MAXFUNCS];

F_STACKPTR f_stack;
P_STACKPTR p_stack;
E_STACKPTR parm_stack;
E_LISTPTR parm_list;


YYSTYPE yylval;
int lexlevel;
int funclevel;
int elemlevel;
int identcount;
int formalcount;
int idusage; /* is ident being declared, used, or what? */
#define HASHSIZE        120
SYMPTR hashtable[HASHSIZE+1];

FILE *lexin, *outfile;

struct {
	FILE *in_fdes;
	STRING in_name;
	int in_line;
} in_files[12];

#define MAXSTRINGS        200
STRING stringtable[MAXSTRINGS];
STRING wordtable[MAXSTRINGS];
int stringcount,wordcount;
#define MAXVARS         400
#define MAXFILES       1000
EXPRPTR exprtable[MAXVARS]; /* the expressions trees for each variable */
STRING  nametable[MAXVARS]; /* the user names for each variable */
EXPRPTR filetable[MAXFILES];
