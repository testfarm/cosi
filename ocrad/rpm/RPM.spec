# $Id$
#
# RPM build spec for creating a binary package
# of the COSI-compliant flavour of GNU Ocrad
# 
# Copyright (C) 2010 Sylvain Giroudon
# 


Name:     ocrad-srv
Version:  0.19
Release:  1
Summary:  The GNU Ocrad Open source OCR Engine, with COSI server support
Packager: Sylvain Giroudon <giroudon {at} users.sourceforge.net>

Group:          Graphics
License:        GPL
URL:            http://cosi.sourceforge.net

%description
GNU Ocrad is an OCR (Optical Character Recognition) program based on a feature
extraction method. It reads images in pbm (bitmap), pgm (greyscale) or ppm
(color) formats and produces text in byte (8-bit) or UTF-8 formats.
Also includes a layout analyser able to separate the columns or blocks of text
normally found on printed pages.
Ocrad can be used as a stand-alone console application, or as a backend to
other programs.
This release also supports the Common OCR Service Interface (COSI),
with shared-memory frame-buffer input and XML output capability.

%files -f RPM.files
%defattr(-,root,root,-)
