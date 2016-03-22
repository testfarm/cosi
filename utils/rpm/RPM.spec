# $Id: RPM.spec 33 2010-01-12 10:24:03Z giroudon $
#
# COSI - The Common OCR Service Interface
# RPM build spec for creating a binary package
# of the COSI utils.
# 
# Copyright (C) 2010 Sylvain Giroudon
# 


Name:     cosi-utils
Version:  0.2
Release:  1
Summary:  Utilities for testing COSI-compliant OCR agents
Packager: Sylvain Giroudon <giroudon {at} users.sourceforge.net>

Group:          Graphics
License:        GPL
URL:            http://cosi.sourceforge.net

%description
The Common OCR Service Interface.COSI is an API that allows developpers
and integrators to easily bring OCR (Optical Character Recognition)
capabilities to image processing applications.
COSI makes it easy to interface to existing OCR tools such as Tesseract or GOCR.
The COSI utilities allows to test OCR agents by sending them COSI requests to
decode some regions of a PNG image file.

%files -f RPM.files
%defattr(-,root,root,-)
