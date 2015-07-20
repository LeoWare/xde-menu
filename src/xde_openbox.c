/*****************************************************************************

 Copyright (c) 2008-2015  Monavacon Limited <http://www.monavacon.com/>
 Copyright (c) 2001-2008  OpenSS7 Corporation <http://www.openss7.com/>
 Copyright (c) 1997-2001  Brian F. G. Bidulock <bidulock@openss7.org>

 All Rights Reserved.

 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, version 3 of the license.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>, or write to the
 Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 -----------------------------------------------------------------------------

 U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
 behalf of the U.S. Government ("Government"), the following provisions apply
 to you.  If the Software is supplied by the Department of Defense ("DoD"), it
 is classified as "Commercial Computer Software" under paragraph 252.227-7014
 of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
 successor regulations) and the Government is acquiring only the license rights
 granted herein (the license rights customarily provided to non-Government
 users).  If the Software is supplied to any unit or agency of the Government
 other than DoD, it is classified as "Restricted Computer Software" and the
 Government's rights in the Software are defined in paragraph 52.227-19 of the
 Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
 the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
 (or any successor regulations).

 -----------------------------------------------------------------------------

 Commercial licensing and support of this software is available from OpenSS7
 Corporation at a fee.  See http://www.openss7.com/

 *****************************************************************************/

#include "xde-menu.h"

static GList *
xde_wmmenu(MenuContext *ctx)
{
	return NULL;
}

static GList *
xde_appmenu(MenuContext *ctx, GList *entries, const char *name)
{
	return NULL;
}

static GList *
xde_rootmenu(MenuContext *ctx, GList *entries)
{
	return NULL;
}

static GList *
xde_menu(MenuContext *ctx, GMenuTreeDirectory *menu)
{
	return NULL;
}

static GList *
xde_directory(MenuContext *ctx, GMenuTreeDirectory *dir)
{
	return NULL;
}

static GList *
xde_header(MenuContext *ctx, GMenuTreeHeader *hdr)
{
	return NULL;
}

static GList *
xde_separator(MenuContext *ctx, GMenuTreeSeparator *sep)
{
	return NULL;
}

static GList *
xde_entry(MenuContext *ctx, GMenuTreeEntry *ent)
{
	return NULL;
}

static GList *
xde_alias(MenuContext *ctx, GMenuTreeAlias *als)
{
	return NULL;
}

static GList *
xde_themes(MenuContext *ctx)
{
	return NULL;
}

static GList *
xde_styles(MenuContext *ctx)
{
	return NULL;
}

MenuContext xde_menu_ops = {
	.name = "openbox",
	.version = VERSION,
	.tree = NULL,
	.level = 0,
	.output = NULL,
	.wmmenu = &xde_wmmenu,
	.appmenu = &xde_appmenu,
	.rootmenu = &xde_rootmenu,
	.ops = {
		.menu = &xde_menu,
		.directory = &xde_directory,
		.header = &xde_header,
		.separator = &xde_separator,
		.entry = &xde_entry,
		.alias = &xde_alias,
		},
	.themes = &xde_themes,
	.styles = &xde_styles,
};

