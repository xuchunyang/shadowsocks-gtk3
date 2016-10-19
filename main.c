#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

#define VERSION "0.0"

void
on_about (GtkMenuItem *menu_item, gpointer user_data)
{
  /* This helps prevent multiple instances */
  if (!gtk_grab_get_current ())
    {
      const gchar *authors[] = {"Chunyang Xu <chunyang@xuchunyang.me>",
                                NULL};
      gtk_show_about_dialog (NULL,
                             "program-name", "shadowsocks gtk+",
                             "comments", "A simple tray program to control shadowsocks.",
                             "authors", authors,
                             "website", "https://github.com/xuchunyang/shadowsocks-gtk3",
                             "website-label", "Project Homepage",
                             "version", VERSION,
                             "license-type", GTK_LICENSE_GPL_3_0,
                             NULL);

    }
  else
    {
      /* A window is already open, so we present it to the user */
      GtkWidget *toplevel = gtk_widget_get_toplevel (gtk_grab_get_current ());
      gtk_window_present (GTK_WINDOW (toplevel));
    }
}

void
activate (GtkApplication *app, gpointer data)
{
  AppIndicator *indicator;
  GtkWidget *indicator_menu;
  GtkWidget *menu_item;

  indicator_menu = gtk_menu_new ();

  /* Auto Proxy Mode */
  menu_item = gtk_check_menu_item_new_with_label ("Auto Proxy Mode");
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), menu_item);
  /* Global Mode */
  menu_item = gtk_check_menu_item_new_with_label ("Global Mode");
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), menu_item);
  /* About */
  menu_item = gtk_menu_item_new_with_mnemonic ("About");
  g_signal_connect (menu_item, "activate", G_CALLBACK (on_about), NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), menu_item);
  /* Quit */
  menu_item = gtk_menu_item_new_with_mnemonic ("Quit");
  g_signal_connect_swapped (menu_item, "activate", G_CALLBACK (g_application_quit), app);
  gtk_menu_shell_append (GTK_MENU_SHELL (indicator_menu), menu_item);

  indicator = app_indicator_new ("shadowsocks-gtk3",
                                 "starred",
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
