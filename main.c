#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

int
main (int argc, char *argv[])
{
  AppIndicator *indicator;
  GtkWidget *indicator_menu;

  gtk_init (NULL, NULL);

  indicator = app_indicator_new ("shadowsocks-gtk3",
                                 "starred",
                                 APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
  app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
  indicator_menu = gtk_menu_new ();
  app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));

  gtk_main ();
  return 0;
}

/* Local Variables: */
/* compile-command: "gcc main.c `pkg-config --cflags --libs gtk+-3.0 appindicator3-0.1`" */
/* End: */
