#include "TicTacToe.h"
#include <gtk/gtk.h>

#define SIZE 9
static int turn = 0;
static int board[SIZE] = {0}; 
static GtkWidget *buttons[SIZE];

static void button_clicked(GtkWidget *widget, gpointer data) {
    int button_id = GPOINTER_TO_INT(data);
    if (board[button_id] != 0) return;

    const char *img_path = (turn % 2 == 0) ? "circle.png" : "cross.png";
    board[button_id] = (turn % 2 == 0) ? 1 : 2;
    
    GtkWidget *image = gtk_image_new_from_file(img_path);
    gtk_button_set_image(GTK_BUTTON(widget), image);
    gtk_widget_set_sensitive(widget, FALSE);
    
    turn++;
    check_winner();
}

static void check_winner() {
    int wins[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        int a = wins[i][0], b = wins[i][1], c = wins[i][2];
        if (board[a] != 0 && board[a] == board[b] && board[a] == board[c]) {
            g_print("Player %d wins!\n", board[a]);
            for (int j = 0; j < SIZE; j++)
                gtk_widget_set_sensitive(buttons[j], FALSE);
            return;
        }
    }

    if (turn == SIZE) {
        g_print("It's a draw!\n");
    }
}

static void reset_game(GtkWidget *widget, gpointer data) {
    turn = 0;
    for (int i = 0; i < SIZE; i++) {
        board[i] = 0;
        gtk_button_set_image(GTK_BUTTON(buttons[i]), NULL);
        gtk_widget_set_sensitive(buttons[i], TRUE);
    }
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GObject *window, *button_reset;
    GError *error = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "builder.xml", &error)) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    window = gtk_builder_get_object(builder, "window");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    for (int i = 0; i < SIZE; i++) {
        char button_name[10];
        snprintf(button_name, sizeof(button_name), "button%d", i);
        buttons[i] = GTK_WIDGET(gtk_builder_get_object(builder, button_name));
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i));
    }

    button_reset = gtk_builder_get_object(builder, "button_reset");
    g_signal_connect(button_reset, "clicked", G_CALLBACK(reset_game), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    return 0;
}
