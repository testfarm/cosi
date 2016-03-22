/**********************************************************************
 * $Id: pngfile.h 48 2012-04-17 21:06:56Z giroudon $
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

#ifndef __COSI_PNGFILE_H__
#define __COSI_PNGFILE_H__

#include "cosi.h"

extern int png_load(char *fname, unsigned int *pwidth, unsigned int *pheight,
		    unsigned char **pbuf);

extern int png_save(cosi_buf_t *fb,
		    int x, int y, unsigned int width, unsigned int height,
		    char *fname);

#endif /* __COSI_PNGFILE_H__ */
