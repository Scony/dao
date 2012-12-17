#include "GraphicalBoard.h"

using namespace std;

GraphicalBoard::GraphicalBoard()
{
  this->dArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(this->dArea,320,320);
  gtk_widget_add_events(this->dArea, GDK_BUTTON_PRESS_MASK); 
  // g_signal_connect(this->dArea,"button-press-event",G_CALLBACK(onClick),(gpointer)this);
  // g_signal_connect(G_OBJECT(this->dArea),"draw",G_CALLBACK(onDraw),(gpointer)this);

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      {
	this->effect[i][j] = 0;
	if(i == j)
	  this->stone[i][j] = 1;
	else if(3 - i == j)
	  this->stone[i][j] = 2;
	else
	  this->stone[i][j] = 0;
      }
}

GraphicalBoard::~GraphicalBoard()
{
}

void GraphicalBoard::onClick(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
  g_print("onClick %f %f\n",event->x,event->y);

  GraphicalBoard * gBoard = (GraphicalBoard*)user_data;

  gBoard->handleClick(event->x,event->y);

  gtk_widget_queue_draw(widget);
}

gboolean GraphicalBoard::onDraw(GtkWidget * widget, cairo_t * cr, gpointer user_data)
{
  g_print("onDraw\n");

  GraphicalBoard * gBoard = (GraphicalBoard*)user_data;

  string boardf = "../data/board.png";
  string p1f = "../data/stone0.png";
  string p2f = "../data/stone1.png";
  string hilightf = "../data/highlight.png";
  string lolightf = "../data/gray.png";

  cairo_surface_t * board = cairo_image_surface_create_from_png(boardf.c_str());
  cairo_surface_t * p1 = cairo_image_surface_create_from_png(p1f.c_str());
  cairo_surface_t * p2 = cairo_image_surface_create_from_png(p2f.c_str());
  cairo_surface_t * hilight = cairo_image_surface_create_from_png(hilightf.c_str());
  cairo_surface_t * lolight = cairo_image_surface_create_from_png(lolightf.c_str());

  cr = gdk_cairo_create(gtk_widget_get_window(widget));

  cairo_set_source_surface(cr,board,10,10);
  cairo_paint(cr);

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      {
  	switch(abs(gBoard->stone[i][j]))
  	  {
  	  case 1:
  	    cairo_set_source_surface(cr,p1,45+j*60,45+i*60);
  	    cairo_paint(cr);
  	    break;
  	  case 2:
  	    cairo_set_source_surface(cr,p2,45+j*60,45+i*60);
  	    cairo_paint(cr);
  	    break;
  	  }
  	switch(gBoard->effect[i][j])
  	  {
  	  case 1:
  	    cairo_set_source_surface(cr,hilight,45+j*60,45+i*60);
  	    cairo_paint(cr);
  	    break;
  	  case 2:
  	    cairo_set_source_surface(cr,lolight,45+j*60,45+i*60);
  	    cairo_paint(cr);
  	    break;
  	  }
      }

  cairo_surface_destroy(board);
  cairo_surface_destroy(p1);
  cairo_surface_destroy(p2);
  cairo_surface_destroy(hilight);
  cairo_surface_destroy(lolight);

  cairo_destroy(cr);

  return false;
}

GtkWidget * GraphicalBoard::getDrawingArea()
{
  return this->dArea;
}

void GraphicalBoard::handleClick(int x, int y)
{
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      stone[i][j] *= (stone[i][j] < 0 ? -1 : 1);
  x -= 45;
  y -= 45;
  if(x > 0 && y > 0)
    {
      if(x%60 <= 50 && y%60 <= 50 && x/60 < 4 && y/60 < 4)
	{
	  this->effect[y/60][x/60] = 2;
	}
    }
}
