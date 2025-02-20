#include <gtk/gtk.h>

static void button_clicked(GtkWidget *widget, gpointer data) {
  int button_id = GPOINTER_TO_INT(data);
  g_print("Button %d clicked\n", button_id);
  gtk_widget_set_sensitive(widget, FALSE);
  gtk_widget_set_opacity(widget, 0.0);
}

int main(int   argc, char *argv[]) {
  GtkBuilder *builder;
  GtkCssProvider *css_provider;
  GObject *window;
  GObject *button;
  GError *error = NULL;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, "builder.xml", &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  window = gtk_builder_get_object(builder, "window");
  g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  css_provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(css_provider, "style.css", &error);
  if (error != NULL) {
    g_printerr("Error loading CSS file: %s\n", error->message);
    g_clear_error(&error);
  }

  gtk_style_context_add_provider_for_screen(
        gtk_window_get_screen(GTK_WINDOW(window)),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
  );

  for (int i = 0; i < 9; i++) {
    char button_name[10];
    snprintf(button_name, sizeof(button_name), "button%d", i); 
    button = gtk_builder_get_object(builder, button_name);
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i));
  }

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}
