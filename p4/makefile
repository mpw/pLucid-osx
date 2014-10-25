DESTDIR=/usr/local/bin
CFLAGS=-O -DYYMAXDEPTH=900 -g -Wno-return-type -arch i386
LINT= lint -p
TAR= tar -cv
PRINT= pr -t $1 | cat -n
inc = cmanifs.h cglobals.h

pass4: main.o y.tab.o yylex.o expr.o walk.o;
	cc -arch i386 -g main.o expr.o y.tab.o yylex.o walk.o -o pass4

cp install:
	cp pass4 $R$(DESTDIR)/pass4
	rm -f *.o y.tab.h y.tab.c pass4

clean:
	rm -f *.o y.tab.h y.tab.c pass4

lint:   main.c expr.c yylex.c walk.c
	$(LINT) main.c expr.c yylex.c walk.c

tape:
	$(TAR) main.c expr.c yylex.c walk.c cmanifs.h cglobals.h tree.y

print:
	$(PRINT) main.c expr.c yylex.c walk.c cmanifs.h cglobals.h tree.y


all:    main.o y.tab.o yylex.o expr.o walk.o ;
	cc -arch i386 -g main.o expr.o y.tab.o yylex.o walk.o -o pass4
	cp pass4 $R$(DESTDIR)/pass4
	rm -f *.o y.tab.h y.tab.c pass4

main.o: $(inc) main.c
cmanifs.h: y.tab.h
y.tab.c y.tab.h: tree.y
	yacc -d tree.y
y.tab.o: $(inc) y.tab.h y.tab.c
yylex.o: $(inc) yylex.c
expr.o: $(inc) expr.c
walk.o: $(inc) walk.c
