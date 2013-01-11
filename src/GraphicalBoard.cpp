#include <iostream>

#include "defines.h"
#include "GraphicalBoard.h"
#include "Human.h"

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
  
  m_board = Board::initialBoard();
  m_currentPlayer = NULL;

  choosen.a = -1;
  choosen.b = -1;

  lock();

  signal_button_press_event().connect(sigc::mem_fun(this, &GraphicalBoard::onButtonPress));
}

GraphicalBoard::~GraphicalBoard()
{
  //TODO: free Img's or smth
}

void GraphicalBoard::proposeMove(Human* player)
{
  cout << "proposeMove(" << player->m_name << ")" << endl;

  m_currentPlayer = player;

  unlock();

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      if(m_board.m_fields[i][j] != FIELD_EMPTY)
	effect[i][j] = GRAY;

  Move * pMove = m_currentPlayer->getAvailableMoves();
  while(pMove != NULL)
    {
      cout << "f" << pMove->from << "t" << pMove->to << endl;
      effect[pMove->from%4][pMove->from/4] = NONE;
      pMove = pMove->next;
    }

  queue_draw();
}

bool GraphicalBoard::onButtonPress(GdkEventButton* event)
{
  cout << "onButtonPress(" << event->x << "x" << event->y << ")" << endl;

  if(locked)
    return true;

  int x = event->x - 45;
  int y = event->y - 45;

  //TODO: Po wykonaniu ruchu wywolaj Human::commitMoveProposal
  // currentPlayer->commitMoveProposal

  // Spowoduje to cykl wywolan funkcji, m.in
  // GraphicalBoard::onGameStateChanged

  if(x > 0 && y > 0)
    {
      if(x%60 <= 50 && y%60 <= 50 && x/60 < 4 && y/60 < 4)
	{
	  int a = y / 60, b = x / 60;

	  if(this->choosen.a >= 0 && this->effect[a][b] == LIGHT && (this->choosen.a != a || this->choosen.b != b))
	    {
	      Move move;
	      move.from = choosen.a + 4 * choosen.b;
	      move.to = a + 4 * b;

	      choosen.a = -1;
	      choosen.b = -1;

	      lock();
	      // m_currentPlayer->commitMoveProposal(move);
	    }
	  else if(this->choosen.a >= 0 && this->effect[a][b] == LIGHT)
	    {
	      for(int i = 0; i < 4; i++)
	      	for(int j = 0; j < 4; j++)
		  if(effect[i][j] == LIGHT)
		    effect[i][j] = NONE;

	      this->choosen.a = -1;
	      this->choosen.b = -1;
	    }
	  else if(choosen.a < 0 && m_board.m_fields[a][b] != FIELD_EMPTY && effect[a][b] != GRAY)
	    {
	      //IF xyx0 -> y -> 0xxx => THEN TODO: vectors & path enlightment
	      Move * pMove = m_currentPlayer->getAvailableMoves();
	      while(pMove != NULL)
		{
		  if(pMove->from % 4 == a && pMove->from / 4 == b)
		    effect[pMove->to%4][pMove->to/4] = LIGHT;
		  pMove = pMove->next;
		}
	      effect[a][b] = LIGHT;

	      choosen.a = a;
	      choosen.b = b;
	    }
	}
    }

  queue_draw();

  return true;
}

void GraphicalBoard::onGameNew(State s, const Player& p)
{
  //TODO: enter no move selection mode
  //Jezeli aktualny gracz bedzie czlowiekiem, to
  //sam wykona GraphicalBoard::proposeMove()
  m_currentPlayer = NULL;
  m_board = s.m_board;
  lock();
  
  queue_draw();
}

void GraphicalBoard::onGameStateChanged(State s, const Player& p)
{
  //TODO: enter no move selection mode
  //Jezeli aktualny gracz bedzie czlowiekiem, to
  //sam wykona GraphicalBoard::proposeMove()
  m_board = s.m_board;

  queue_draw();
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

void GraphicalBoard::lock()
{
  locked = true;

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      this->effect[i][j] = GRAY;  
}

void GraphicalBoard::unlock()
{
  locked = false;

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      this->effect[i][j] = NONE;
}
