##
## $Id: defs.mk 40 2010-06-03 14:27:44Z giroudon $
##
## Common Makefile definitions for building COSI OCR agents and utilities
##

PKGDIR = $(shell pwd)/out
INSTALLDIR = $(PKGDIR)/root
spec=RPM.spec

DEBARCH = $(shell dpkg-architecture -qDEB_HOST_ARCH)

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
	$(MKDIR) $(PKGDIR)/BUILD $(PKGDIR)/RPMS
	find $(INSTALLDIR) -type f | sed 's|^'$(INSTALLDIR)'||' > $(PKGDIR)/BUILD/RPM.files
	echo '%_topdir '$(PKGDIR) > $(HOME)/.rpmmacros
	sed spec.in -e 's/@NAME@/$(PKGNAME)/' \
	    -e 's/@VERSION@/$(PKGVERSION)/' \
	    -e 's/@RELEASE@/$(PKGRELEASE)/' \
	    -e 's/@ARCH@/$(DEBARCH)/' \
	    > $(PKGDIR)/spec
	rpmbuild -bb $(PKGDIR)/spec --buildroot $(INSTALLDIR) --target `arch`
	find $(PKGDIR)/RPMS -name '*.rpm' -type f -exec $(MV) '{}' $(PKGDIR) ';'

deb: install
	$(MKDIR) $(INSTALLDIR)/DEBIAN
	for file in preinst postinst prerm postrm; do \
		[ -f $$file ] && install -m 755 $$file $(INSTALLDIR)/DEBIAN/; done; \
	grep -v '^#' control.in | \
	sed -e 's/@NAME@/$(PKGNAME)/' \
	    -e 's/@VERSION@/$(PKGVERSION)-$(PKGRELEASE)/' \
	    -e 's/@ARCH@/$(DEBARCH)/' \
	    > $(INSTALLDIR)/DEBIAN/control
	fakeroot dpkg-deb --build $(INSTALLDIR) $(PKGDIR)/$(PKGNAME)_$(PKGVERSION)-$(PKGRELEASE)_$(DEBARCH).deb
