#! make -f

e: e.c
	gcc -O2 -s -o e e.c -lm

VERSION = 0.02718
DISTFILES = EXAMPLES LICENSE GRAMMAR README.md e e.c makefile

dist: e
	mkdir e-$(VERSION)
	cp $(DISTFILES) e-$(VERSION)
	tar cvzf e-$(VERSION).tar.gz e-$(VERSION)

clean:
	rm -rf e e-$(VERSION) e-$(VERSION).tar.gz
