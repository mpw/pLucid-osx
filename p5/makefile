R=
DESTDIR=/usr/local/bin
CFLAGS=-O -DYYMAXDEPTH=900 -g -Wno-return-type -arch i386

LINT= lint -p
TAR= tar -cv
PRINT= pr -t $1 | cat -n
inc = cmanifs.h cglobals.h

pass5: main.o y.tab.o yylex.o expr.o ident.o;
	cc -arch i386 -g main.o expr.o y.tab.o yylex.o ident.o -o pass5

cp install:
	cp pass5 $R$(DESTDIR)/pass5
	rm -f *.o y.tab.h y.tab.c pass5

clean:
	rm -f *.o y.tab.h y.tab.c pass5

lint:   main.c expr.c yylex.c ident.c
	$(LINT) main.c expr.c yylex.c ident.c

tape:
	$(TAR) main.c expr.c yylex.c ident.c cmanifs.h cglobals.h tree.y

print:
	$(PRINT) main.c expr.c yylex.c ident.c cmanifs.h cglobals.h tree.y


all:    main.o y.tab.o yylex.o expr.o ident.o ;
	cc -g -arch i386 main.o expr.o y.tab.o yylex.o ident.o -o pass5
	cp pass5 $R$(DESTDIR)/pass5
	rm -f *.o y.tab.h y.tab.c pass5

.c.o:
	cc $(CFLAGS) -c -g $<

main.o: $(inc) main.c
cmanifs.h: y.tab.h
y.tab.c y.tab.h: tree.y
	yacc -d tree.y
y.tab.o: $(inc) y.tab.h y.tab.c
yylex.o: $(inc) yylex.c
expr.o: $(inc) expr.c
ident.o: $(inc) ident.c
