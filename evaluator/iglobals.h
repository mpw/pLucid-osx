
int false, true;
#define MAXFUNCS       105 
#define copy(x,y) x = y

FITEM ftable[MAXFUNCS];


/*
 *      io stuff, and trace flags
 */
#define MAXWORDS        400
#define MAXSTRINGS      400
#define MAXOPENFILES    20
#define MAXVARS         1000
#define AVGWRDLEN      100
#define MAXFILTERSPACE  400
char *infilename;
FILE *infile,*current_out,*current_in;
#define TRACEQUAN 100
int s_count,b_count,filterlevel,active_filters;
int dc1,dc2,dc3,c1,c2,c3;
int brecycle,srecycle,varcount,stpcount,memcount;
int eodch,current_var;
char evalerr,newout;
char fnl,sflag,cflag,iflag,fflag,nflag,dflag,pflag, tflags[TRACEQUAN];
char filterp[30];
int errcount,notfoundcount;
int open_file,file_var,current_strg,u_countvec[MAXVARS],retvec[MAXVARS];
int b_recquan,s_recquan,ddepth,current_inx,current_outx;
int curr_in_mode,curr_out_mode,curr_point,curr_ign,conc;
IOPTR io_info[MAXVARS],current_io;
EXPRPTR filter_e[100];
#define STPHASHSIZE 1113
#define PHASHSIZE 1113
#define THASHSIZE 1113
#define NHASHSIZE 1113
#define SHASHSIZE 1113
STPPTR phashtab[PHASHSIZE];
STPPTR thashtab[THASHSIZE];
STPPTR shashtab[SHASHSIZE];
MEMPTR nhashtab[NHASHSIZE];
int memargc;
char **memargv;
STPPTR outside; /* the time in the outside environment */


/*
 *      run time stacks:
 *              v_stack:        value stack
 *              d_stack:        display stack
 *              stp_stack:       space-time-place stack
 */

#define VSIZE 500
#define VStop           v_top
#define VSpush {if(v_top+1>= &v_stack[0]+2*VSIZE){printf("v_stack overflow\n"); exit(1); } v_top++; }
#define         VSpop           v_top--
#define         VStype          v_top->v_type
#define         VSvalue         v_top->v_value
VALUE v_stack[VSIZE];
VALUEPTR v_top;

#define DSIZE   2000
DISPLAYITEM     d_stack[DSIZE];
DISPLAYPTR d_top;

#define STPSIZE  6000
#define         STPStop          stp_top
#define STPSpush { if(stp_top+1>&stp_stack[0]+2*STPSIZE) { printf("stp_stack overflow\n");  my_exit(1); } stp_top++; }
#define         STPSpop          stp_top--
#define         STPSt            stp_top->stp_t
#define         STPSp            stp_top->stp_p
#define         STPSs            stp_top->stp_s
#define         STPSpushval(s,t,p) {STPSpush; STPSs = (s); STPSt = (t); STPSp = (p);}
STPPAIR stp_stack[STPSIZE], *stp_top;

unsigned filequan,exprquan,wordquan;
STRING *nametable,*wordtable;
EXPRPTR *filetable,*exprtable;
char *ngc_allocbuf,*ngc_allocp;
int ngc_allocsize;
/* STPPTR safe;  safe is used to hold a time in case */
int dynasizes[2];
int pid;
int ch ;
