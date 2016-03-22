#!/bin/sh -f
#
# Install packages needed to build COSI OCR agents and utilities
#

PACKAGES_rpm="gcc gcc-c++ lzip help2man netpbm-devel libtiff-devel libglade2-devel readline-devel"
PACKAGES_deb="build-essential lzip help2man libnetpbm10-dev libtiff5-dev libglade2-dev libpng12-dev libreadline-dev"

if [ -f /etc/os-release ]; then
    echo "DISTRO = Debian/Ubuntu"
    sudo apt-get install $PACKAGES_deb
elif [ -f /etc/system-release ]; then
    echo "DISTRO = RedHat/Fedora"
    sudo yum install $PACKAGES_rpm
fi
