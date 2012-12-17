#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <iostream>
#include <gtk/gtk.h>
#include <stdlib.h>

class GraphicalBoard
{
  GtkWidget * dArea;
  int stone[4][4];
  int effect[4][4];
 public:
  GraphicalBoard();
  ~GraphicalBoard();
  void handleClick(int x, int y);
  GtkWidget * getDrawingArea();

  static void onClick(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
  static gboolean onDraw(GtkWidget * widget, cairo_t * cr, gpointer user_data);
};

#endif
