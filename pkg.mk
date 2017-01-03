##
## COSI - The Common OCR Service Interface
## Makefile rules for creating RPM/DEB packages
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

DEBARCH = $(shell dpkg-architecture -qDEB_HOST_ARCH)

VERSION ?= $(shell git describe --long --always --dirty 2>/dev/null || cat VERSION 2>/dev/null)
SHORT_VERSION := $(shell echo $(VERSION) | sed -e 's/-dirty$$//' -e 's/-[a-zA-Z0-9]\+$$//')
PKGVERSION ?= $(shell echo $(SHORT_VERSION:v%=%) | cut -d- -f1)
PKGRELEASE ?= $(shell echo $(SHORT_VERSION:v%=%) | cut -d- -f2)

rpm: install
	$(MKDIR) $(PKGDIR)/BUILD $(PKGDIR)/RPMS
	find $(DESTDIR) -type f | sed 's|^'$(DESTDIR)'||' > $(PKGDIR)/BUILD/RPM.files
	echo '%_topdir '$(PKGDIR) > $(HOME)/.rpmmacros
	sed spec.in -e 's/@NAME@/$(PKGNAME)/' \
	    -e 's/@VERSION@/$(PKGVERSION_PREFIX)$(PKGVERSION)/' \
	    -e 's/@RELEASE@/$(PKGRELEASE)/' \
	    > $(PKGDIR)/spec
	rpmbuild -bb $(PKGDIR)/spec --buildroot $(DESTDIR) --target `arch`
	find $(PKGDIR)/RPMS -name '*.rpm' -type f -exec $(MV) '{}' $(PKGDIR) ';'

deb: install
	$(MKDIR) $(DESTDIR)/DEBIAN
	for file in preinst postinst prerm postrm; do \
		[ -f $$file ] && install -m 755 $$file $(DESTDIR)/DEBIAN/; done; \
	grep -v '^#' control.in | \
	sed -e 's/@NAME@/$(PKGNAME)/' \
	    -e 's/@VERSION@/$(PKGVERSION_PREFIX)$(PKGVERSION)-$(PKGRELEASE)/' \
	    -e 's/@ARCH@/$(DEBARCH)/' \
	    > $(DESTDIR)/DEBIAN/control
	fakeroot dpkg-deb --build $(DESTDIR) $(PKGDIR)/$(PKGNAME)_$(PKGVERSION_PREFIX)$(PKGVERSION)-$(PKGRELEASE)_$(DEBARCH).deb
