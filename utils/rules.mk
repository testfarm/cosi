##
## $Id: rules.mk 50 2012-04-18 20:51:33Z giroudon $
##
## COSI - The Common OCR Service Interface
## Top-level Makefile rules
## 
## Copyright (C) 2007-2012 Sylvain Giroudon
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

$(BINS):
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean::
	$(RM) *~ *.o $(BINS)

distclean: clean

rpm: install
	$(MKDIR) $(RPMDIR)/BUILD $(RPMDIR)/RPMS
	find $(INSTALLDIR) -type f | sed 's|^'$(INSTALLDIR)'||' > $(RPMDIR)/BUILD/RPM.files
	echo '%_topdir '$(RPMDIR) > $(HOME)/.rpmmacros
	rpmbuild -bb $(spec) --buildroot $(INSTALLDIR) --target `arch`
	find $(RPMDIR)/RPMS -name '*.rpm' -type f -exec $(MV) '{}' $(RPMDIR) ';'
