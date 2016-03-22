##
## $Id: Makefile 42 2010-06-03 14:43:33Z giroudon $
##
## Download + Build sequence for Tesseract with COSI server support
##

VERSION = 2.04

URL = http://tesseract-ocr.googlecode.com/files
TARBALL = tesseract-$(VERSION).tar.gz

LNGDATA = 2.00.eng 2.00.fra 2.00.ita 2.00.deu 2.00.spa 2.00.nld 2.01.por
LNGTARBALLS = $(LNGDATA:%=tesseract-%.tar.gz)
LNGTARGETS = $(LNGDATA:

SRCDIR = tesseract-$(VERSION)
BIN = $(SRCDIR)/ccmain/tesseract

include ../defs.mk

trashdir="/TRASH"
bindir="/usr/bin"
datadir="/usr/share/tesseract"
libdir=$(trashdir)
includedir=$(trashdir)

$(BIN): src lng
	(cd $(SRCDIR) && ./configure --bindir=$(bindir) --datadir=$(datadir) --libdir=$(libdir)  --includedir=$(includedir))
	make -C $(SRCDIR)

src: $(SRCDIR)/tessdata/configs/server
lng: $(LNGTARBALLS)
	for file in $(LNGTARBALLS); do \
	  (cd $(SRCDIR) && tar xfz ../$$file) \
	done

$(SRCDIR)/tessdata/configs/server: $(SRCDIR)/patches/series
	(cd $(SRCDIR) && quilt push -a)

$(SRCDIR)/patches/series: $(TARBALL)
	tar xfz $(TARBALL)
	(cd $(SRCDIR) && ln -sf ../patches .)

%.tar.gz:
	wget -N $(URL)/$@

install: $(BIN)
	$(RM) $(INSTALLDIR)
	make -C $(SRCDIR) install DESTDIR=$(INSTALLDIR)
	$(RM) $(INSTALLDIR)/$(trashdir)
