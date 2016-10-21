/* Copyright (C) 2016 by Chunyang Xu <chunyang@xuchunyang.me>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

const gchar *version = "0.1.0";
const gchar *authors[] = {"Chunyang Xu <chunyang@xuchunyang.me>",
                          NULL};

void
on_about (GtkMenuItem *menu_item, gpointer user_data)
{
  gtk_show_about_dialog (GTK_WINDOW (user_data),
                         "program-name", "shadowsocks gtk3",
                         "comments", "A simple tray program to control shadowsocks.",
                         "authors", authors,
                         "website", "https://github.com/xuchunyang/shadowsocks-gtk3",
                         "website-label", "github.com/xuchunyang/shadowsocks-gtk3",
                         "version", version,
                         "license-type", GTK_LICENSE_GPL_3_0,
                         "logo-icon-name", "shadowsocks-gtk3",
                         NULL);
}

void
on_none_proxy (GtkMenuItem *item, gpointer data)
{
  if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (item)))
    {
      g_message ("switching to none proxy mode...");
      GError *error = NULL;
      if (!g_spawn_command_line_async ("gsettings set org.gnome.system.proxy mode 'none'",
                                       &error))
        {
          fprintf (stderr, "run gsettings failed: %s\n", error->message);
          g_error_free (error);
        }
    }
}

void
on_auto_proxy (GtkMenuItem *item, gpointer data)
{
  if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (item)))
    {
      g_message ("switching to auto proxy mode...");
      GError *error = NULL;
      if (!g_spawn_command_line_async ("gsettings set org.gnome.system.proxy mode 'auto'",
                                       &error))
        {
          fprintf (stderr, "run gsettings failed: %s\n", error->message);
          g_error_free (error);
        }
    }
}

void
on_global_proxy (GtkMenuItem *item, gpointer data)
{
  if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (item)))
    {
      g_message ("switching to global proxy mode...");
      GError *error = NULL;
      if (!g_spawn_command_line_async ("gsettings set org.gnome.system.proxy mode 'manual'",
                                       &error))
        {
          fprintf (stderr, "run gsettings failed: %s\n", error->message);
          g_error_free (error);
        }
    }
}

void
activate (GtkApplication *app, gpointer data)
{
  GtkWidget *window;
  AppIndicator *indicator;
  GtkWidget *indicator_menu;
  GtkWidget *item;
  GSList *group = NULL;
  /* Hmm, I'm really really really new to C */
  gboolean none_proxy = FALSE;
  gboolean auto_proxy = FALSE;
  gboolean global_proxy = FALSE;
  GSettings *setting;
  gchar *mode;

  window = gtk_application_window_new (app);
  indicator_menu = gtk_menu_new ();

  setting = g_settings_new ("org.gnome.system.proxy");
  mode = g_settings_get_string (setting, "mode");

  if (0 == g_strcmp0 ("manual", mode))
    global_proxy = TRUE;
  else if (0 == g_strcmp0 ("none", mode))
    none_proxy = TRUE;
  else if (0 == g_strcmp0 ("auto", mode))
    auto_proxy = TRUE;

  g_clear_object (&setting);
  g_free (mode);

  /* No Proxy Mode */
  item = gtk_radio_menu_item_new_with_label (group, "No Proxy");
  group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (item));
  if (none_proxy)
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), TRUE);
  g_signal_connect (item, "toggled", G_CALLBACK (on_none_proxy), NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), item);
  /* Auto Proxy Mode */
  item = gtk_radio_menu_item_new_with_label (group, "Auto Proxy");
  group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (item));
  if (auto_proxy)
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), TRUE);
  g_signal_connect (item, "toggled", G_CALLBACK (on_auto_proxy), NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), item);
  /* Global Proxy Mode */
  item = gtk_radio_menu_item_new_with_label (group, "Global Proxy");
  group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (item));
  if (global_proxy)
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), TRUE);
  g_signal_connect (item, "toggled", G_CALLBACK (on_global_proxy), NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), item);
  /* Separator */
  item = gtk_separator_menu_item_new ();
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), item);
  /* About */
  item = gtk_menu_item_new_with_mnemonic ("About");
  g_signal_connect (item, "activate", G_CALLBACK (on_about), window);
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), item);
  /* Quit */
  item = gtk_menu_item_new_with_mnemonic ("Quit");
  g_signal_connect_swapped (item, "activate", G_CALLBACK (g_application_quit), app);
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), item);

  indicator = app_indicator_new ("shadowsocks-gtk3",
                                 "shadowsocks-gtk3-menu-icon",
                                 APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
  app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
  app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));
  /* Popup the menu... */
  gtk_widget_show_all (indicator_menu);
  /* Hold */
  g_application_hold (G_APPLICATION (app));
}

int
main (int argc, char *argv[])
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("com.github.xuchunyang.shadowsocks-gtk3",
                             G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

/* Local Variables: */
/* compile-command: "gcc main.c `pkg-config --cflags --libs gtk+-3.0 appindicator3-0.1`" */
/* End: */
