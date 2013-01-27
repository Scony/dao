#include <iostream>

#include "defines.h"
#include "GraphicalBoard.h"
#include "Human.h"
#include "Game.h"

using namespace std;

GraphicalBoard::GraphicalBoard()
{
  set_size_request(320, 320);
  add_events(Gdk::BUTTON_PRESS_MASK);

  m_boardImg =
    Cairo::ImageSurface::create_from_png(DATA("board.png"));
  m_stones[COLOR_RED] =
    Cairo::ImageSurface::create_from_png(DATA("stone0.png"));
  m_stones[COLOR_BLUE] =
    Cairo::ImageSurface::create_from_png(DATA("stone1.png"));
  m_hilightImg =
    Cairo::ImageSurface::create_from_png(DATA("highlight.png"));
  m_lolightImg =
    Cairo::ImageSurface::create_from_png(DATA("gray.png"));
  m_backgroundImg =
    Cairo::ImageSurface::create_from_png(DATA("background.png"));
  m_backgroundPattern =
    Cairo::SurfacePattern::create(m_backgroundImg);  
  m_backgroundPattern->set_extend(Cairo::EXTEND_REPEAT);
  m_board = Board::initialBoard();
  m_currentPlayer = NULL;

  m_choosen.a = -1;
  m_choosen.b = -1;
  lock();

  m_offset.x = 10;
  m_offset.y = 10;

  signal_button_press_event().connect(sigc::mem_fun(this, &GraphicalBoard::onButtonPress));
}

GraphicalBoard::~GraphicalBoard()
{
  //TODO: free Img's or smth
}

void GraphicalBoard::proposeMove(Human* player)
{
  // cout << "proposeMove(" << player->m_name << ")" << endl;

  m_currentPlayer = player;

  unlock();

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      if(m_board.m_fields[i][j] != FIELD_EMPTY)
	m_effect[i][j] = GRAY;

  MoveSet& moves = m_currentPlayer->getAvailableMoves();
  MoveSet::Iterator it = moves.begin();
  for(; it != moves.end(); it++)
    {
      const Move& move = it.at();
      cout << "f" << move.from << "t" << move.to << endl;
      m_effect[move.from/4][move.from%4] = NONE;
    }

  queue_draw();
}

bool GraphicalBoard::onButtonPress(GdkEventButton* event)
{
  // cout << "onButtonPress(" << (int)event->x << "x" << (int)event->y << ")" << endl;

  if(m_locked)
    return true;

  int x = event->x - (m_offset.x + 35);
  int y = event->y - (m_offset.y + 35);

  if(x > 0 && y > 0)
    {
      if(x%60 <= 50 && y%60 <= 50 && x/60 < 4 && y/60 < 4)
	{
	  int a = y / 60, b = x / 60;

	  if(this->m_choosen.a >= 0 && this->m_effect[a][b] == LIGHT && (this->m_choosen.a != a || this->m_choosen.b != b))
	    {
	      Move move;
	      move.from = m_choosen.a * 4 + m_choosen.b;
	      move.to = a * 4 + b;

	      m_choosen.a = -1;
	      m_choosen.b = -1;

	      lock();
	      m_currentPlayer->commitMoveProposal(move);
	    }
	  else if(this->m_choosen.a >= 0 && this->m_effect[a][b] == LIGHT)
	    {
	      for(int i = 0; i < 4; i++)
	      	for(int j = 0; j < 4; j++)
		  if(m_effect[i][j] == LIGHT)
		    m_effect[i][j] = NONE;

	      this->m_choosen.a = -1;
	      this->m_choosen.b = -1;
	    }
	  else if(m_choosen.a < 0 && m_board.m_fields[a][b] != FIELD_EMPTY && m_effect[a][b] != GRAY)
	    {
	      MoveSet& moves = m_currentPlayer->getAvailableMoves();
	      MoveSet::Iterator it = moves.begin();
	      for (; it != moves.end(); it++)
		{
		  Move& move = it.at();
		  if(move.from / 4 == a && move.from % 4 == b)
		    m_effect[move.to/4][move.to%4] = LIGHT;
		}
	      m_effect[a][b] = LIGHT;

	      m_choosen.a = a;
	      m_choosen.b = b;
	    }
	}
    }

  queue_draw();

  return true;
}

void GraphicalBoard::onGameNew(const Game& game)
{
  m_currentPlayer = NULL;
  m_players = game.getPlayers();
  m_board = game.getCurrentState().m_board;
  m_choosen.a = -1;
  m_choosen.b = -1;
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
  m_offset.x = (get_width() - 300) / 2;
  m_offset.y = (get_height() - 300) / 2;

  cr->set_source(m_backgroundPattern);
  cr->rectangle(0, 0, get_width(), get_height());
  cr->paint();
  cr->set_source(m_boardImg, m_offset.x, m_offset.y);
  cr->paint();

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      {
	FieldState fs;
	PlayerColor color;
	
	fs = m_board.m_fields[i][j];
	if (fs != FIELD_EMPTY)
	  {
	    color = m_players[fs]->m_color;
	    cr->set_source(m_stones[color], m_offset.x+35+j*60, m_offset.y+35+i*60);
	    cr->paint();
	  }

  	switch(this->m_effect[i][j])
  	  {
  	  case LIGHT:
	    cr->set_source(m_hilightImg, m_offset.x+35+j*60, m_offset.y+35+i*60);
	    cr->paint();
	    break;
  	  case GRAY:
	    cr->set_source(m_lolightImg, m_offset.x+35+j*60, m_offset.y+35+i*60);
	    cr->paint();
	  case NONE:
	    ;
  	  }
      }

  return true;
}

void GraphicalBoard::lock()
{
  m_locked = true;

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      this->m_effect[i][j] = GRAY;  
}

void GraphicalBoard::unlock()
{
  m_locked = false;

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      this->m_effect[i][j] = NONE;
}
