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
  struct { int a, b; } choosen;
  enum { NONE, GRAY, LIGHT } effect[4][4];
  bool locked;
  
  Cairo::RefPtr<Cairo::ImageSurface> m_boardImg;
  Cairo::RefPtr<Cairo::ImageSurface> m_p1Img;
  Cairo::RefPtr<Cairo::ImageSurface> m_p2Img;
  Cairo::RefPtr<Cairo::ImageSurface> m_hilightImg;
  Cairo::RefPtr<Cairo::ImageSurface> m_lolightImg;

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
  void onGameNew(State s, const Player& p);
  void onGameStateChanged(State s, const Player& p);
};

#endif
