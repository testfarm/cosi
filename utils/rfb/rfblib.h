/**********************************************************************
 * $Id: rfblib.h 50 2012-04-18 20:51:33Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * VNC (RFB) client: RFB communication primitives 
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

#ifndef __COSI_RFBLIB_H__
#define __COSI_RFBLIB_H__

#include <netinet/in.h>

#define RFB_PORT 5900


typedef enum {
	RFB_SCROLL_UP=1,
	RFB_SCROLL_DOWN,
	RFB_SCROLL_LEFT,
	RFB_SCROLL_RIGHT,
} rfb_scroll_dir_t;


typedef struct {
  char *host;
  int port;
  struct sockaddr_in iremote;
  int sock;
  int major, minor;

  unsigned short width, height;
  char *name;

  unsigned int bits_per_pixel;
  unsigned int depth;
  int big_endian;
  int true_color;
  unsigned short red_max, green_max, blue_max;
  unsigned char red_shift, green_shift, blue_shift;

  unsigned int pointer_x;
  unsigned int pointer_y;
  unsigned char pointer_buttons;
} rfb_t;


extern int rfb_Debug(int state);
extern rfb_t *rfb_New(char *host, int port, char *passwd, int shared);
extern void rfb_Destroy(rfb_t *rfb);
extern int rfb_Report(rfb_t *rfb, FILE *f, char *hdr);
extern void rfb_Shutdown(rfb_t *rfb);

extern int rfb_KeyEvent(rfb_t *rfb, int down, unsigned long key);

#define RFB_BUTTON_1 0x01
#define RFB_BUTTON_2 0x02
#define RFB_BUTTON_3 0x04
#define RFB_BUTTON_UP    0x08
#define RFB_BUTTON_DOWN  0x10
#define RFB_BUTTON_LEFT  0x20
#define RFB_BUTTON_RIGHT 0x40

extern int rfb_PointerEvent(rfb_t *rfb, unsigned char buttons, unsigned int x, unsigned int y);
extern int rfb_PointerPosition(rfb_t *rfb, unsigned int x, unsigned int y);
extern int rfb_PointerButtons(rfb_t *rfb, unsigned char buttons);
extern int rfb_PointerScroll(rfb_t *rfb, rfb_scroll_dir_t direction);

extern int rfb_SetEncodings(rfb_t *rfb, int tabc, const unsigned long tabv[]);

extern int rfb_SetPixelFormat(rfb_t *rfb);
extern int rfb_SetRGBPixelFormat(rfb_t *rfb, int bits_per_color);

extern int rfb_UpdateRequest(rfb_t *rfb, unsigned char incremental,
                             unsigned int x, unsigned int y, unsigned int w, unsigned int h);

#endif /* __COSI_RFBLIB_H__ */
