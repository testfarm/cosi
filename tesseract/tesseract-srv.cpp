/**********************************************************************
 * This is the COSI server using the
 * TESSERACT Optical-Character-Recognition library
 * Copyright (C) 2007-2016 Sylvain Giroudon
 *
 * Author:      Sylvain Giroudon
 * Created:     17-OCT-2014
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <tesseract/baseapi.h>

#define NAME "tesseract-srv: "

typedef struct {
	unsigned int width, height;          /* RGB frame size */
	unsigned int bytes_per_pixel;
	unsigned int bytes_per_line;
	unsigned char buf[1];                /* RGB frame buffer */
} cosi_buf_t;


typedef struct {
  int x, y;                   /* Window position */
  unsigned int width, height; /* Window size */
} cosi_geometry_t;


static cosi_buf_t *cosi_buf_map(int shmid)
{
	void *ptr;

	if ( (ptr = shmat(shmid, NULL, SHM_RDONLY)) == (void *) -1 ) {
		fprintf(stderr, NAME "shmat(%d): %s\n", shmid, strerror(errno));
		return NULL;
	}

	return (cosi_buf_t *) ptr;
}

static void cosi_buf_unmap(cosi_buf_t *fb)
{
	if (fb != NULL) {
		if (shmdt(fb) == -1) {
			fprintf(stderr, NAME "shmdt: %s\n", strerror(errno));
		}
	}
}


static void cosi_geometry_parse(char *str, cosi_geometry_t *g)
{
	char *s = strdup(str);
	char *sx, *sy;
	char *sheight;

	/* Retrieve window size */
	sheight = strchr(s, 'x');
	if (sheight != NULL) {
		char cx;
		int v;

		*(sheight++) = '\0';
		v = atoi(s);
		if (v < 0) {
			v = 0;
		}
		g->width = v;

		sx = sheight;
		while ((*sx != '\0') && (*sx != '+') && (*sx != '-')) {
			sx++;
		}

		cx = *sx;
		*sx = '\0';
		v = atoi(sheight);
		*sx = cx;

		if (v < 0) {
			v = 0;
		}
		g->height = v;
	}
	else {
		sx = s;
	}

	/* Retrieve window position */
	sy = sx;
	while ((*sy == '+') || (*sy == '-')) {
		sy++;
	}
	while ((*sy != '\0') && (*sy != '+') && (*sy != '-')) {
		sy++;
	}

	if (*sy != '\0') {
		g->y = atoi(sy);
		*sy = '\0';
	}

	g->x = atoi(sx);

	free(s);
}


static char *server_attr_parse(char *s, char **name, char **value)
{
	*name = NULL;
	*value = NULL;

	/* Strip leading blanks */
	while ((*s != '\0') && (*s <= ' ')) {
		s++;
	}

	if (*s == '\0') {
		return s;
	}

	/* Extract the attr name just before the equal sign (if any) */
	*name = s;
	while ((*s > ' ') && (*s != '=')) {
		s++;
	}

	if (*s == '=') {
		*value = s+1;
	}
	else {
		*value = NULL;
	}

	if (*s != '\0') {
		*(s++) = '\0';
	}

	/* Extract the attr value (if any) */
	if (*value != NULL) {
		if (*s == '"') {
			(*value)++;
			s++;

			while ((*s != '\0') && (*s != '"')) {
				s++;
			}
		}
		else {
			while (*s > ' ') {
				s++;
			}
		}

		if (*s != '\0') {
			*(s++) = '\0';
		}
	}

	/* Strip trailing blanks */
	while ((*s != '\0') && (*s <= ' ')) {
		s++;
	}

	return s;
}


static void server_loop(tesseract::TessBaseAPI *api, cosi_buf_t *fb)
{
	char command[80];

	while (fgets(command, sizeof(command), stdin) != NULL) {
		char *id = NULL;
		cosi_geometry_t g = {
			.x = 0,
			.y = 0,
			.width = fb->width,
			.height = fb->height
		};

		/* Strip leading blanks */
		char *s = command;
		while ( *s != '\0' ) {
			char *attr_name, *attr_value;

			/* Parse current request parameter */
			s = server_attr_parse(s, &attr_name, &attr_value);

			if ( attr_name != NULL ) {
				if ( strcmp(attr_name, "id") == 0 ) {
					id = attr_value;
				}
				else if ( strcmp(attr_name, "geometry") == 0 ) {
					cosi_geometry_parse(attr_value, &g);
				}
				else {
					/* Unrecognised attributes are ignored silently */
				}
			}
		}

		// Set region
		api->SetRectangle(g.x, g.y, g.width, g.height);

		// Process frame buffer region
		api->Recognize(0);
		tesseract::ResultIterator* ri = api->GetIterator();

		if (ri != 0) {
			tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
			int sp_x1 = -1, sp_y1 = -1, sp_y2 = -1;

			do {
				const char *str = ri->GetUTF8Text(level);
				int x1, y1, x2, y2;

				if (ri->IsAtBeginningOf(tesseract::RIL_BLOCK)) {
					ri->BoundingBox(tesseract::RIL_BLOCK, &x1, &y1, &x2, &y2);
					printf("<document id=\"%s\" geometry=\"%dx%d+%d+%d\">\n", id, x2-x1+1, y2-y1+1, x1, y1);
					printf("  <page>\n");
				}

				if (ri->IsAtBeginningOf(tesseract::RIL_TEXTLINE)) {
					ri->BoundingBox(tesseract::RIL_TEXTLINE, &x1, &y1, &x2, &y2);
					printf("    <line geometry=\"%dx%d+%d+%d\">\n", x2-x1+1, y2-y1+1, x1, y1);
					sp_x1 = -1;
				}

				ri->BoundingBox(level, &x1, &y1, &x2, &y2);

				if (sp_x1 >= 0) {
					if (y1 < sp_y1) {
						sp_y1 = y1;
					}
					if (y2 > sp_y2) {
						sp_y2 = y2;
					}
					printf("      <space geometry=\"%dx%d+%d+%d\" />\n", x2-sp_x1+1, sp_y2-sp_y1+1, sp_x1, sp_y1);
					sp_x1 = -1;
				}

				printf("      <box geometry=\"%dx%d+%d+%d\" value=\"%s\" />\n", x2-x1+1, y2-y1+1, x1, y1, str);

				if (ri->IsAtFinalElement(tesseract::RIL_WORD, level)) {
					sp_x1 = x2+1;
					sp_y1 = y1;
					sp_y2 = y2;
				}

				if (ri->IsAtFinalElement(tesseract::RIL_TEXTLINE, level)) {
					printf("    </line>\n");
				}

				if (ri->IsAtFinalElement(tesseract::RIL_BLOCK, level)) {
					printf("  </page>\n");
					printf("</document>\n");
				}

				delete[] str;
			} while (ri->Next(level));
		}
	}
}


int main(int argc, char *argv[])
{
	// Initialize tesseract-ocr with English, without specifying tessdata path
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, NAME "Could not initialize tesseract.\n");
		exit(1);
	}

	fprintf(stderr, NAME "Using Tesseract version %s\n", tesseract::TessBaseAPI::Version());

	// Map input image from shared memory buffer
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <shmid>", argv[0]);
		exit(1);
	}

	int shmid = atoi(argv[1]);
	cosi_buf_t *fb = cosi_buf_map(shmid);

	if (fb == NULL) {
		exit(2);
	}

	fprintf(stderr, NAME "shmid=%d %dx%d\n", shmid, fb->width, fb->height);

	api->SetImage(fb->buf, fb->width, fb->height, fb->bytes_per_pixel, fb->bytes_per_line);

	// Disable output buffering
	setbuf(stdout, NULL);

	// Enter server loop
	server_loop(api, fb);

	// Destroy used object and release memory
	api->End();

	// Detach shared memory buffer
	if (fb != NULL) {
		cosi_buf_unmap(fb);
	}

	return 0;
}
