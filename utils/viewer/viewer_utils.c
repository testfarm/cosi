/**********************************************************************
 * $Id: viewer_utils.c 48 2012-04-17 21:06:56Z giroudon $
 *
 * COSI - The Common OCR Service Interface
 * Frame buffer viewer (GTK+) -- Various utilities
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

#include <stdarg.h>
#include <gtk/gtk.h>


char *lprintf(GtkLabel *label, char *fmt, ...)
{
  va_list ap;
  static char str[80];

  va_start(ap, fmt);
  vsnprintf(str, sizeof(str), fmt, ap);
  va_end(ap);

  gtk_label_set_label(label, str);

  return str;
}
