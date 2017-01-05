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
PKGVERSION_PREFIX = $(shell ./tesseract-version)-

OUTDIR := $(shell pwd)/out
PKGDIR := $(OUTDIR)
DESTDIR = $(OUTDIR)/root
BINDIR = $(DESTDIR)/usr/bin

CC = g++
RM = /bin/rm -rf
MKDIR = /bin/mkdir -p
CP = /bin/cp -fv

CFLAGS = -Wall
LDFLAGS = -ltesseract

BINS = tesseract-srv
TOOLS = tesseract-version

all: $(BINS) $(TOOLS)

tesseract-srv: tesseract-srv.o
tesseract-version: tesseract-version.o

$(BINS) $(TOOLS):
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean::
	$(RM) *~ *.o $(BINS)

install: all
	$(MKDIR) $(BINDIR)
	$(CP) $(BINS) $(BINDIR)

include ../pkg.mk
