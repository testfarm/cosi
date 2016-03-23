##
## Build rules for COSI server based on Tesseract library
##

PKGNAME := tesseract-srv

VERSION := $(shell git describe --long --always --dirty 2>/dev/null || cat VERSION 2>/dev/null)
SHORT_VERSION := $(shell echo $(VERSION) | sed -e 's/-dirty$$//' -e 's/-[a-zA-Z0-9]\+$$//')
PKGVERSION := $(shell echo $(SHORT_VERSION:v%=%) | cut -d- -f1)
PKGRELEASE := $(shell echo $(SHORT_VERSION:v%=%) | cut -d- -f2)

PKGDIR := $(shell pwd)/out
INSTALLDIR = $(PKGDIR)/root
BINDIR = $(INSTALLDIR)/usr/bin

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
