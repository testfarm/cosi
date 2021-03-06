# COSI - The Common OCR Service Interface.
COSI is an API that allows developpers to easily bring OCR (Optical Character Recognition)
capabilities to image processing applications.
COSI supports existing OCR tools such as Tesseract, GOCR or GNU Ocrad.


## Description
COSI is an open standard targeted at using different OCR (Optical Character Recognition) programs as external agents in a unified manner. COSI allows to integrate an OCR tool into various top-level applications using a client-server approach.

COSI has been successfully implemented and experimented on the following well-known open source OCR applications:
- Tesseract-OCR: using the natively installed Tesseract library (typ. 3.03 in Ubuntu 14.04LTS).
- GNU Ocrad: version 0.25
- GOCR: version 0.50

## Resources
- The detailed technical description of COSI: http://www.testfarm.org/documents/IP090002-en%20COSI%20-%20The%20Common%20OCR%20Service%20Interface.4.pdf
- The COSI GitHub page: https://github.com/testfarm/cosi
- The old COSI SourceForge project page: https://sourceforge.net/p/cosi
- Tesseract-OCR GitHub page: https://github.com/tesseract-ocr
- GNU Ocrad: https://www.gnu.org/software/ocrad/
- GOCR SourceForge project page: http://jocr.sourceforge.net/

## Bringing COSI support to existing OCR agents
As of today, COSI support is available on Linux platforms only.
Support for other operating systems will be available later.

You can build COSI-compliant versions of Tesseract-OCR and GOCR
directly from the COSI source tree. It provides some Makefile's
that automatically download the source code of OCR agents,
apply COSI patches, compile everything, and optionnaly generate
the corresponding .rpm and .deb packages.

### Downloading and Compiling
```
$ git clone https://github.com/testfarm/cosi.git
$ cd cosi
$ ./install-tools.sh
$ make
```

### Generating the distro packages
```
$ make rpm
```
or
```
$ make deb
```

Then you can find the following packages, ready to be installed :
- Tesseract-OCR: tesseract/out/tesseract-srv-*.{rpm,deb}
- GOCR: gocr/out/gocr-srv-*.{rpm,deb}
- GNU Ocrad: ocrad/out/ocrad-srv-*.{rpm,deb}
- COSI-utils: utils/out/cosi-utils-*.{rpm,deb}

## The COSI utils
### Overview
The COSI utils is a collection of simple utilities to test a
COSI-compliant OCR agent under Linux (or any other POSIX OS).
There are two binaries allowing to easily test an OCR agent:
- cosi-png creates a shared memory frame buffer from a PNG image file.
- cosi-viewer allows to easily generate COSI requests, that can be piped to the OCR agent standard input. This utility requires the GTK+ graphical environment.

### Testing the OCR agents
From the COSI-utils directory, export a test image file to a shared
memory frame buffer, and note the shmid resulting from this operation.
In the example below, we assume <shmid> is 3833877.
```
$ cosi-png eurotext.png
COSI RGB buffer: 1024x800, shmid=3833877
```

While keeping the cosi-cmd utility running, launch the cosi-viewer utility and pipe it to the OCR agent.

#### Using Tesseract-OCR
```
$ cosi-viewer <shmid> | tesseract-srv <shmid>
```

#### Using GOCR
```
$ cosi-viewer <shmid> | gocr-srv <shmid>
```

#### Using GNU Ocrad
```
$ cosi-viewer <shmid> | ocrad-srv <shmid>
```
