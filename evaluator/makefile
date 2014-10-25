R=
DESTDIR=/usr/local/bin
CFLAGS=-O -DYYMAXDEPTH=900 -g -Wno-return-type -arch i386

LINT= lint -p
TAR= tar -cv
mac = imanifs.h iglobals.h

luval: main.o f_flucid.o input.o dynamic.o util.o \
		      dump.o rupture.o memory.o f_decls.o \
		      f_alg.o f_lucid.o f_vars.o string.o list.o real.o
	cc -arch i386  $(CFLAGS) main.o f_flucid.o memory.o input.o dynamic.o \
		util.o dump.o rupture.o f_decls.o f_alg.o \
		f_lucid.o f_vars.o string.o real.o list.o -lm \
		-o luval

cp install: luval
	cp luval $R$(DESTDIR)/luval
	rm luval *.o

clean:
	rm -f *.o luval

lint:   main.c input.c dynamic.c util.c f_flucid.c \
	dump.c rupture.c memory.c f_decls.c \
	f_alg.c f_lucid.c f_vars.c string.c list.c real.c
	$(LINT) main.c input.c dynamic.c util.c f_flucid.c\
	dump.c rupture.c memory.c f_decls.c \
	f_alg.c f_lucid.c f_vars.c string.c list.c real.c

tape:
	$(TAR) imanifs.h iglobals.h main.c input.c dynamic.c util.c \
	dump.c rupture.c memory.c f_decls.c f_flucid.c\
	f_alg.c f_lucid.c f_vars.c string.c list.c real.c

print:
	for i in *.[ch] do 1page $i done | lpr 

all: main.o input.o dynamic.o util.o f_flucid.o\
		      dump.o rupture.o memory.o f_decls.o \
		      f_alg.o f_lucid.o f_vars.o string.o list.o real.o
	cc  -arch i386  $(CFLAGS) main.o memory.o input.o dynamic.o \
		util.o dump.o rupture.o f_decls.o f_alg.o \
		f_lucid.o f_vars.o string.o f_flucid.o list.o real.o -lm \
		-o luval
	cp luval $R$(DESTDIR)/luval
	rm -f *.o luval

main.o: $(mac) main.c
input.o: $(mac) input.c
dynamic.o: $(mac) dynamic.c
util.o: $(mac) util.c
dump.o: $(mac) dump.c
rupture.o: $(mac) rupture.c
memory.o: $(mac) memory.c
f_decls.o: $(mac) f_decls.c
f_alg.o: $(mac) f_alg.c
f_lucid.o: $(mac) f_lucid.c
f_flucid.o: $(mac) f_flucid.c
f_vars.o: $(mac) f_vars.c
string.o: $(mac) string.c
list.o: $(mac) list.c
real.o: $(mac) real.c
