#include "GraphicalBoard.h"

GraphicalBoard::GraphicalBoard()
{
  this->dArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(dArea,300,200);
  g_signal_connect(G_OBJECT(dArea),"draw",G_CALLBACK(onDraw),NULL);
}

GraphicalBoard::~GraphicalBoard()
{
}

gboolean GraphicalBoard::onDraw(GtkWidget * widget, cairo_t * cr, gpointer user_data)
{
  cr = gdk_cairo_create(gtk_widget_get_window(widget));
  g_print("draw!\n");
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 2.5);

  cairo_move_to(cr,0,0);
  cairo_line_to(cr,300,200);

  cairo_stroke(cr);

  cairo_destroy(cr);

  return false;
}

GtkWidget * GraphicalBoard::getDrawingArea()
{
  return this->dArea;
}
