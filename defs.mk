##
## Common Makefile definitions for building COSI OCR agents and utilities
##

ROOTDIR := $(dir $(lastword $(MAKEFILE_LIST)))
PKGDIR := $(shell pwd)/out
INSTALLDIR = $(PKGDIR)/root

RM = /bin/rm -rf
MKDIR = /bin/mkdir -p
MV = /bin/mv -fv
CP = /bin/cp -fv

all: $(BIN)

clean:
	$(RM) $(SRCDIR) $(INSTALLDIR)
distclean: clean
	$(RM) $(TARBALL)

include $(ROOTDIR)pkg.mk
