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
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.  
##

DEBARCH = $(shell dpkg-architecture -qDEB_HOST_ARCH)

rpm: install
	$(MKDIR) $(PKGDIR)/BUILD $(PKGDIR)/RPMS
	find $(INSTALLDIR) -type f | sed 's|^'$(INSTALLDIR)'||' > $(PKGDIR)/BUILD/RPM.files
	echo '%_topdir '$(PKGDIR) > $(HOME)/.rpmmacros
	sed spec.in -e 's/@NAME@/$(PKGNAME)/' \
	    -e 's/@VERSION@/$(PKGVERSION)/' \
	    -e 's/@RELEASE@/$(PKGRELEASE)/' \
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
