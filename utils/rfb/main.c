/**********************************************************************
 * $Id: main.c 50 2012-04-18 20:51:33Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * VNC (RFB) client: program body 
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <glib.h>

#include "rfbframe.h"

/*
 * TESTING:
 * Start VNC server on virtual display:
 *   $ Xvnc :1 -localhost -SecurityTypes None -desktop "COSI RFB test"
 * Start COSI VNC client and viewer:
 *   $ cosi-rfb :1 | cosi-viewer
 * Do anything you want on the VNC display:
 *   $ DISPLAY=:1 gnome-about
 */


static GMainLoop *loop = NULL;
static rfb_capture_t *capture;


void rfb_capture_event_update(rfb_capture_t *capture, frame_geometry_t *g)
{
	printf("UPDATE %ux%u%+d%+d\n", g->width, g->height, g->x, g->y);
	
}


void rfb_capture_event_bell(rfb_capture_t *capture)
{
    printf("BELL\n");
}


void rfb_capture_event_cuttext(rfb_capture_t *capture, unsigned char *cutbuf)
{
	unsigned char *s = cutbuf;

	printf("\rCUTTEXT \"");
	while (*s) {
		unsigned char c = *s;
		if ( (c >= ' ') && (c < 128) )
			printf("%c", c);
		else
			printf("\\%u", c);
	}
	printf("\"\n");
}


static void usage(void)
{
	fprintf(stderr, "Usage: cosi-rfb [<password>@]<host>[:<port>] [-shared] [-debug]\n");
	exit(1);
}


static void terminate(void)
{
  if ( capture != NULL )
    rfb_capture_close(capture);
  capture = NULL;

  if ( loop != NULL )
    g_main_loop_quit(loop);
  loop = NULL;
}


static void terminate_sig(int sig)
{
	terminate();
}


int main(int argc, char *argv[])
{
	char *peer = NULL;
	int shared = 0;
	int debug = 0;
	int i;

	/* Parse command arguments */
	for (i = 1; i < argc; i++) {
		char *args = argv[i];
		if (*args == '-') {
			if (strcmp(args, "-shared") == 0) {
				shared = 1;
			}
			else if (strcmp(args, "-debug") == 0) {
				debug = 1;
			}
			else {
				usage();
			}
		}
		else {
			if (peer == NULL) {
				peer = args;
			}
			else {
				usage();
			}
		}
	}

	if (peer == NULL) {
		usage();
	}

	/* Hook termination procedure at exit stack and signal handling */
	atexit(terminate);
	signal(SIGHUP, terminate_sig);
	signal(SIGQUIT, terminate_sig);
	signal(SIGTERM, terminate_sig);
	signal(SIGPIPE, terminate_sig);

	setbuf(stdout, NULL);

	/* Create GLib main loop */
	loop = g_main_loop_new(NULL, FALSE);

	/* Connect to VNC server */
	capture = rfb_capture_open(peer, shared, debug);
	if (capture == NULL) {
		exit(2);
	}

	/* Show frame buffer shmid */
	printf("SHMID %d\n", capture->shmid);

	/* Set refresh rate to 20 fps (period=50ms) */
	rfb_capture_set_period(capture, 50);

	/* Enter processing loop */
	g_main_loop_run(loop);

	/* Destroy main loop */
	g_main_loop_unref(loop);
	loop = NULL;

	terminate();

	return 0;
}
