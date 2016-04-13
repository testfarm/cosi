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
SHAREDIR = $(INSTALLDIR)/usr/share
DOCDIR = $(SHAREDIR)/doc/gocr
BINDIR = $(INSTALLDIR)/usr/bin

.PHONY: src config

$(BIN): src config
	make -C $(SRCDIR) all doc

config: $(SRCDIR)/config.status
$(SRCDIR)/config.status:
	(cd $(SRCDIR) && ./configure --prefix=/usr)

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
	$(MKDIR) $(BINDIR) $(SHAREDIR) $(DOCDIR)
	make -C $(SRCDIR)/src DESTDIR=$(INSTALLDIR)/usr install
	make -C $(SRCDIR)/man DESTDIR=$(SHAREDIR) install
	$(CP) $(DOCFILES:%=$(SRCDIR)/%) $(DOCDIR)/
	$(CP) gocr-srv $(BINDIR)
