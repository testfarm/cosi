# $Id: RPM.spec 42 2010-06-03 14:43:33Z giroudon $
#
# RPM build spec for creating a binary package
# of the COSI-compliant flavour of Tesseract-OCR
# 
# Copyright (C) 2010 Sylvain Giroudon
# 


Name:     tesseract-srv
Version:  2.04
Release:  2
Summary:  The Tesseract Open source OCR Engine, with COSI server support
Packager: Sylvain Giroudon <giroudon {at} users.sourceforge.net>

Group:          Graphics
License:        Apache License
URL:            http://cosi.sourceforge.net

%description
The Tesseract OCR engine was one of the top 3 engines in the 1995 UNLV
Accuracy test. Since then it has had little work done on it, but it is
probably one of the most accurate open source OCR engines available. The
source code will read a binary, grey or color image and output text. A tiff
reader is built in that will read uncompressed TIFF images, or libtiff can
be added to read compressed images.

%files -f RPM.files
%defattr(-,root,root,-)
