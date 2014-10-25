DESTDIR= /lucy/a/wwadge/nulu
CFLAGS=-O -DYYMAXDEPTH=900 -g -Wno-return-type -arch i386
LINT= lint -p
TAR= tar -cv
inc = cmanifs.h cglobals.h

pass1: main.o y.tab.o yylex.o expr.o ;
	cc -arch i386 -g -DYYMAXDEPTH=900 main.o expr.o y.tab.o yylex.o -o pass1

cp install:
	cp pass1 $R$(DESTDIR)/pass1
	rm -f *.o y.tab.h y.tab.c pass1

clean:
	rm -f *.o y.tab.h y.tab.c pass1

print:  

lint: main.c yylex.c expr.c
	$(LINT) main.c yylex.c expr.c

tape:
	$(TAR) main.c yylex.c expr.c cmanifs.h cglobals.h flucid.y

all:    main.o y.tab.o yylex.o expr.o ;
	cc -arch i386 -s -DYYMAXDEPTH=900 main.o expr.o y.tab.o yylex.o -o pass1
	cp pass1 $R$(DESTDIR)/pass1
	rm -f *.o y.tab.h y.tab.c pass1

main.o: $(inc) main.c
cmanifs.h: y.tab.h
y.tab.c y.tab.h: flucid.y
	yacc -d flucid.y
y.tab.o: $(inc) y.tab.h y.tab.c
yylex.o: $(inc) yylex.c
expr.o: $(inc) expr.c
