R=
DESTDIR=/usr/local/bin
CFLAGS=-O -DYYMAXDEPTH=900 -g -Wno-return-type -arch i386
LINT= lint -p
TAR= tar -cv
PRINT= pr -t $1 | cat -n
inc = cmanifs.h cglobals.h

pass3: main.o y.tab.o yylex.o expr.o ;
	cc -arch i386 -s main.o expr.o y.tab.o yylex.o -o pass3

cp install:
	cp pass3 $R$(DESTDIR)/pass3
	rm -f *.o y.tab.h y.tab.c pass3

clean:
	rm -f *.o y.tab.h y.tab.c pass3

lint:   main.c expr.c yylex.c
	$(LINT) main.c expr.c yylex.c

tape:
	$(TAR) main.c expr.c yylex.c cmanifs.h cglobals.h tree.y

print:
	$(PRINT) main.c expr.c yylex.c cmanifs.h cglobals.h tree.y


all:    main.o y.tab.o yylex.o expr.o ;
	cc -arch i386 -s main.o expr.o y.tab.o yylex.o -o pass3
	cp pass3 $R$(DESTDIR)/pass3
	rm -f *.o y.tab.h y.tab.c pass3

main.o: $(inc) main.c
cmanifs.h: y.tab.h
y.tab.c y.tab.h: tree.y
	yacc -d tree.y
y.tab.o: $(inc) y.tab.h y.tab.c
yylex.o: $(inc) yylex.c
expr.o: $(inc) expr.c
