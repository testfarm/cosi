#!/bin/sh -f
#
# Install RPM packages needed to build COSI OCR agents and utilities
#

PACKAGES="gcc gcc-c++ help2man netpbm-devel libtiff-devel libglade2-devel readline-devel"

exec yum install $PACKAGES
