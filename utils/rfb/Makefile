##
## $Id: Makefile 50 2012-04-18 20:51:33Z giroudon $
##
## COSI - The Common OCR Service Interface
## Frame buffer source: VNC client (RFB)
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
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details:
## http://www.gnu.org/licenses/gpl.html
##

include ../defs.mk

GLIB = glib-2.0

CFLAGS += -I..

CFLAGS  += $(shell pkg-config --cflags $(GLIB))
LDFLAGS += $(shell pkg-config --libs $(GLIB))

BINS = cosi-rfb

all: $(BINS)

cosi-rfb: main.o d3des.o rfblib.o rfbframe.o ../cosi_buf.o

install: all
	$(MKDIR) $(BINDIR)
	$(CP) $(BINS) $(BINDIR)

include ../rules.mk
