#! make -f

VERSION     = 0.02718
DISTFILES   = EXAMPLES LICENSE GRAMMAR README.md Makefile e.c e
DISTDIR     = e-$(VERSION)
DISTARCHIVE = $(DISTDIR).tar.gz

e: Makefile e.c
	gcc -lm -o e e.c

dist: e
	mkdir $(DISTDIR)
	cp $(DISTFILES) $(DISTDIR)
	tar cvzf $(DISTARCHIVE) $(DISTDIR)

clean:
	rm -rf e $(DISTDIR) $(DISTARCHIVE)
