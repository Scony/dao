#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <iostream>
#include <gtk/gtk.h>
#include <stdlib.h>

class GraphicalBoard
{
  GtkWidget * dArea;
  struct { int a, b; } choosen;
  enum { EMPTY, P1, P2 } stone[4][4];
  enum { NONE, GRAY, LIGHT } effect[4][4];
 public:
  GraphicalBoard();
  ~GraphicalBoard();
  void handleClick(int x, int y);
  GtkWidget * getDrawingArea();

  static void onClick(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
  static gboolean onDraw(GtkWidget * widget, cairo_t * cr, gpointer user_data);
};

#endif
