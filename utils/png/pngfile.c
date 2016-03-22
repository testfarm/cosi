/**********************************************************************
 * $Id: pngfile.c 48 2012-04-17 21:06:56Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * PNG image file management
 * 
 * Copyright (C) 2007-2009 Sylvain Giroudon
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.  
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <png.h>

#include "utils.h"
#include "pngfile.h"


int png_load(char *fname, unsigned int *pwidth, unsigned int *pheight,
	     unsigned char **pbuf)
{
  FILE *f;
  unsigned char header[8];
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  unsigned long width, height;
  int bit_depth, color_type, interlace_type;
  int ret = 0;

  /* Open input file */
  if ((f = fopen(fname, "rb")) == NULL) {
    eprintf("%s: Cannot open PNG file: %s\n", fname, strerror(errno));
    return -1;
  }

  /* Check file signature */
  ret = fread(header, 1, sizeof(header), f);
  if ( ret < 0 ) {
    eprintf("%s: Cannot read PNG file: %s\n", fname, strerror(errno));
    goto bailout;
  }

  if ( ret != sizeof(header) ) {
    eprintf("%s: Illegal PNG header\n", fname);
    ret = -1;
    goto bailout;
  }

  if ( png_sig_cmp(header, 0, sizeof(header)) ) {
    eprintf("%s: Not a PNG file\n", fname);
    ret = -1;
    goto bailout;
  }

  ret = 0;

  /* Setup PNG read gears */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if ( png_ptr == NULL ) {
    ret = -1;
    goto bailout;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if ( info_ptr == NULL ) {
    ret = -1;
    goto bailout;
  }

  if ( setjmp(png_jmpbuf(png_ptr)) ) {
    ret = -1;
    goto bailout;
  }

  /* Init file operations */
  png_init_io(png_ptr, f);
  png_set_sig_bytes(png_ptr, sizeof(header));

  /* Get image info */
  png_read_info(png_ptr, info_ptr);
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
	       &interlace_type, int_p_NULL, int_p_NULL);
  //fprintf(stderr, "-- %s: %lux%lu depth=%d color=%d\n", fname, width, height, bit_depth, color_type);

  if ( pwidth != NULL )
    *pwidth = width;
  if ( pheight != NULL )
    *pheight = height;

  /* We only support RGB images */
  if ( ! (color_type & PNG_COLOR_MASK_COLOR) ) {
    eprintf("%s: Image pixels are not in RGB/RGBA format\n", fname);
    ret = -1;
    goto bailout;
  }

  if ( (pbuf != NULL) && (width > 0) && (height > 0) ) {
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    int bytesperpixel = rowbytes / width;
    png_bytep row_pointers[height];
    unsigned char *buf, *ptr;
    int row;

    //fprintf(stderr, "   rowbytes=%d => bytesperpixel=%d\n", rowbytes, bytesperpixel);

    /* Alloc image buffer */
    ptr = buf = malloc(rowbytes * height);
    for (row = 0; row < height; row++) {
      row_pointers[row] = ptr;
      ptr += rowbytes;
    }

    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, info_ptr);

    if ( color_type & PNG_COLOR_MASK_ALPHA ) {
      int rowbytes2 = 3 * width;
      unsigned char *ptr2;

      //fprintf(stderr, "   rowbytes2=%d\n", rowbytes2);

      ptr2 = *pbuf = malloc(rowbytes2 * height);
      ptr = buf;

      for (row = 0; row < height; row++) {
	unsigned char *xptr = ptr;
	unsigned char *xptr2 = ptr2;
	int x;

	for (x = 0; x < width; x++) {
	  xptr2[0] = xptr[0];
	  xptr2[1] = xptr[1];
	  xptr2[2] = xptr[2];

	  xptr += bytesperpixel;
	  xptr2 += 3;
	}

	ptr += rowbytes;
	ptr2 += rowbytes2;
      }

      free(buf);
    }
    else {
      *pbuf = buf;
    }
  }

  bailout:
  png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
  fclose(f);

  return ret;
}


int png_save(cosi_buf_t *fb,
	     int x, int y, unsigned int width, unsigned int height,
	     char *fname)
{
  FILE *f;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  int ret = 0;
  unsigned char *ptr;
  int i;

  if ( (f = fopen(fname, "wb")) == NULL ) {
    eprintf("%s: Failed to create PNG file: %s\n", fname, strerror(errno));
    return -1;
  }
  
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if ( png_ptr == NULL ) {
    ret = -1;
    goto bailout;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if ( info_ptr == NULL ) {
    ret = -1;
    goto bailout;
  }

  if ( setjmp(png_jmpbuf(png_ptr)) ) {
    ret = -1;
    goto bailout;
  }

  png_init_io(png_ptr, f);

  png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
	       PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  {
   png_bytep row_pointers[height];

    ptr = fb->buf + (fb->rowstride * y) + (fb->bpp * x);
    for (i = 0; i < height; i++) {
      row_pointers[i] = ptr;
      ptr += fb->rowstride;
    }

    png_write_image(png_ptr, row_pointers);
  }

  png_write_end(png_ptr, info_ptr);

 bailout:
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(f);

  return ret;
}
