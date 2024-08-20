#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer *data) {
  GtkWidget *window;

  window = gtk_application_window_new(app);
  // tamanho e titulo

  gtk_window_set_title(GTK_WINDOW(window), "Menu Principal");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  gtk_window_present(GTK_WINDOW(window));
}
