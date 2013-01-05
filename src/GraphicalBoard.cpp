#include "defines.h"
#include "GraphicalBoard.h"

using namespace std;

GraphicalBoard::GraphicalBoard()
{
  this->dArea = gtk_drawing_area_new();

  gtk_widget_set_size_request(this->dArea,320,320);
  gtk_widget_add_events(this->dArea, GDK_BUTTON_PRESS_MASK); 
  g_signal_connect(this->dArea,"button-press-event",G_CALLBACK(onClick),(gpointer)this);
  g_signal_connect(G_OBJECT(this->dArea),"draw",G_CALLBACK(onDraw),(gpointer)this);

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      {
	this->effect[i][j] = NONE;
	this->stone[i][j] = EMPTY;

	if(i == j)
	  this->stone[i][j] = P1;
	if(3 - i == j)
	  this->stone[i][j] = P2;
      }

  choosen.a = -1;
  choosen.b = -1;
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

  string boardf = DATA("board.png");
  string p1f = DATA("stone0.png");
  string p2f = DATA("stone1.png");
  string hilightf = DATA("highlight.png");
  string lolightf = DATA("gray.png");

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
  	  case P1:
  	    cairo_set_source_surface(cr,p1,45+j*60,45+i*60);
  	    cairo_paint(cr);
  	    break;
  	  case P2:
  	    cairo_set_source_surface(cr,p2,45+j*60,45+i*60);
  	    cairo_paint(cr);
  	    break;
	  case EMPTY:
	    ;
  	  }
  	switch(gBoard->effect[i][j])
  	  {
  	  case LIGHT:
  	    cairo_set_source_surface(cr,hilight,45+j*60,45+i*60);
  	    cairo_paint(cr);
  	    break;
  	  case GRAY:
  	    cairo_set_source_surface(cr,lolight,45+j*60,45+i*60);
  	    cairo_paint(cr);
	  case NONE:
	    ;
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
  // for(int i = 0; i < 4; i++)
  //   for(int j = 0; j < 4; j++)
  //     effect[i][j] = NONE;

  x -= 45;
  y -= 45;

  if(x > 0 && y > 0)
    {
      if(x%60 <= 50 && y%60 <= 50 && x/60 < 4 && y/60 < 4)
	{
	  int a = y / 60, b = x / 60;

	  if(this->choosen.a >= 0 && this->effect[a][b] == LIGHT && (this->choosen.a != a || this->choosen.b != b))
	    {
	      for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		  this->effect[i][j] = NONE;

	      //vector of move direction
	      int vecA = a - this->choosen.a;
	      int vecB = b - this->choosen.b;
	      g_print("here %d %d ",vecA,vecB);
	      //normalization
	      vecA = vecA < -1 ? -1 : vecA;
	      vecA = vecA > 1 ? 1 : vecA;
	      vecB = vecB < -1 ? -1 : vecB;
	      vecB = vecB > 1 ? 1 : vecB;
	      //moving till end or stone
	      while(a + vecA >= 0 && a + vecA < 4 && b + vecB >= 0 && b + vecB < 4 && this->stone[a+vecA][b+vecB] == EMPTY)
		{
		  a += vecA;
		  b += vecB;
		}

	      this->stone[a][b] = this->stone[this->choosen.a][this->choosen.b];
	      this->stone[this->choosen.a][this->choosen.b] = EMPTY;

	      this->choosen.a = -1;
	      this->choosen.b = -1;
	    }
	  else if(this->choosen.a >= 0 && this->effect[a][b] == LIGHT)
	    {
	      for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		  this->effect[i][j] = NONE;

	      this->choosen.a = -1;
	      this->choosen.b = -1;
	    }
	  else if(choosen.a < 0 && this->stone[a][b] == P1)
	    {
	      this->effect[a][b] = LIGHT;

	      //vertical
	      for(int i = b + 1; i < 4; i++)
		if(this->stone[a][i] == EMPTY)
		  this->effect[a][i] = LIGHT;
		else
		  break;
	      for(int i = b - 1; i >= 0; i--)
		if(this->stone[a][i] == EMPTY)
		  this->effect[a][i] = LIGHT;
		else
		  break;
	      //horizontal
	      for(int i = a + 1; i < 4; i++)
		if(this->stone[i][b] == EMPTY)
		  this->effect[i][b] = LIGHT;
		else
		  break;
	      for(int i = a - 1; i >= 0; i--)
		if(this->stone[i][b] == EMPTY)
		  this->effect[i][b] = LIGHT;
		else
		  break;
	      //right slant
	      for(int i = 1; a + i < 4 && b + i < 4; i++)
		if(this->stone[a+i][b+i] == EMPTY)
		  this->effect[a+i][b+i] = LIGHT;
		else
		  break;
	      for(int i = 1; a - i >= 0 && b - i >= 0; i++)
		if(this->stone[a-i][b-i] == EMPTY)
		  this->effect[a-i][b-i] = LIGHT;
		else
		  break;
	      //left slant
	      for(int i = 1; a + i < 4 && b - i >= 0; i++)
		if(this->stone[a+i][b-i] == EMPTY)
		  this->effect[a+i][b-i] = LIGHT;
		else
		  break;
	      for(int i = 1; a - i >= 0 && b + i < 4; i++)
		if(this->stone[a-i][b+i] == EMPTY)
		  this->effect[a-i][b+i] = LIGHT;
		else
		  break;

	      this->choosen.a = a;
	      this->choosen.b = b;
	    }
	}
    }
}

