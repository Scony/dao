#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <gtkmm/hvbox.h>
#include <gtkmm/button.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/filechooserdialog.h>

#include "defines.h"
#include "Application.h"
#include "DaoException.h"
#include "Game.h"
#include "Player.h"

using namespace std;

Application::Application()
{
  this->initUI();

  m_configurationUI = new ConfigurationUI(*this);

  m_game = new Game;
  m_game->signal_new_game.connect( sigc::mem_fun(*this, &Application::onGameNew));
  m_game->signal_new_game.connect( sigc::mem_fun(*m_gBoard, &GraphicalBoard::onGameNew));
  m_game->signal_state_changed.connect( sigc::mem_fun(*this, &Application::onGameStateChanged));
  m_game->signal_state_changed.connect( sigc::mem_fun(*m_gBoard, &GraphicalBoard::onGameStateChanged));
  m_game->signal_game_end.connect( sigc::mem_fun(*this, &Application::onGameEnd));
  PlayerFactory::setGBoard(m_gBoard);

  m_game->newGame();
}

Application::~Application()
{
  delete m_gBoard;
}

void Application::initUI()
{
  m_gBoard = 0;
  set_title("Dao");
  // set_position(Gtk::WIN_POS_CENTER_ALWAYS);
  // set_resizable(false);
  
  Gtk::VBox* layout = manage(new Gtk::VBox);

  m_refActionGroup = Gtk::ActionGroup::create();

  //Game menu:
  m_refActionGroup->add(Gtk::Action::create("GameMenu", "Gra"));
  m_refActionGroup->add(Gtk::Action::create("GameNew", Gtk::Stock::NEW, "Nowa"),
			sigc::mem_fun(*this, &Application::onMenuGameNewSelected));
  m_refActionGroup->add(Gtk::Action::create("GameLoad", Gtk::Stock::OPEN, "Wczytaj"),
  			sigc::mem_fun(*this, &Application::onMenuGameNewSelected));
  m_refActionGroup->add(Gtk::Action::create("GameSave", Gtk::Stock::SAVE, "Zapisz"),
  			sigc::mem_fun(*this, &Application::onMenuSaveSelected));
  m_refActionGroup->add(Gtk::Action::create("GameQuit", Gtk::Stock::QUIT, "Zakończ"),
			sigc::mem_fun(*this, &Application::onMenuGameQuitSelected));

  //Settings menu:
  m_refActionGroup->add(Gtk::Action::create("SettingsMenu", "Ustawienia"));
  m_refActionGroup->add(Gtk::Action::create("SettingsPreferences", "Preferencje"),
			Gtk::AccelKey("<control>P"),
			sigc::mem_fun(*this, &Application::onMenuSettingsPreferencesSelected));

  //Help menu:
  m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Pomoc") );
  m_refActionGroup->add( Gtk::Action::create("HelpAbout", "O programie"),
			 sigc::mem_fun(*this, &Application::onMenuHelpAboutSelected));
  m_refActionGroup->add( Gtk::Action::create("HelpRules", "Zasady gry"),
			 sigc::mem_fun(*this, &Application::onMenuHelpRulesSelected));
  m_refActionGroup->add( Gtk::Action::create("HelpGUI", "Interfejs"),
			 sigc::mem_fun(*this, &Application::onMenuHelpGUISelected));

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(m_refActionGroup);

  add_accel_group(m_refUIManager->get_accel_group());

  try
  {
    m_refUIManager->add_ui_from_file("./data/menu.xml");
  }
  catch(const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " <<  ex.what();
  }

  Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
  if(pMenubar)
    layout->pack_start(*pMenubar, false, true);

  m_gBoard = new GraphicalBoard;
  layout->add(*m_gBoard);

  m_statusbar = manage(new Gtk::Statusbar);
  m_statusbar->push("values ...");
  layout->pack_end(*m_statusbar, false, true);

  add(*layout);
}  

void Application::onMenuGameNewSelected()
{
  cout << "Menu selected" << endl;
  try
    {
      m_game->newGame();
    }
  catch (DaoException e)
    {
      Gtk::MessageDialog dlg(*this, "Błąd podczas uruchamiania gry");
      dlg.set_secondary_text(e.what());
      dlg.run();
    }
}

void Application::onMenuSettingsPreferencesSelected()
{
  m_configurationUI->readValues();
  m_configurationUI->run();
  if (!m_game->gameStarted())
    {
      m_game->newGame();
    }
}

void Application::onMenuHelpAboutSelected()
{
  Gtk::AboutDialog ab;
  ab.set_program_name("Dao");
  ab.set_version("Wersja 1.0");
  ab.set_copyright("Copyright (c) 2013");
  ab.set_comments("Gra Dao - projekt zaliczeniowy w ramach laboratorium\nsztucznej inteligencji na Politechnice Poznańskiej");
  ab.set_license("GNU GPL v3");
  vector<Glib::ustring> authors;
  authors.push_back("Tomasz Boczkowski");
  authors.push_back("Paweł Lampe");
  ab.set_authors(authors);
  ab.run();
}

void Application::onMenuGameQuitSelected()
{
  Gtk::Main::quit();
}

void Application::onMenuHelpRulesSelected()
{
  system("xdg-open ./data/index.html");
}

void Application::onMenuHelpGUISelected()
{
  system("xdg-open ./data/gui.html");
}

void Application::onGameNew(const Game& game)
{
  ostringstream message;
  const Player* p = game.getCurrentPlayer();

  message << "Nowa gra. Rozpoczyna " << p->m_name;
  if (p->m_color == 0)
    message << " koloru czerwonego. ";
  else 
    message << " koloru niebieskiego. ";
  
  if (p->isInteractive())
    message << " Kliknij!";
  else
    message << " Czekaj...";

  m_statusbar->push(message.str());
}

void Application::onGameEnd(const Player& winner)
{
  ostringstream message;

  message << "Koniec! Wygral: ";
  message << winner.m_name;
  message << " (";
  if (winner.m_color == COLOR_RED) 
    message << "czerwony";
  else
    message << "niebieski";
  message << ")";

  m_statusbar->push(message.str());

  Gtk::MessageDialog dlg(*this, "Koniec gry");
  //dlg.set_secondary_text(e.what());
  dlg.run();
}

void Application::onGameStateChanged(State s, const Player& p)
{
  ostringstream message;

  if (!s.m_board.isTerminal() )
    {
      message << "Ruch ma " << p.m_name;
      if (p.m_color == COLOR_RED)
	message << " koloru czerwonego. ";
      else 
	message << " koloru niebieskiego. ";
      
      if (p.isInteractive())
	message << " Kliknij!";
      else
	message << " Czekaj...";
      
      m_statusbar->push(message.str());
    }
}

void Application::onMenuSaveSelected()
{
  Gtk::FileChooserDialog dialog("Zapisz stan gry", 
			     Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  dialog.set_current_name("gra.txt");
  dialog.set_current_folder("saved");
  dialog.set_transient_for(*this);
  int result = dialog.run();
  
  if (result == Gtk::RESPONSE_OK)
    {
      ostringstream message;

      string filename = dialog.get_filename();
      ofstream fout(filename.c_str());
      m_game->write(fout);
      fout.close();

      message << "Zapisano do pliku: " << filename;
      m_statusbar->push(message.str());
    }
}
