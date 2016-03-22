##
## COSI - The Common OCR Service Interface
## Top-level Makefile
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

VERSION := 0.2
PKGNAME := cosi-utils
PKGVERSION := $(VERSION)
PKGRELEASE := 1

export PKGDIR := $(shell pwd)/out
export INSTALLDIR = $(PKGDIR)/root

include defs.mk

OBJS = cosi_buf.o

SUBDIRS = png rfb viewer

all: $(OBJS)
	for dir in $(SUBDIRS); do \
	  make -C $$dir; \
	done

install: all
	for dir in $(SUBDIRS); do \
	  make -C $$dir install; \
	done

clean::
	for dir in $(SUBDIRS); do \
	  make -C $$dir clean; \
	done

include rules.mk
include ../pkg.mk
