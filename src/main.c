#include <agenda.h>
#include <agendaArquivo.h>
#include <gtk/gtk.h>
#include <time.h>

static gchar *on_detail_func(GtkCalendar *calendar, guint year, guint month,
                             guint day) {
  // Example detail information: a simple message including the date
  gchar *detail =
      g_strdup_printf("Detail for %d-%02d-%02d", year, month + 1, day);
  gtk_calendar_set_detail_height_rows(calendar, 8);
  return detail;
}
static GtkWidget* create_list_item(const gchar *text) {
    GtkWidget *box, *label, *edit_button, *delete_button;
    
    // Cria uma caixa horizontal para a linha
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    
    // Adiciona o texto
    label = gtk_label_new(text);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
    
    // Adiciona o botão de editar
    edit_button = gtk_button_new_with_label("Editar");
    gtk_box_pack_start(GTK_BOX(box), edit_button, FALSE, FALSE, 0);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL); // Exemplo de ação
    
    // Adiciona o botão de deletar
    delete_button = gtk_button_new_with_label("Deletar");
    gtk_box_pack_start(GTK_BOX(box), delete_button, FALSE, FALSE, 0);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(gtk_main_quit), NULL); // Exemplo de ação
    
    return box;
}
static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *calendar;
  GtkWidget *header;
  GtkWidget *box;
  GtkWidget *list_box;
    GtkWidget *scrolled_window;

  list_box = gtk_list_box_new();


  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  header = gtk_header_bar_new();
  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);

  gtk_box_pack_start(GTK_BOX(box), header, TRUE, TRUE, 0);
  

  // Create a new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "GtkCalendar Detail Example");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

  // Create a new GtkCalendar widget
  calendar = gtk_calendar_new();
  gtk_box_pack_start(GTK_BOX(box), calendar, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(window), box);

  // Set the detail function for the calendar
  gtk_calendar_set_detail_func(GTK_CALENDAR(calendar),
                               (GtkCalendarDetailFunc)on_detail_func, NULL,
                               NULL);

  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), list_box);
    
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);
    
    // Adicionar alguns itens de exemplo à lista
    gtk_list_box_insert(GTK_LIST_BOX(list_box), create_list_item("Item 1"), -1);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), create_list_item("Item 2"), -1);
    gtk_list_box_insert(GTK_LIST_BOX(list_box), create_list_item("Item 3"), -1);
  // Show all widgets
  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  // Create a new GtkApplication
  app = gtk_application_new("org.example.GtkCalendarDetail",
                            G_APPLICATION_FLAGS_NONE);

  // Connect the "activate" signal to the activate function
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  // Run the application and get the exit status
  status = g_application_run(G_APPLICATION(app), argc, argv);

  // Release the application object
  g_object_unref(app);

  return status;
}
