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

PKGNAME := cosi-utils

OUTDIR := $(shell pwd)/out
PKGDIR := $(OUTDIR)
DESTDIR = $(OUTDIR)/root

include defs.mk

OBJS = cosi_buf.o

SUBDIRS = png rfb viewer

all: $(OBJS)
	for dir in $(SUBDIRS); do \
	  make -C $$dir; \
	done

install: all
	for dir in $(SUBDIRS); do \
	  make -C $$dir DESTDIR=$(DESTDIR) install; \
	done

clean::
	for dir in $(SUBDIRS); do \
	  make -C $$dir DESTDIR=$(DESTDIR) clean; \
	done

include rules.mk
include ../pkg.mk
