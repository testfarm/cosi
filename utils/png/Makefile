##
## $Id: Makefile 49 2012-04-17 21:12:38Z giroudon $
##
## COSI - The Common OCR Service Interface
## Frame buffer source : PNG file
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

include ../defs.mk

CFLAGS += -I..
LDFLAGS += -lreadline

CFLAGS  += $(shell pkg-config --cflags libpng)
LDFLAGS += $(shell pkg-config --libs libpng)

BINS = cosi-png

all: $(BINS)

cosi-png: cmd.o pngfile.o ../cosi_buf.o

install: all
	$(MKDIR) $(BINDIR)
	$(CP) $(BINS) $(BINDIR)

include ../rules.mk
