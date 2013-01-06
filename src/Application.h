#ifndef APPLICATION_H
#define APPLICATION_H

#include <glibmm/refptr.h>
#include <gtkmm/window.h>
#include <gtkmm/statusbar.h>

#include "Configuration.h"
#include "GraphicalBoard.h"
#include "State.h"
#include "Player.h"

class Application : public Gtk::Window
{
public:
  Application();
  ~Application();

private:
  GraphicalBoard * m_gBoard;
  Game* m_game;
  Gtk::Statusbar* m_statusbar;
  Glib::RefPtr<Gtk::TextBuffer> m_configuration_buffer;

  void initUI();

private: /* SLOTS */
  void onCommitClicked();
  void onConfigurationChanged();
  void onMenuGameNewSelected();
  void onGameNew(State s, const Player& p);
  void onGameStateChanged(State s, const Player& p);
};
#endif
