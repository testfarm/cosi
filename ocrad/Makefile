##
## COSI - The Common OCR Service Interface
## Download + Build sequence for OCRAD with COSI server support
##
## Copyright (C) 2007-2016 Sylvain Giroudon
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##
## The OCRAD tool and library are licensed under the GNU General Public License.
## See https://www.gnu.org/software/ocrad/ for further details.
##

NAME = ocrad
VERSION = 0.25
PKGNAME := ocrad-srv
PKGVERSION := $(VERSION)
PKGRELEASE := 1

URL = http://ftp.gnu.org/gnu/ocrad
TARBALL = $(NAME)-$(VERSION).tar.lz

SRCDIR = $(NAME)-$(VERSION)
BIN = $(SRCDIR)/$(NAME)

include ../defs.mk

.PHONY: src config

$(BIN): src config
	make -C $(SRCDIR) all doc

config: $(SRCDIR)/Makefile
$(SRCDIR)/Makefile:
	(cd $(SRCDIR) && ./configure --prefix=/usr)

src: $(SRCDIR)/cosi.h
$(SRCDIR)/cosi.h: $(SRCDIR)/patches/series
	(cd $(SRCDIR) && quilt push -a)

$(SRCDIR)/patches/series: $(TARBALL)
	tar xfa $(TARBALL)
	(cd $(SRCDIR) && ln -sf ../patches .)

%.tar.lz:
	wget -N $(URL)/$@

install: $(BIN)
	$(RM) $(DESTDIR)
	make -C $(SRCDIR) DESTDIR=$(DESTDIR) install
	$(RM) $(DESTDIR)/usr/share/info/dir $(DESTDIR)/usr/lib $(DESTDIR)/usr/include
	$(CP) ocrad-srv $(DESTDIR)/usr/bin/
