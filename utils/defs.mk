##
## $Id: defs.mk 50 2012-04-18 20:51:33Z giroudon $
##
## COSI - The Common OCR Service Interface
## Top-level Makefile definitions
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

CC = gcc
LD = gcc

CFLAGS = -Wall -O2
LDFLAGS =

ifdef INSTALLDIR
BINDIR = $(INSTALLDIR)/usr/bin
else
BINDIR = /usr/local/bin
endif

RM = /bin/rm -rf
MKDIR = /bin/mkdir -p
MV = /bin/mv -fv
CP = /bin/cp -fv
