#define copy(x,y) x = y

int ffcount,fflevel,funclevel,f_level[100],valoflevel,currentlevel;
EXPRPTR used_list[100],defined_list[100],new_defs[100],new_decls[100];
EXPRPTR formals_list[100];
FFPTR new_def[100];
int l,c,peekc,errcount,in_index,true,false;

char cconst,linebuf[200],buffer[500];

FILE *lexin, *outfile,*savelex;

YYSTYPE yylval;

struct {
	FILE *in_fdes;
	STRING in_name;
	int in_line;
} in_files[12];

STRING s,t,largest;
