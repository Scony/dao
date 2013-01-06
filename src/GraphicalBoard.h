#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <gtkmm/drawingarea.h>
#include <gdk/gdk.h>

#include "Board.h"

class GraphicalBoard : public Gtk::DrawingArea
{
 public:
  GraphicalBoard();
  ~GraphicalBoard();

  /*Slots*/
public:
  bool onButtonPress(GdkEventButton* event);
  
  /*Events*/
protected:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  
private:
  Board m_board;
  struct { int a, b; } choosen;
  enum { NONE, GRAY, LIGHT } effect[4][4];
  
  Cairo::RefPtr<Cairo::ImageSurface> m_boardImg;
  Cairo::RefPtr<Cairo::ImageSurface> m_p1Img;
  Cairo::RefPtr<Cairo::ImageSurface> m_p2Img;
  Cairo::RefPtr<Cairo::ImageSurface> m_hilightImg;
  Cairo::RefPtr<Cairo::ImageSurface> m_lolightImg;

  void handleClick(int x, int y);
};

#endif
