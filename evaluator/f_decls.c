#include "imanifs.h"
#include "iglobals.h"
int f_all();
int f_elt();
int f_first();
int f_next();
int f_pred();
int f_prev();
int f_fby();
int f_before();
int f_asa();
int f_now();
int f_attime();
int f_initial();
int f_succ();
int f_cby();
int f_sby();
int f_whr();
int f_here();
int f_atspace();
int f_noriginal();
int f_nrest();
int f_original();
int f_rest();
int f_aby();
int f_swap();
int f_lshift();
int f_rshift();

int f_chr();
int f_isatom();
int f_ord();
int f_hd();
int f_tl();
int f_cons();
int f_nil();
int f_islist();
int f_isnil();
int f_exp();
int f_arg();
int f_filter();
int f_log10();
int f_abs();
int f_tan();
int f_sqrt();
int f_cxfile();
int f_index();
int f_mknumber();
int f_length();
int f_iserror();
int f_error();
int f_input();
int f_isnumber();
int f_uminus();
int f_not();
int f_and();
int f_or();
int f_num2();
int f_log2();
int f_eq();
int f_ne();
int f_const();
int f_word();
int f_swchar();
int f_if();
int f_var();
int f_local();
int f_fcall();
int f_eres();
int f_cos();
int f_sin();
int f_log();
int f_iseod();
int f_scons();
int f_isstring();
int f_isword();
int f_strconc();
int f_substr();
int f_append();
int f_eod();
int f_mkword();
int f_mkstring();
int f_real();
int f_imag();
int f_complex();

FITEM ftable[] = {
	{"eres",        1,	f_eres,		TERMINAL},
        {"var",         3,	f_var,		TERMINAL},
	{"input",       1,	f_input,	TERMINAL},
	{"local",       1,	f_local,	TERMINAL},
	{"word",        1,	f_word,		TERMINAL},
	{"fcall",       4,	f_fcall,	TERMINAL},
	{"swchar",      1,	f_swchar,	TERMINAL},
	{"const",	1,	f_const,	TERMINAL},
        {"nil",		1,	f_nil,		TERMINAL},
        {"cxfile",      4,	f_cxfile,	TERMINAL},
	{"scons",       3,	f_scons,	INTERIOR},
        {"wrd",         1,	f_error,	TERMINAL},
        {"valof",       1,	f_error,	TERMINAL},
        {"decl",        1,	f_error,	TERMINAL},
        {"defn",        1,	f_error,	TERMINAL},
        {"strg",        1,	f_error,	TERMINAL},
        {"numb",        1,	f_error,	TERMINAL},
        {"nullry",      1,	f_error,	TERMINAL},
        {"nonnullry",   1,	f_error,	TERMINAL},
        {"constant",    1,	f_error,	TERMINAL},
	{"eglobal",     1,	f_error,	TERMINAL},
	{"nglobal",     1,	f_error,	TERMINAL},
	{"frmls",       1,	f_error,	TERMINAL},
	{"op",          1,	f_error,	TERMINAL},
	{"first",	2,	f_first,	INTERIOR},
	{"next",	2,	f_next,		INTERIOR},
	{"pred",	2,	f_pred,		INTERIOR},
	{"prev",	2,	f_prev,		INTERIOR},
	{"fby", 	3,	f_fby,		INTERIOR},
	{"before",	3,	f_before,	INTERIOR},
	{"asa",	        3,	f_asa,		INTERIOR},
	{"now",		1,	f_now,		INTERIOR},
	{"@t",		3,	f_attime,	INTERIOR},
	{"initial",	2,	f_initial,	INTERIOR},
	{"succ",	2,	f_succ,		INTERIOR},
	{"sby", 	3,	f_sby,		INTERIOR},
	{"whr", 	3,	f_whr,		INTERIOR},
	{"here", 	1,	f_here,		INTERIOR},
	{"@s",		3,	f_atspace,	INTERIOR},
	{"original",	2,	f_original,	INTERIOR},
	{"noriginal",	2,	f_noriginal,	INTERIOR},
	{"nrest",	2,	f_nrest,	INTERIOR},
	{"rest",	2,	f_rest,		INTERIOR},
	{"aby", 	3,	f_aby,		INTERIOR},
	{"cby", 	3,	f_cby,		INTERIOR},
	{"swap", 	3,	f_swap,		INTERIOR},
	{"rshift",	2,	f_rshift,	INTERIOR},
	{"lshift",	2,	f_lshift,	INTERIOR},
	{"all", 	2,	f_all,  	INTERIOR},
	{"elt", 	2,	f_elt,  	INTERIOR},
	{"isnumber",	2,	f_isnumber,	INTERIOR},
	{"div",		3,	f_num2,		INTERIOR},
	{"mod",		3,	f_num2,		INTERIOR},
	{"and",		3,	f_and,		INTERIOR},
	{"not",		2,	f_not,		INTERIOR},
	{"or",		3,	f_or,		INTERIOR},
	{"eq",		3,	f_eq,		INTERIOR},
	{"ne",		3,	f_ne,		INTERIOR},
	{"gt",		3,	f_log2,		INTERIOR},
	{"ge",		3,	f_log2,		INTERIOR},
	{"le",		3,	f_log2,		INTERIOR},
	{"lt",		3,	f_log2,		INTERIOR},
	{"sin",		2,	f_sin,		INTERIOR},
	{"cos",		2,	f_sin,		INTERIOR},
	{"log",		2,	f_log,		INTERIOR},
	{"iseod",	2,	f_iseod,	INTERIOR},
	{"isstring",	2,	f_isstring,	INTERIOR},
	{"isword",	2,	f_isword,	INTERIOR},
	{"substr",	4,	f_substr,	INTERIOR},
	{"eod",		1,	f_eod,		TERMINAL},
	{"arg",		2,	f_arg,		INTERIOR},
	{"mkword",	2,	f_mkword,	INTERIOR},
	{"mkstring",	2,	f_mkstring,	INTERIOR},
	{"error",	1,	f_error,	TERMINAL},
	{"iserror",	2,	f_iserror,	INTERIOR},
	{"length",	2,	f_length,	INTERIOR},
	{"mknumber",	2,	f_mknumber,	INTERIOR},
	{"tan",		2,	f_tan,		INTERIOR},
	{"log10",	2,	f_log10,	INTERIOR},
	{"abs",		2,	f_abs,		INTERIOR},
	{"sqrt",	2,	f_sqrt,		INTERIOR},
	{"filter",	4,	f_filter,	INTERIOR},
        {"cons",	3,	f_cons,		INTERIOR}, 
        {"islist",	2,	f_islist,	INTERIOR},
        {"isatom",	2,	f_isatom,	INTERIOR},
        {"hd",		2,	f_hd,		INTERIOR},
        {"tl",		2,	f_tl,		INTERIOR},
        {"ord",		2,	f_ord,		INTERIOR},
        {"chr",		2,	f_chr,		INTERIOR},
        {"isnil",	2,	f_isnil,	INTERIOR},
        {"complex",	2,	f_complex,	INTERIOR},
        {"real",	2,	f_real,		INTERIOR},
        {"imag",	2,	f_imag,		INTERIOR},
	{"uminus",      2,	f_uminus,	INTERIOR},
	{"plus",        3,	f_num2, 	INTERIOR},
	{"minus",       3,	f_num2, 	INTERIOR},
	{"times",       3,	f_num2, 	INTERIOR},
	{"if",          4,	f_if,		INTERIOR},
	{"fdiv",        3,	f_num2,		INTERIOR},
	{"strconc",     3,	f_strconc,	INTERIOR},
	{"append",      3,	f_append,	INTERIOR},
	{"arg",         2,	f_arg,		INTERIOR},
	{"exp",         3,	f_exp,		INTERIOR},
};
