##
## Download + Build sequence for OCRAD with COSI server support
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

$(BIN): src
	(cd $(SRCDIR) && ./configure --prefix=$(INSTALLDIR)/usr)
	make -C $(SRCDIR) all doc

src: $(SRCDIR)/cosi.h

$(SRCDIR)/cosi.h: $(SRCDIR)/patches/series
	(cd $(SRCDIR) && quilt push -a)

$(SRCDIR)/patches/series: $(TARBALL)
	tar xfa $(TARBALL)
	(cd $(SRCDIR) && ln -sf ../patches .)

%.tar.lz:
	wget -N $(URL)/$@

install: $(BIN)
	$(RM) $(INSTALLDIR)
	make -C $(SRCDIR) install
	$(RM) $(INSTALLDIR)/usr/share/info/dir
	$(CP) ocrad-srv $(INSTALLDIR)/usr/bin/
