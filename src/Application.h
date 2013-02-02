#ifndef APPLICATION_H
#define APPLICATION_H

#include <glibmm/refptr.h>
#include <gtkmm/window.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/stock.h>
#include <gtkmm/uimanager.h>

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
  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

  void initUI();

private: /* SLOTS */
  void onCommitClicked();
  void onConfigurationChanged();
  void onMenuGameNewSelected();
  void onGameNew(const Game& game);
  void onGameEnd(const Player& winner);
  void onGameStateChanged(State s, const Player& p);
};
#endif
