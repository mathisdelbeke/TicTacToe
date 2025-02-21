#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <gtk/gtk.h>

static void button_clicked(GtkWidget *widget, gpointer data);
static void check_winner();
static void reset_game(GtkWidget *widget, gpointer data);

#endif

