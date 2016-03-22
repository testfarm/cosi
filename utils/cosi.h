/**********************************************************************
 * $Id: cosi.h 50 2012-04-18 20:51:33Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * General COSI definitions
 * 
 * Copyright (C) 2007-2009 Sylvain Giroudon
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details:
 * http://www.gnu.org/licenses/gpl.html
 **********************************************************************/

#ifndef __COSI_H__
#define __COSI_H__

typedef struct {
  unsigned int width, height;          /* RGB frame size */
  unsigned int bpp, rowstride;
  unsigned char buf[1];                /* RGB frame buffer */
} cosi_buf_t;

extern cosi_buf_t *cosi_buf_alloc(unsigned int width, unsigned int height, unsigned char *rgb,
				  int *_shmid);
extern cosi_buf_t *cosi_buf_map(int shmid);
extern void cosi_buf_unmap(cosi_buf_t *fb);

#endif /* __COSI_H__ */
