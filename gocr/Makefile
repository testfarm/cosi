##
## COSI - The Common OCR Service Interface
## Download + Build sequence for GOCR with COSI server support
##
## Copyright (C) 2007-2016 Sylvain Giroudon
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##
## The GOCR tool and library are licensed under the GNU General Public License.
## See http://jocr.sourceforge.net/ for further details.
##

APP_VERSION := 0.50
PKGNAME := gocr-srv
PKGVERSION_PREFIX := $(APP_VERSION)-

URL = http://www-e.uni-magdeburg.de/jschulen/ocr
TARBALL = gocr-$(APP_VERSION).tar.gz

SRCDIR = gocr-$(APP_VERSION)
BIN = $(SRCDIR)/src/gocr

include ../defs.mk

DOCFILES = AUTHORS BUGS CREDITS HISTORY README READMEde.txt REMARK.txt REVIEW TODO gpl.html doc/gocr.html
SHAREDIR = $(DESTDIR)/usr/share
DOCDIR = $(SHAREDIR)/doc/gocr
BINDIR = $(DESTDIR)/usr/bin

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
	$(RM) $(DESTDIR)
	$(MKDIR) $(BINDIR) $(SHAREDIR) $(DOCDIR)
	make -C $(SRCDIR)/src DESTDIR=$(DESTDIR)/usr install
	make -C $(SRCDIR)/man DESTDIR=$(SHAREDIR) install
	$(CP) $(DOCFILES:%=$(SRCDIR)/%) $(DOCDIR)/
	$(CP) gocr-srv $(BINDIR)
