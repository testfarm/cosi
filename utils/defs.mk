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

CC = gcc
LD = gcc

CFLAGS = -Wall -O2
LDFLAGS =

ifdef DESTDIR
BINDIR = $(DESTDIR)/usr/bin
else
BINDIR = /usr/local/bin
endif

RM = /bin/rm -rf
MKDIR = /bin/mkdir -p
MV = /bin/mv -fv
CP = /bin/cp -fv
