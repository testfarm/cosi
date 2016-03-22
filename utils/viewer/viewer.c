/**********************************************************************
 * $Id: viewer.c 48 2012-04-17 21:06:56Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * Frame buffer viewer (GTK+)
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
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <glade/glade-parser.h>

#include "cosi.h"
#include "viewer_utils.h"
#include "viewer_sel.h"


#define ARRAY_SIZE(array) (sizeof(array)/sizeof(*(array)))

#define MIN_WIDTH  10
#define MIN_HEIGHT 10

cosi_buf_t *fb = NULL;

static GladeXML *glade = NULL;
static GtkWidget *window = NULL;
GtkDrawingArea *darea = NULL;
static GtkLabel *label_mouse = NULL;
GtkLabel *label_selection = NULL;
static GtkButton *button_request = NULL;

static int request_id = 0;


void refresh(GdkRectangle *rect)
{
  unsigned int ofs;

  dprintf("REDRAW %ux%u+%d+%d\n", rect->width, rect->height, rect->x, rect->y);

  if ( darea == NULL )
    return;
  if ( fb == NULL )
    return;

  /* Clip refresh area to actual RGB buffer */ 
  if ( (rect->x + rect->width) > fb->width )
    rect->width = fb->width - rect->x;
  if ( (rect->y + rect->height) > fb->height )
    rect->height = fb->height - rect->y;

  /* Compute buffer offset */
  ofs = (fb->rowstride * rect->y) + (fb->bpp * rect->x);

  gdk_draw_rgb_image(GTK_WIDGET(darea)->window, GTK_WIDGET(darea)->style->fg_gc[GTK_STATE_NORMAL],
                     rect->x, rect->y, rect->width, rect->height,
                     GDK_RGB_DITHER_NONE,
		     fb->buf + ofs, fb->rowstride);
}


static gboolean event_expose(GtkWidget *widget, GdkEventExpose *event)
{
  dprintf("EXPOSE x=%u y=%u w=%u h=%u\n", event->area.x, event->area.y, event->area.width, event->area.height);

  refresh(&event->area);

  return TRUE;
}


static void display_mouse(int x, int y, unsigned long time)
{
  if ( darea == NULL )
    return;
  if ( fb == NULL )
    return;

  if ( (x >= 0) && (x < fb->width) && (y >= 0) && (y < fb->height) ) {
    lprintf(label_mouse, "+%d+%d", x, y);
  }
  else {
    lprintf(label_mouse, "");
  }
}


static gboolean event_crossing(GtkWidget *widget, GdkEventCrossing *event)
{
  int x = (int) event->x;
  int y = (int) event->y;

  dprintf("CROSSING: type=%d x=%d y=%d focus=%d\n", event->type, x, y, event->focus);

  if ( event->type == GDK_LEAVE_NOTIFY ) {
    display_mouse(-1, -1, event->time);
  }
  else {
    gtk_widget_grab_focus(GTK_WIDGET(darea));
    display_mouse(x, y, event->time);
  }

  return TRUE;
}


static gboolean event_motion(GtkWidget *widget, GdkEventMotion *event)
{
  int x = (int) event->x;
  int y = (int) event->y;

  dprintf("MOTION: type=%d x=%d y=%d\n", event->type, x, y);

  display_mouse(x, y, event->time);

  return FALSE;
}


static void ocr_request(GtkButton *widget)
{
  GdkRectangle rect;

  request_id++;
  printf("id=\"%d\"", request_id);

  sel_get(&rect);

  if ( (rect.x >= 0) && (rect.y >= 0) &&
       (rect.width >= MIN_WIDTH) && (rect.height > MIN_HEIGHT) ) {
    printf(" geometry=\"%dx%d+%d+%d\"", rect.width, rect.height, rect.x, rect.y);
  }

  printf("\n");
}


static int setup_fb(int shmid)
{
	/* Detach previously mapped frame buffer */
	if (fb != NULL) {
		cosi_buf_unmap(fb);
		fb = NULL;
	}

	/* Attach new frame buffer */
	fb = cosi_buf_map(shmid);
	if ( fb == NULL ) {
		eprintf("Failed to attach frame buffer (shmid=%d)\n", shmid);
		return -1;
	}

	eprintf("Frame buffer %d attached: size=%ux%u\n", shmid, fb->width, fb->height);

	/* Setup drawing area */
	gtk_drawing_area_size(darea, fb->width, fb->height);
	gtk_widget_show(GTK_WIDGET(darea));

	return 0;
}


static void command(gpointer data, int fd, GdkInputCondition condition)
{
	char buf[128];
	char *s;

	/* Read command from stdin */
	if (fgets(buf, sizeof(buf), stdin) == NULL)
		return;

	/* Strip trailing CR/LF */
	s = buf;
	while ((*s != '\0') && (*s != '\r') && (*s != '\n'))
		s++;
	*s = '\0';

	dprintf("COMMAND: '%s'\n", buf);

	/* Split command/arguments */
	s = buf;
	while (*s > ' ')
		s++;
	if (*s != '\0')
		*(s++) = '\0';
	while ((*s != '\0') && (*s <= ' '))
		s++;

	if (strcmp(buf, "SHMID") == 0) {
		setup_fb(atoi(s));
	}
	else if (strcmp(buf, "UPDATE") == 0) {
		GdkRectangle rect;

		if (sscanf(s, "%dx%d+%d+%d", &rect.width, &rect.height, &rect.x, &rect.y) == 4) {
			refresh(&rect);
		}
	}
	else if (*buf != '\0') {
		eprintf("Unknown command: %s\n", buf);
	}
}


static void shutdown(void)
{
  exit(0);
}


int main(int argc, char *argv[])
{
  int shmid = 0;

  /* Enable per-line stdout buffering */
  setlinebuf(stdout);

  /* Init GTK stuffs */
  gtk_set_locale();
  gtk_init(&argc, &argv);

  /* Get command arguments */
  if ((argc > 1) && (strcmp(argv[1], "-h") == 0)) {
    fprintf(stderr, "Usage: " NAME " [<X-options>] [<shmid>]\n");
    return 1;
  }

  if (argc > 1) {
	  shmid = atoi(argv[1]);
  }

  /* Init libglade gears */
  glade_init();

  glade = glade_xml_new("cosi-viewer.glade", NULL, NULL);
  if ( glade == NULL ) {
    eprintf("Failed to initialize user interface\n");
    return 2;
  }

  /* Initialize main window */
  window = glade_xml_get_widget(glade, "window");
  if ( window == NULL ) {
    eprintf("Cannot find main window\n");
    return 2;
  }

  gtk_signal_connect_object(GTK_OBJECT(window), "destroy",
                            GTK_SIGNAL_FUNC(shutdown), NULL);

  /* Initialize drawing area */
  darea = GTK_DRAWING_AREA(glade_xml_get_widget(glade, "darea"));
  if ( darea == NULL ) {
    eprintf("Cannot find drawing area\n");
    return 2;
  }

  gtk_signal_connect(GTK_OBJECT(darea), "expose_event",
                     GTK_SIGNAL_FUNC(event_expose), NULL);
  gtk_signal_connect(GTK_OBJECT(darea), "enter_notify_event",
                     GTK_SIGNAL_FUNC(event_crossing), NULL);
  gtk_signal_connect(GTK_OBJECT(darea), "leave_notify_event",
                     GTK_SIGNAL_FUNC(event_crossing), NULL);
  gtk_signal_connect(GTK_OBJECT(darea), "motion_notify_event",
                     GTK_SIGNAL_FUNC(event_motion), NULL);

  /* Get mouse/selection labels */
  label_mouse = GTK_LABEL(glade_xml_get_widget(glade, "label_mouse"));
  label_selection = GTK_LABEL(glade_xml_get_widget(glade, "label_selection"));

  /* Get COSI request widgets */
  button_request = GTK_BUTTON(glade_xml_get_widget(glade, "button_request"));
  gtk_signal_connect(GTK_OBJECT(button_request), "clicked",
                     GTK_SIGNAL_FUNC(ocr_request), NULL);

  /* Show main window */
  gtk_widget_show(window);

  /* Init selection management */
  sel_init(darea);

  /* Hook to frame buffer */
  if (shmid > 0) {
	  if (setup_fb(shmid)) {
		  return 1;
	  }
  }

  /* Setup stdin command handler */
  gdk_input_add(fileno(stdin), GDK_INPUT_READ, (GdkInputFunction) command, NULL);

  /* Enter GTK processing loop */
  gtk_main();

  return 0;
}
