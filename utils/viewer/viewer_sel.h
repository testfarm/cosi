/**********************************************************************
 * $Id: viewer_sel.h 48 2012-04-17 21:06:56Z giroudon $
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

#ifndef __VIEWER_SEL_H__
#define __VIEWER_SEL_H__

extern void sel_init(GtkDrawingArea *darea);
extern void sel_get(GdkRectangle *rect);

#endif /* __VIEWER_SEL_H__ */
