#include <iostream>

#include "defines.h"
#include "GraphicalBoard.h"

using namespace std;

GraphicalBoard::GraphicalBoard()
{
  set_size_request(320, 320);
  add_events(Gdk::BUTTON_PRESS_MASK);

  m_boardImg =
    Cairo::ImageSurface::create_from_png(DATA("board.png"));
  m_p1Img =
    Cairo::ImageSurface::create_from_png(DATA("stone0.png"));
  m_p2Img =
    Cairo::ImageSurface::create_from_png(DATA("stone1.png"));
  m_hilightImg =
    Cairo::ImageSurface::create_from_png(DATA("highlight.png"));
  m_lolightImg =
    Cairo::ImageSurface::create_from_png(DATA("gray.png"));

  signal_button_press_event().connect(sigc::mem_fun(this, &GraphicalBoard::onButtonPress));
  
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      this->effect[i][j] = NONE;
  
  m_board = Board::initialBoard();

  choosen.a = -1;
  choosen.b = -1;
}

GraphicalBoard::~GraphicalBoard()
{
}

bool GraphicalBoard::onButtonPress(GdkEventButton* event)
{
  cout << "onClick " << event->x << " " << event->y << endl;
  handleClick(event->x, event->y);
  queue_draw();
  return true;
}


bool GraphicalBoard::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  cout << "on_draw()" << endl;
  
  cr->set_source(m_boardImg, 10, 10);
  cr->paint();

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      {
  	switch(m_board.m_fields[i][j])
  	  {
  	  case FIELD_COLOR0:
	    cr->set_source(m_p1Img, 45+j*60, 45+i*60);
	    cr->paint();
  	    break;
  	  case FIELD_COLOR1:
	    cr->set_source(m_p2Img, 45+j*60, 45+i*60);
	    cr->paint();
  	    break;
	  case FIELD_EMPTY:
	    ;
  	  }
  	switch(this->effect[i][j])
  	  {
  	  case LIGHT:
	    cr->set_source(m_hilightImg, 45+j*60, 45+i*60);
	    cr->paint();
	    break;
  	  case GRAY:
	    cr->set_source(m_lolightImg, 45+j*60, 45+i*60);
	    cr->paint();
	  case NONE:
	    ;
  	  }
      }

  return true;
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
	      while(a + vecA >= 0 && a + vecA < 4 && b + vecB >= 0 && b + vecB < 4 && m_board.m_fields[a+vecA][b+vecB] == FIELD_EMPTY)
		{
		  a += vecA;
		  b += vecB;
		}

	      m_board.m_fields[a][b] = m_board.m_fields[this->choosen.a][this->choosen.b];
	      m_board.m_fields[this->choosen.a][this->choosen.b] = FIELD_EMPTY;

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
	  else if(choosen.a < 0 && m_board.m_fields[a][b] == FIELD_COLOR0)
	    {
	      this->effect[a][b] = LIGHT;

	      //vertical
	      for(int i = b + 1; i < 4; i++)
		if(m_board.m_fields[a][i] == FIELD_EMPTY)
		  this->effect[a][i] = LIGHT;
		else
		  break;
	      for(int i = b - 1; i >= 0; i--)
		if(m_board.m_fields[a][i] == FIELD_EMPTY)
		  this->effect[a][i] = LIGHT;
		else
		  break;
	      //horizontal
	      for(int i = a + 1; i < 4; i++)
		if(m_board.m_fields[i][b] == FIELD_EMPTY)
		  this->effect[i][b] = LIGHT;
		else
		  break;
	      for(int i = a - 1; i >= 0; i--)
		if(m_board.m_fields[i][b] == FIELD_EMPTY)
		  this->effect[i][b] = LIGHT;
		else
		  break;
	      //right slant
	      for(int i = 1; a + i < 4 && b + i < 4; i++)
		if(m_board.m_fields[a+i][b+i] == FIELD_EMPTY)
		  this->effect[a+i][b+i] = LIGHT;
		else
		  break;
	      for(int i = 1; a - i >= 0 && b - i >= 0; i++)
		if(m_board.m_fields[a-i][b-i] == FIELD_EMPTY)
		  this->effect[a-i][b-i] = LIGHT;
		else
		  break;
	      //left slant
	      for(int i = 1; a + i < 4 && b - i >= 0; i++)
		if(m_board.m_fields[a+i][b-i] == FIELD_EMPTY)
		  this->effect[a+i][b-i] = LIGHT;
		else
		  break;
	      for(int i = 1; a - i >= 0 && b + i < 4; i++)
		if(m_board.m_fields[a-i][b+i] == FIELD_EMPTY)
		  this->effect[a-i][b+i] = LIGHT;
		else
		  break;

	      this->choosen.a = a;
	      this->choosen.b = b;
	    }
	}
    }
}

