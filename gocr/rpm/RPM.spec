# $Id: RPM.spec 42 2010-06-03 14:43:33Z giroudon $
#
# RPM build spec for creating a binary package
# of the COSI-compliant flavour of GOCR
# 
# Copyright (C) 2010 Sylvain Giroudon
# 


Name:     gocr-srv
Version:  0.48
Release:  2
Summary:  The GOCR Open source OCR Engine, with COSI server support
Packager: Sylvain Giroudon <giroudon {at} users.sourceforge.net>

Group:          Graphics
License:        GPL
URL:            http://cosi.sourceforge.net

%description
GOCR is an optical character recognition program. 
It reads images in many formats  and outputs a text file.
Possible image formats are pnm, pbm, pgm, ppm, some pcx and
tga image files. Other formats like pnm.gz, pnm.bz2, png, jpg, tiff, gif,
bmp will be automatically converted using the netpbm-progs, gzip and bzip2
via unix pipe.
Gocr is also able to recognize and translate barcodes.
You do not have to train the program or store large font bases.
Simply call gocr from the command line and get your results.

%files -f RPM.files
%defattr(-,root,root,-)
