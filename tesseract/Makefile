##
## COSI - The Common OCR Service Interface
## Build rules for COSI server based on Tesseract library
##
## Copyright (C) 2007-2016 Sylvain Giroudon
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##
## The Tesseract-OCR tool and library are licensed under the Apache License, Version 2.0.
## See https://github.com/tesseract-ocr/tesseract for further details.
##

PKGNAME := tesseract-srv

VERSION := $(shell git describe --long --always --dirty 2>/dev/null || cat VERSION 2>/dev/null)
SHORT_VERSION := $(shell echo $(VERSION) | sed -e 's/-dirty$$//' -e 's/-[a-zA-Z0-9]\+$$//')
PKGVERSION := $(shell echo $(SHORT_VERSION:v%=%) | cut -d- -f1)
PKGRELEASE := $(shell echo $(SHORT_VERSION:v%=%) | cut -d- -f2)

PKGDIR := $(shell pwd)/out
DESTDIR = $(PKGDIR)/root
BINDIR = $(DESTDIR)/usr/bin

CC = g++
RM = /bin/rm -rf
MKDIR = /bin/mkdir -p
CP = /bin/cp -fv

CFLAGS = -Wall
LDFLAGS = -ltesseract

BINS = tesseract-srv

all: $(BINS)

tesseract-srv: tesseract-srv.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean::
	$(RM) *~ *.o $(BINS)

install: all
	$(MKDIR) $(BINDIR)
	$(CP) $(BINS) $(BINDIR)

include ../pkg.mk
