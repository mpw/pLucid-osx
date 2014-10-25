#define copy(x,y) x = y

int l,c,peekc,errcount,in_index,true,false;

char noclashes,cconst,linebuf[200],buffer[500];

FILE *lexin, *outfile,*savelex;

YYSTYPE yylval;

struct {
	FILE *in_fdes;
	STRING in_name;
	int in_line;
} in_files[12];

STRING s,t;
