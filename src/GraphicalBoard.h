#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <iostream>
#include <gtk/gtk.h>
#include <stdlib.h>

#include "Board.h"

class GraphicalBoard
{
 public:
  GraphicalBoard();
  ~GraphicalBoard();
  void handleClick(int x, int y);
  GtkWidget * getDrawingArea();

  static void onClick(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
  static gboolean onDraw(GtkWidget * widget, cairo_t * cr, gpointer user_data);

 private:
  GtkWidget * dArea;
  struct { int a, b; } choosen;
  Board m_board;
  enum { NONE, GRAY, LIGHT } effect[4][4];
};

#endif
