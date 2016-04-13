##
## COSI - The Common OCR Service Interface
## Top-level build rules
##
## Copyright (C) 2007-2016 Sylvain Giroudon
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

SUBDIRS = gocr tesseract ocrad utils

all:
	@for dir in $(SUBDIRS); do \
	  make -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
	  make -C $$dir clean; \
	done
distclean:
	@for dir in $(SUBDIRS); do \
	  make -C $$dir distclean; \
	done

rpm:
	@for dir in $(SUBDIRS); do \
	  make -C $$dir rpm; \
	done

deb:
	@for dir in $(SUBDIRS); do \
	  make -C $$dir deb; \
	done
