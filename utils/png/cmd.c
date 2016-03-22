/**********************************************************************
 * $Id: cmd.c 48 2012-04-17 21:06:56Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * This utility exports a shared memory frame buffer from a PNG image
 * file, and allows to send test requests to a COSI-compliant OCR agent.
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

#include <readline/readline.h>
#include <readline/history.h>

#define NAME "cosi-png"

#include "utils.h"
#include "pngfile.h"
#include "cosi.h"

#define COSI_PROMPT "COSI> "


int main(int argc, char *argv[])
{
  unsigned int width, height;
  unsigned char *rgb;
  cosi_buf_t *buf;
  int shmid = 0;
  char *cmd;

  if ( argc != 2 ) {
    fprintf(stderr, "Usage: " NAME " <png-file>\n");
    exit(1);
  }

  setbuf(stdout, NULL);

  /* Load PNG file */
  if ( png_load(argv[1], &width, &height, &rgb) )
    exit(2);
  eprintf("Image '%s' loaded: %ux%u\n", argv[1], width, height);

  /* Map image COSI frame buffer in shared memory */
  buf = cosi_buf_alloc(width, height, rgb, &shmid);

  /* Show frame buffer shmid */
  printf("SHMID %d\n", shmid);

  while ( (cmd = readline(COSI_PROMPT)) != NULL ) {
    char *args = cmd;

    // Skip leading blank chars from command
    while ( (*args != '\0') && (*args <= ' ') )
      args++;

    // Split first word
    while ( *args > ' ' )
      args++;
    if ( *args != '\0' )
      *(args++) = '\0';

    // Skip leading blank chars from arguments
    while ( (*args != '\0') && (*args <= ' ') )
      args++;

    if ( (strcmp(cmd, "exit") == 0) ||
	      (strcmp(cmd, "quit") == 0) ||
	      (strcmp(cmd, "bye") == 0) ) {
      break;
    }
    else if (*cmd != '\0') {
      eprintf("Unknown command '%s'\n", cmd);
    }

    free(cmd);
  }

  return 0;
}
