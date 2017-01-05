##
## Common Makefile definitions for building COSI OCR agents and utilities
##
##
## Copyright (C) 2007-2016 Sylvain Giroudon
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation; either version 2 of the
## License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
## General Public License for more details.
##

ROOTDIR := $(dir $(lastword $(MAKEFILE_LIST)))
OUTDIR := $(shell pwd)/out
PKGDIR := $(OUTDIR)
DESTDIR = $(OUTDIR)/root

RM = /bin/rm -rf
MKDIR = /bin/mkdir -p
MV = /bin/mv -fv
CP = /bin/cp -fv

all: $(BIN)

clean:
	$(RM) $(SRCDIR) $(DESTDIR)
distclean: clean
	$(RM) $(TARBALL)

include $(ROOTDIR)pkg.mk
