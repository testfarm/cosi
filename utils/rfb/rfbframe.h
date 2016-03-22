/**********************************************************************
 * $Id: rfbframe.h 50 2012-04-18 20:51:33Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * VNC (RFB) client: frame buffer and events processing engine 
 * 
 * Copyright (C) 2007-2012 Sylvain Giroudon
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

#ifndef __COSI_RFBFRAME_H__
#define __COSI_RFBFRAME_H__

#include <glib.h>

#include "cosi.h"
#include "rfbproto.h"
#include "rfblib.h"


/*
 * Frame geometry
 */
typedef struct {
  int x, y;                   /* Window position */
  unsigned int width, height; /* Window size */
} frame_geometry_t;


typedef struct {
  int shmid;
  cosi_buf_t *fb;              /* Pointer to RGB Frame Buffer */
  frame_geometry_t window;     /* Active screen window */

  rfb_t *rfb;
  GIOChannel *rfb_channel;
  guint rfb_tag;
  guint rfb_timeout;

  unsigned char *rfb_buf;              /* Incoming frame buffer */
  frame_geometry_t rfb_g;              /* Recently refreshed area */
  int refresh_requested;               /* Refresh requested but nothing new from RFB server */

  unsigned long delay;                 /* Frame refresh delay in milliseconds */
  unsigned int state;                  /* State of frame buffer update */
  guint watchdog_tag;                  /* Read watchdog timeout tag */
  unsigned int nrects;
  rfbFramebufferUpdateRectHeader rect;
  unsigned char *rowbuf;
  unsigned long rowsize;
  unsigned long xi, yi;

  unsigned long cutlen;                /* Number of cut text characters */
  unsigned long cutidx;                /* Cut text receive index */
  unsigned char *cutbuf;
  unsigned long cutstate;              /* Cut text previous state backup */

  void *read_buf;
  int read_size;
  int read_ofs;
} rfb_capture_t;


/*
 * Low-level interface
 */
extern int rfb_capture_read(rfb_capture_t *capture);


/*
 * RFB client functions
 */
extern rfb_capture_t *rfb_capture_open(char *peer, int shared, int debug);
extern void rfb_capture_close(rfb_capture_t *capture);

extern int rfb_capture_set_window(rfb_capture_t *capture, frame_geometry_t *g);
extern int rfb_capture_get_window(rfb_capture_t *capture, frame_geometry_t *g);

extern long rfb_capture_set_period(rfb_capture_t *capture, long delay);
extern int rfb_capture_refresh(rfb_capture_t *capture);

extern void rfb_capture_action_key(rfb_capture_t *capture, int down, unsigned long key);
extern void rfb_capture_action_pointer(rfb_capture_t *capture, unsigned char buttons, unsigned int x, unsigned int y);
extern void rfb_capture_action_scroll(rfb_capture_t *capture, unsigned char direction);
extern void rfb_capture_show_status(rfb_capture_t *capture, FILE *f, char *hdr);


/*
 * RFB frame buffer update event callback
 */
extern void rfb_capture_event_update(rfb_capture_t *capture, frame_geometry_t *g);
extern void rfb_capture_event_bell(rfb_capture_t *capture);
extern void rfb_capture_event_cuttext(rfb_capture_t *capture, unsigned char *cutbuf);


#endif /* __COSI_RFBFRAME_H__ */
