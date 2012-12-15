#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <gtk/gtk.h>

class GraphicalBoard
{
  GtkWidget * dArea;
 public:
  GraphicalBoard();
  ~GraphicalBoard();
  static gboolean onDraw(GtkWidget * widget, cairo_t * cr, gpointer user_data);
  GtkWidget * getDrawingArea();
};

#endif
