/**********************************************************************
 * $Id: viewer_sel.c 48 2012-04-17 21:06:56Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * Frame buffer viewer (GTK+) -- Sub-window selection
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

#include <gtk/gtk.h>

#include "cosi.h"
#include "viewer_utils.h"
#include "viewer_sel.h"


/* Viewer widgets and frame buffer */
extern cosi_buf_t *fb;
extern GtkLabel *label_selection;
extern GtkDrawingArea *darea;

extern void refresh(GdkRectangle *rect);

/* Selection window coordinates */
static int sel_x = -1;
static int sel_y = -1;
static GdkRectangle sel_window = {-1,-1,-1,-1};
static GdkColor sel_color;
static GdkGC *sel_gc = NULL;


static void sel_draw(GdkRectangle *rect)
{
  if ( darea == NULL )
    return;
  if ( rect->width <= 1 )
    return;
  if ( rect->height <= 1 )
    return;

  dprintf("SEL DRAW x=%u y=%u w=%u h=%u\n", rect->x, rect->y, rect->width, rect->height);

  /* Clip selection into RGB area */
  if ( rect->x < 0 )
    rect->x = 0;
  if ( rect->y < 0 )
    rect->y = 0;

  gdk_draw_rectangle(GTK_WIDGET(darea)->window,
                     sel_gc,
                     FALSE, /* Not filled */
                     rect->x, rect->y,
		     rect->width-1, rect->height-1);
}


static void sel_undraw(GdkRectangle *rect)
{
  GdkRectangle g;

  if ( darea == NULL )
    return;
  if ( rect->width <= 1 )
    return;
  if ( rect->height <= 1 )
    return;

  dprintf("SEL UNDRAW x=%u y=%u w=%u h=%u\n", rect->x, rect->y, rect->width, rect->height);

  /* Horizontal, top */
  g.x = rect->x;
  g.y = rect->y;
  g.width = rect->width - 1;
  g.height = 1;
  refresh(&g);

  /* Vertical, left */
  g.width = 1;
  g.height = rect->height;
  refresh(&g); 

  /* Vertical, right */
  g.x = rect->x + rect->width - 1;
  refresh(&g);

  /* Horizontal, bottom */
  g.x = rect->x;
  g.y = rect->y + rect->height - 1;
  g.width = rect->width;
  g.height = 1;
  refresh(&g);
}


static void sel_hide(void)
{
  if ( sel_window.width > 0 ) {
    lprintf(label_selection, "");
    sel_undraw(&sel_window);
  }
}


static void sel_show(void)
{
  if ( sel_window.width > 0 ) {
    lprintf(label_selection, "%dx%d+%d+%d",
	    sel_window.width, sel_window.height, sel_window.x, sel_window.y);
    sel_draw(&sel_window);
  }
}


static void sel_update(int x, int y)
{
  sel_hide();

  if ( fb == NULL )
    return;

  /* Clip selection to RGB area */
  if ( x < 0 )
    x = 0;
  if ( x >= fb->width )
    x = fb->width - 1;
  if ( y < 0 )
    y = 0;
  if ( y >= fb->height )
    y = fb->height - 1;

  if ( sel_x <= x ) {
    sel_window.x = sel_x;
    sel_window.width = x - sel_x + 1;
  }
  else {
    sel_window.x = x;
    sel_window.width = sel_x - x + 1;
  }

  if ( sel_y <= y ) {
    sel_window.y = sel_y;
    sel_window.height = y - sel_y + 1;
  }
  else {
    sel_window.y = y;
    sel_window.height = sel_y - y + 1;
  }

  if ( (sel_window.width < 2) && (sel_window.height < 2) ) {
    sel_window.width = 0;
    sel_window.height = 0;
  }

  sel_show();
}


static gboolean sel_motion(GtkWidget *widget, GdkEventMotion *event)
{
  if ( (sel_x >= 0) && (sel_y >= 0) ) {
    sel_update(event->x, event->y); 
  }
  return FALSE;
}


static gboolean sel_button_press(GtkWidget *widget, GdkEventButton *event)
{
  if ( event->type == GDK_BUTTON_PRESS ) {
    dprintf("BUTTON PRESS: button=%d x=%d y=%d\n", event->button, (int) event->x, (int) event->y);
    if ( event->button == 1 ) {
      sel_hide();

      sel_window.width = 0;
      sel_window.height = 0;

      if (fb != NULL) {
	      sel_x = event->x;
	      if ( sel_x >= fb->width )
		      sel_x = fb->width - 1;

	      sel_y = event->y;
	      if ( sel_y >= fb->height )
		      sel_y = fb->height - 1;
      }
      else {
	      sel_x = -1;
	      sel_y = -1;
      }
    }
  }

  return FALSE;
}


static gboolean sel_button_release(GtkWidget *widget, GdkEventButton *event)
{
  if ( event->type == GDK_BUTTON_RELEASE ) {
    dprintf("BUTTON RELEASE: button=%d x=%d y=%d\n", event->button, (int) event->x, (int) event->y);
    if ( event->button == 1 ) {
      sel_update(event->x, event->y); 

      sel_x = -1;
      sel_y = -1;
    }
  }

  return FALSE;
}


void sel_get(GdkRectangle *rect)
{
  *rect = sel_window;
}


void sel_init(GtkDrawingArea *darea)
{
  /* Create color for selection frame */
  sel_color.red = 65535;
  sel_color.green = 0;
  sel_color.blue = 0;

  if ( ! gdk_colormap_alloc_color(gdk_colormap_get_system(), &sel_color, FALSE, TRUE) )
    eprintf("Cannot allocate color\n");

  /* Create graphic context for selection frame */
  sel_gc = gdk_gc_new(GTK_WIDGET(darea)->window);
  gdk_gc_set_foreground(sel_gc, &sel_color);
  gdk_gc_set_line_attributes(sel_gc, 1, GDK_LINE_ON_OFF_DASH, GDK_CAP_NOT_LAST, GDK_JOIN_MITER);
  gdk_gc_set_clip_origin(sel_gc, 0, 0);

  /* Hook selection event handlers */
  gtk_signal_connect(GTK_OBJECT(darea), "motion_notify_event",
                     GTK_SIGNAL_FUNC(sel_motion), NULL);
  gtk_signal_connect(GTK_OBJECT(darea), "button_press_event",
                     GTK_SIGNAL_FUNC(sel_button_press), NULL);
  gtk_signal_connect(GTK_OBJECT(darea), "button_release_event",
                     GTK_SIGNAL_FUNC(sel_button_release), NULL);
}
