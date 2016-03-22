/**********************************************************************
 * $Id: cosi_buf.c 50 2012-04-18 20:51:33Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * POSIX shared memory Frame Buffer 
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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>

#include "utils.h"
#include "cosi.h"


/* We only support 3 bpp frame buffers */
#define COSI_BUF_BPP 3


cosi_buf_t *cosi_buf_alloc(unsigned int width, unsigned int height, unsigned char *rgb,
			   int *_shmid)
{
  unsigned int rowstride;
  unsigned int rgbsize;
  unsigned int shmsize;
  int shmid;
  cosi_buf_t *fb;

  /* Compute COSI frame buffer dimensions */
  rowstride = width * COSI_BUF_BPP;
  rgbsize =  height * rowstride;
  shmsize = sizeof(cosi_buf_t) + rgbsize;

  /* Alloc COSI frame buffer */
  if ( (shmid = shmget(IPC_PRIVATE, shmsize, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W)) == -1 ) {
    eprintf("shmget: %s\n", strerror(errno));
    return NULL;
  }
  if ( (fb = shmat(shmid, NULL, 0)) == (void *) -1 ) {
    eprintf("shmat(%d): %s\n", shmid, strerror(errno));
    return NULL;
  }

  /* Mark shared memory buffer for automatic deletion
     when all processes have detached */
  if ( shmctl(shmid, IPC_RMID, NULL) == -1 ) {
    eprintf("shmctl(%d): %s\n", shmid, strerror(errno));
    return NULL;
  }

  /* Setup frame control buffer */
  if ( fb == NULL ) {
    eprintf("Frame Control buffer allocation failed (null pointer returned)\n");
    return NULL;
  }

  /* Fill COSI frame buffer */
  fb->width = width;
  fb->height = height;
  fb->bpp = COSI_BUF_BPP;
  fb->rowstride = rowstride;
  if (rgb != NULL) {
	  memcpy(fb->buf, rgb, rgbsize);
  }
  else {
	  memset(fb->buf, 0, rgbsize);
  }

  if ( _shmid != NULL ) {
    *_shmid = shmid;
  }

  return fb;
}


cosi_buf_t *cosi_buf_map(int shmid)
{
  void *ptr;

  if ( (ptr = shmat(shmid, NULL, SHM_RDONLY)) == (void *) -1 ) {
    eprintf("shmat(%d): %s\n", shmid, strerror(errno));
    return NULL;
  }

  return (cosi_buf_t *) ptr;
}


void cosi_buf_unmap(cosi_buf_t *fb)
{
	if ( fb != NULL ) {
		if (shmdt(fb) == -1) {
			eprintf("shmdt: %s\n", strerror(errno));
		}
	}
}
