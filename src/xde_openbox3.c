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

static char *
xde_wrap_icon(char *file)
{
	char *icon = file ? g_strdup_printf(" icon=\"%s\"", file) : strdup("");

	free(file);
	return (icon);
}

static GList *
xde_create(MenuContext *ctx, Style style, const char *name)
{
	GMenuTreeDirectory *dir;
	GList *result = NULL;
	GList *entries = NULL;
	char *s;

	ctx->output = NULL;

	s = g_strdup_printf("%s\n\n", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	ctx->output = g_list_append(ctx->output, s);
	s = g_strdup_printf("%s\n\n", "<openbox_menu xmlns=\"http://openbox.org/3.4/menu\">");
	ctx->output = g_list_append(ctx->output, s);

	if (!(dir = gmenu_tree_get_root_directory(ctx->tree))) {
		EPRINTF("could not get root directory\n");
		return (result);
	}
	xde_reset_indent(ctx, 0);
	xde_increase_indent(ctx);
	entries = ctx->ops.menu(ctx, dir);
	xde_decrease_indent(ctx);

	switch(style) {
	case StyleFullmenu:
	default:
		result = ctx->rootmenu(ctx, entries);
		break;
	case StyleAppmenu:
		if (!name)
			name = gmenu_tree_directory_get_name(dir);
		result = ctx->appmenu(ctx, entries, name);
		break;
	case StyleEntries:
		break;
	}
	result = g_list_concat(ctx->output, result);
	ctx->output = NULL;

	s = g_strdup_printf("\n%s\n", "</openbox_menu>");
	result = g_list_append(result, s);
	return (result);
}

static GList *
xde_wmmenu(MenuContext *ctx)
{
	GList *text = NULL;
	GList *xsessions, *xsession;
	char *icon;
	char *s;

	icon = xde_wrap_icon(xde_get_icon(ctx, "gtk-quit"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s>\n",
			ctx->indent,
			"Window Managers Menu",
			"Window Managers",
			icon);
	text = g_list_append(text, s);
	free(icon);
	xde_increase_indent(ctx);
	xsessions = xde_get_xsessions();
	for (xsession = xsessions; xsession; xsession = xsession->next) {
		XdeXsession *xsess = xsession->data;

		if (strncasecmp(xsess->key, "openbox", 7) == 0)
			continue;
		icon = xde_get_entry_icon(ctx, xsess->entry, "preferences-system-windows",
				"metacity", GET_ENTRY_ICON_FLAG_XPM|GET_ENTRY_ICON_FLAG_PNG);
		icon = xde_wrap_icon(icon);
		s = g_strdup_printf("%s<item label=\"%s\"%s>\n",
				ctx->indent,
				xsess->name,
				icon);
		text = g_list_append(text, s);
		s = g_strdup_printf("%s  <action name=\"Restart\">\n",
				ctx->indent);
		text = g_list_append(text, s);
		s = g_strdup_printf("%s    <command>xdg-launch --pointer -X %s</command>\n",
				ctx->indent,
				xsess->key);
		text = g_list_append(text, s);
		s = g_strdup_printf("%s  </action>\n",
				ctx->indent);
		text = g_list_append(text, s);
		s = g_strdup_printf("%s</item>\n",
				ctx->indent);
		text = g_list_append(text, s);
		free(icon);
	}
	xde_decrease_indent(ctx);
	s = g_strdup_printf("%s</menu>\n\n", ctx->indent);
	text = g_list_append(text, s);
	xde_free_xsessions(xsessions);
	return (text);
}

static GList *
xde_appmenu(MenuContext *ctx, GList *entries, const char *name)
{
	GList *text = NULL;
	char *icon;
	char *s;

	icon = xde_wrap_icon(xde_get_icon2(ctx, "start-here", "folder"));
	s = g_strdup_printf("%s<menu id=\"%s Menu\" label=\"%s\"%s>\n",
			    ctx->indent, name, name, icon);
	text = g_list_append(text, s);
	text = g_list_concat(text, entries);
	s = g_strdup_printf("%s</menu>\n", ctx->indent);
	text = g_list_append(text, s);

	free(icon);
	return (text);
}

static GList *
xde_obmenu(MenuContext *ctx)
{
	GList *text = NULL;
	char *icon;
	char *s;

	ctx->output = g_list_concat(ctx->output, ctx->wmmenu(ctx));

	icon = xde_wrap_icon(xde_get_icon(ctx, "openbox"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s>\n",
			    ctx->indent, "Openbox", "Openbox", icon);
	text = g_list_append(text, s);
	free(icon);

	xde_increase_indent(ctx);

	icon = xde_wrap_icon(xde_get_icon(ctx, "preferences-desktop-display"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s />\n",
			    ctx->indent, "client-list-menu", "Desktops", icon);
	text = g_list_append(text, s);
	free(icon);

	icon = xde_wrap_icon(xde_get_icon(ctx, "preferences-system-windows"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s />\n",
			    ctx->indent, "client-list-combined-menu", "Windows", icon);
	text = g_list_append(text, s);
	free(icon);

	icon = xde_wrap_icon(xde_get_icon(ctx, "gtk-quit"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s />\n",
			    ctx->indent, "Window Managers Menu", "Window Managers", icon);
	text = g_list_append(text, s);
	free(icon);

	icon = xde_wrap_icon(xde_get_icon(ctx, "style"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s execute=\"%s\" />\n",
			    ctx->indent, "Themes Menu", "Themes", icon, "xde-style -m -t");
	text = g_list_append(text, s);
	free(icon);

	icon = xde_wrap_icon(xde_get_icon(ctx, "style"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s execute=\"%s\" />\n",
			    ctx->indent, "Styles Menu", "Styles", icon, "xde-style -m");
	text = g_list_append(text, s);
	free(icon);

	xde_decrease_indent(ctx);

	s = g_strdup_printf("%s</menu>\n\n", ctx->indent);
	text = g_list_append(text, s);

	return (text);
}

static GList *
xde_rootmenu(MenuContext *ctx, GList *entries)
{
	GList *text = NULL;
	char *icon;
	char *s;

	ctx->output = g_list_concat(ctx->output, xde_obmenu(ctx));

	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\">\n",
			    ctx->indent, "root-menu", "Openbox 3");
	text = g_list_append(text, s);

	text = g_list_concat(text, entries);

	xde_increase_indent(ctx);

	s = g_strdup_printf("%s<separator />\n", ctx->indent);
	text = g_list_append(text, s);

	icon = xde_wrap_icon(xde_get_icon(ctx, "openbox"));
	s = g_strdup_printf("%s<menu id=\"%s\" label=\"%s\"%s />\n", ctx->indent, "Openbox",
			    "Openbox", icon);
	text = g_list_append(text, s);
	free(icon);

	if (options.filename) {
		icon = xde_wrap_icon(xde_get_icon(ctx, "gtk-refresh"));
		s = g_strdup_printf("%s<item label=\"%s\"%s>\n", ctx->indent, "Refresh Menu",
				    icon);
		text = g_list_append(text, s);
		free(icon);
		s = g_strdup_printf("%s  <action name=\"Execute\">\n", ctx->indent);
		text = g_list_append(text, s);

		s = g_strdup_printf("%s    <command>%s %s</command>\n", ctx->indent,
				    "xde-menugen -format openbox3 -desktop OPENBOX -launch -o",
				    options.filename);
		text = g_list_append(text, s);

		s = g_strdup_printf("%s  </action>\n", ctx->indent);
		text = g_list_append(text, s);

		s = g_strdup_printf("%s</item>\n", ctx->indent);
		text = g_list_append(text, s);
	}

	icon = xde_wrap_icon(xde_get_icon(ctx, "gtk-redo-ltr"));
	s = g_strdup_printf("%s<item label=\"%s\"%s>\n", ctx->indent, "Reload", icon);
	text = g_list_append(text, s);
	free(icon);
	s = g_strdup_printf("%s  <action name=\"Reconfigure\" />\n", ctx->indent);
	text = g_list_append(text, s);
	s = g_strdup_printf("%s</item>\n", ctx->indent);
	text = g_list_append(text, s);

	icon = xde_wrap_icon(xde_get_icon(ctx, "gtk-refresh"));
	s = g_strdup_printf("%s<item label=\"%s\"%s>\n", ctx->indent, "Restart", icon);
	text = g_list_append(text, s);
	free(icon);
	s = g_strdup_printf("%s  <action name=\"Restart\" />\n", ctx->indent);
	text = g_list_append(text, s);
	s = g_strdup_printf("%s</item>\n", ctx->indent);
	text = g_list_append(text, s);

	s = g_strdup_printf("%s<separator />\n", ctx->indent);
	text = g_list_append(text, s);

	icon = xde_wrap_icon(xde_get_icon(ctx, "gtk-quit"));
	s = g_strdup_printf("%s<item label=\"%s\"%s>\n", ctx->indent, "Exit", icon);
	text = g_list_append(text, s);
	free(icon);
	s = g_strdup_printf("%s  <action name=\"Exit\" />\n", ctx->indent);
	text = g_list_append(text, s);
	s = g_strdup_printf("%s</item>\n", ctx->indent);
	text = g_list_append(text, s);

	xde_decrease_indent(ctx);

	s = g_strdup_printf("%s</menu>\n", ctx->indent);
	text = g_list_append(text, s);

	return (text);
}

static GList *
xde_build(MenuContext *ctx, GMenuTreeItemType type, gpointer item)
{
	return xde_build_simple(ctx, type, item);
}

static GList *
xde_menu(MenuContext *ctx, GMenuTreeDirectory *menu)
{
	GList *text;

	text = xde_menu_simple(ctx, menu);
	return (text);
}

static GList *
xde_separator(MenuContext *ctx, GMenuTreeSeparator *sep)
{
	GList *text = NULL;

	text = g_list_append(text, g_strdup_printf("%s<separator />\n", ctx->indent));
	return (text);
}

static GList *
xde_header(MenuContext *ctx, GMenuTreeHeader *hdr)
{
	GMenuTreeDirectory *dir;
	GList *text = NULL;
	const char *name, *path;
	char *icon = NULL, *s;
	char *esc;

	if (!(dir = gmenu_tree_header_get_directory(hdr)))
		return (text);

	name = gmenu_tree_directory_get_name(dir);

	esc = g_markup_escape_text(name, -1);

	if ((path = gmenu_tree_directory_get_desktop_file_path(dir))) {
		GKeyFile *file = g_key_file_new();

		g_key_file_load_from_file(file, path, G_KEY_FILE_NONE, NULL);
		icon = xde_get_entry_icon(ctx, file, "folder", "unknown",
					  GET_ENTRY_ICON_FLAG_XPM | GET_ENTRY_ICON_FLAG_PNG |
					  GET_ENTRY_ICON_FLAG_JPG | GET_ENTRY_ICON_FLAG_SVG);
		icon = xde_wrap_icon(icon);
		g_key_file_unref(file);
	} else
		icon = xde_wrap_icon(icon);

	s = g_strdup_printf("%s<separator label=\"%s\"%s />\n", ctx->indent, esc, icon);
	text = g_list_append(text, s);
	free(icon);
	g_free(esc);

	text = g_list_concat(text, ctx->ops.directory(ctx, dir));
	return (text);
}

static GList *
xde_directory(MenuContext *ctx, GMenuTreeDirectory *dir)
{
	GList *text = NULL;
	const char *name, *path;
	char *icon = NULL, *s;
	char *esc;
	int level;

	name = gmenu_tree_directory_get_name(dir);

	esc = g_markup_escape_text(name, -1);

	if ((path = gmenu_tree_directory_get_desktop_file_path(dir))) {
		GKeyFile *file = g_key_file_new();

		g_key_file_load_from_file(file, path, G_KEY_FILE_NONE, NULL);
		icon = xde_get_entry_icon(ctx, file, "folder", "unknown",
					  GET_ENTRY_ICON_FLAG_XPM | GET_ENTRY_ICON_FLAG_PNG |
					  GET_ENTRY_ICON_FLAG_JPG | GET_ENTRY_ICON_FLAG_SVG);
		icon = xde_wrap_icon(icon);
		g_key_file_unref(file);
	} else
		icon = xde_wrap_icon(icon);

	level = xde_reset_indent(ctx, 0);
	s = g_strdup_printf("%s<menu id=\"%s Menu\" label=\"%s\"%s>\n", ctx->indent, esc, esc,
			    icon);
	text = g_list_append(text, s);
	text = g_list_concat(text, ctx->ops.menu(ctx, dir));
	s = g_strdup_printf("%s</menu> <!-- %s Menu -->\n\n", ctx->indent, esc);
	text = g_list_append(text, s);
	level = xde_reset_indent(ctx, level);

	ctx->output = g_list_concat(ctx->output, text);
	s = g_strdup_printf("%s<menu id=\"%s Menu\" label=\"%s\"%s />\n", ctx->indent, esc, esc,
			    icon);
	text = g_list_append(NULL, s);

	free(icon);
	g_free(esc);
	return (text);
}

static GList *
xde_entry(MenuContext *ctx, GMenuTreeEntry *ent)
{
	GDesktopAppInfo *info;
	GList *text = NULL;
	const char *name, *exec, *path;
	char *esc1, *esc2, *cmd;
	char *icon = NULL, *wrap, *s;
	gboolean notify;
	char *wmclass;

	info = gmenu_tree_entry_get_app_info(ent);
	name = g_app_info_get_name(G_APP_INFO(info));

	esc1 = g_markup_escape_text(name, -1);

	if (options.launch) {
		char *p, *str = strdup(gmenu_tree_entry_get_desktop_file_id(ent));

		if ((p = strstr(str, ".desktop")))
			*p = '\0';
		cmd = g_strdup_printf("xdg-launch --pointer %s", str);
		free(str);
	} else {
		exec = g_app_info_get_commandline(G_APP_INFO(info));
		cmd = g_strdup(exec);
	}

	esc2 = g_markup_escape_text(cmd, -1);

	if ((path = gmenu_tree_entry_get_desktop_file_path(ent))) {
		icon = xde_get_app_icon(ctx, info, "exec", "unknown",
					  GET_ENTRY_ICON_FLAG_XPM | GET_ENTRY_ICON_FLAG_PNG |
					  GET_ENTRY_ICON_FLAG_JPG | GET_ENTRY_ICON_FLAG_SVG);
		wrap = xde_wrap_icon(strdup(icon));

		notify = g_desktop_app_info_get_boolean(info, G_KEY_FILE_DESKTOP_KEY_STARTUP_NOTIFY);
		wmclass = g_desktop_app_info_get_string(info, G_KEY_FILE_DESKTOP_KEY_STARTUP_WM_CLASS);
	} else {
		wrap = xde_wrap_icon(strdup(icon));
		notify = FALSE;
		wmclass = NULL;
	}

	s = g_strdup_printf("%s<item label=\"%s\"%s>\n", ctx->indent, esc1, wrap);
	text = g_list_append(text, s);
	s = g_strdup_printf("%s  <action name=\"Execute\">\n", ctx->indent);
	text = g_list_append(text, s);
	s = g_strdup_printf("%s    <command>%s</command>\n", ctx->indent, esc2);
	text = g_list_append(text, s);

	if (notify || wmclass) {
		s = g_strdup_printf("%s    <startupnotify>\n", ctx->indent);
		text = g_list_append(text, s);
		if (notify) {
			s = g_strdup_printf("%s      <enabled>yes</enabled>\n", ctx->indent);
			text = g_list_append(text, s);
		}
		if (wmclass) {
			s = g_strdup_printf("%s      <wmclass>%s</wmclass>\n", ctx->indent, wmclass);
			text = g_list_append(text, s);
		}
		s = g_strdup_printf("%s      <name>%s</name>\n", ctx->indent, esc1);
		text = g_list_append(text, s);
		if (icon) {
			s = g_strdup_printf("%s      <icon>%s</icon>\n", ctx->indent, icon);
			text = g_list_append(text, s);
		}
		s = g_strdup_printf("%s    </startupnotify>\n", ctx->indent);
		text = g_list_append(text, s);
	}

	s = g_strdup_printf("%s  </action>\n", ctx->indent);
	text = g_list_append(text, s);
	s = g_strdup_printf("%s</item>\n", ctx->indent);
	text = g_list_append(text, s);

	free(wrap);
	free(icon);
	g_free(wmclass);
	g_free(esc1);
	g_free(esc2);
	free(cmd);
	return (text);
}

static GList *
xde_alias(MenuContext *ctx, GMenuTreeAlias *als)
{
	return xde_alias_simple(ctx, als);
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
	.name = "openbox3",
	.version = VERSION,
	.tree = NULL,
	.level = 0,
	.iconflags = 0
//              | GTK_ICON_LOOKUP_NO_SVG
//              | GTK_ICON_LOOKUP_FORCE_SVG
//              | GTK_ICON_LOOKUP_USE_BUILTIN
//              | GTK_ICON_LOOKUP_GENERIC_FALLBACK
//              | GTK_ICON_LOOKUP_FORCE_SIZE
	    ,
	.output = NULL,
	.create = &xde_create,
	.wmmenu = &xde_wmmenu,
	.appmenu = &xde_appmenu,
	.rootmenu = &xde_rootmenu,
	.build = &xde_build,
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