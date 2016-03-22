##
## $Id: defs.mk 40 2010-06-03 14:27:44Z giroudon $
##
## Common Makefile definitions for building COSI OCR agents and utilities
##

RPMDIR = $(shell pwd)/rpm
INSTALLDIR = $(RPMDIR)/INSTALL
spec=$(RPMDIR)/RPM.spec

RM = /bin/rm -rf
MKDIR = /bin/mkdir -p
MV = /bin/mv -fv
CP = /bin/cp -fv

all: $(BIN)

clean:
	$(RM) $(SRCDIR) $(INSTALLDIR)
distclean: clean
	$(RM) $(TARBALL)

rpm: install
	$(MKDIR) $(RPMDIR)/BUILD $(RPMDIR)/RPMS
	find $(INSTALLDIR) -type f | sed 's|^'$(INSTALLDIR)'||' > $(RPMDIR)/BUILD/RPM.files
	echo '%_topdir '$(RPMDIR) > $(HOME)/.rpmmacros
	rpmbuild -bb $(spec) --buildroot $(INSTALLDIR) --target `arch`
	find $(RPMDIR)/RPMS -name '*.rpm' -type f -exec $(MV) '{}' $(RPMDIR) ';'
