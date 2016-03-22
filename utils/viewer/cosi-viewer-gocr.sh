#!/bin/sh -f

# $Id: cosi-viewer-gocr.sh 48 2012-04-17 21:06:56Z giroudon $
#
# COSI - The Common OCR Service Interface
# Frame buffer viewer wrapper for GOCR

shmid=$1
if [ -z "$shmid" ]; then
  echo "Usage: $0 <shmid>" 1>&2
  exit 1
fi

./cosi-viewer $shmid | gocr -f XML -F 10 -S $shmid
