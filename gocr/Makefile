##
## Download + Build sequence for GOCR with COSI server support
##

VERSION := 0.50
PKGNAME := gocr-srv
PKGVERSION := $(VERSION)
PKGRELEASE := 2

URL = http://www-e.uni-magdeburg.de/jschulen/ocr
TARBALL = gocr-$(VERSION).tar.gz

SRCDIR = gocr-$(VERSION)
BIN = $(SRCDIR)/src/gocr

include ../defs.mk

DOCFILES = AUTHORS BUGS CREDITS HISTORY README READMEde.txt REMARK.txt REVIEW TODO gpl.html doc/gocr.html
DOCDIR = $(INSTALLDIR)/usr/share/doc/$(SRCDIR)

$(BIN): src
	(cd $(SRCDIR) && ./configure --prefix=$(INSTALLDIR)/usr)
	make -C $(SRCDIR) all doc

src: $(SRCDIR)/src/server.c

$(SRCDIR)/src/server.c: $(SRCDIR)/patches/series
	(cd $(SRCDIR) && quilt push -a)

$(SRCDIR)/patches/series: $(TARBALL)
	tar xfz $(TARBALL)
	(cd $(SRCDIR) && ln -sf ../patches .)

%.tar.gz:
	wget -N $(URL)/$@

install: $(BIN)
	$(RM) $(INSTALLDIR)
	make -C $(SRCDIR) install
	$(MKDIR) $(DOCDIR)
	(cd $(SRCDIR) && $(CP) $(DOCFILES) $(DOCDIR))
