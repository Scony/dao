#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <gtkmm/drawingarea.h>
#include <gdk/gdk.h>

#include "Board.h"
#include "State.h"
#include "Player.h"

class Human;

class GraphicalBoard : public Gtk::DrawingArea
{
  Board m_board;
  Human* m_currentPlayer;
  const pPlayer* m_players;
  struct { int a, b; } m_choosen;
  enum { NONE, GRAY, LIGHT } m_effect[4][4];
  bool m_locked;

  struct { int x, y; } m_offset;
  
  Cairo::RefPtr<Cairo::ImageSurface> m_boardImg;
  Cairo::RefPtr<Cairo::ImageSurface> m_stones[COLOR_NUM];
  Cairo::RefPtr<Cairo::ImageSurface> m_hilightImg;
  Cairo::RefPtr<Cairo::ImageSurface> m_lolightImg;
  Cairo::RefPtr<Cairo::ImageSurface> m_backgroundImg;
  Cairo::RefPtr<Cairo::SurfacePattern> m_backgroundPattern;

  void lock();
  void unlock();

 protected:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

 public:
  GraphicalBoard();
  ~GraphicalBoard();

  /** called by Human **/
  void proposeMove(Human* player);

  /** signal handlers **/
  bool onButtonPress(GdkEventButton* event);
  void onGameNew(const Game& g);
  void onGameStateChanged(State s, const Player& p);
};

#endif
