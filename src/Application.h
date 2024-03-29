#ifndef APPLICATION_H
#define APPLICATION_H

#include <glibmm/refptr.h>
#include <gtkmm/window.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/main.h>


#include "ConfigurationUI.h"
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
  ConfigurationUI* m_configurationUI;
  Gtk::Statusbar* m_statusbar;
  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

  void initUI();

private: /* SLOTS */
  void onMenuGameNewSelected();
  void onMenuSettingsPreferencesSelected();
  void onMenuHelpAboutSelected();
  void onMenuGameQuitSelected();
  void onMenuHelpRulesSelected();
  void onMenuHelpGUISelected();
  void onMenuGameSaveSelected();
  void onMenuGameLoadSelected();
  void onGameNew(const Game& game);
  void onGameEnd(const Player& winner);
  void onGameStateChanged(State s, const Player& p);
};
#endif
