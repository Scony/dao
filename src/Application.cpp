#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <gtkmm/hvbox.h>
#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/scrolledwindow.h>

#include "defines.h"
#include "Application.h"
#include "Configuration.h"
#include "DaoException.h"
#include "Game.h"
#include "Player.h"

using namespace std;

Application::Application()
{
  this->initUI();

  Configuration& config = Configuration::getInstance();  
  string configuration_text = config.getData();
  m_configuration_buffer->set_text(configuration_text);

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
  set_position(Gtk::WIN_POS_CENTER_ALWAYS);
  //set_resizable(false);

  m_configuration_buffer = Gtk::TextBuffer::create();

  Gtk::VBox* layout = manage(new Gtk::VBox);

  Gtk::Button* button = manage(new Gtk::Button("Nowa gra"));
  button->signal_clicked().connect( sigc::mem_fun(*this, &Application::onMenuGameNewSelected));
  layout->add(*button);

  m_gBoard = new GraphicalBoard;
  layout->add(*m_gBoard);

  Gtk::ScrolledWindow* scrolled_window =
    manage(new Gtk::ScrolledWindow);
  scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC,
			     Gtk::POLICY_AUTOMATIC);
  scrolled_window->set_size_request(300, 200);

  Gtk::TextView* configuration_text = manage(new Gtk::TextView);
  configuration_text->set_buffer(m_configuration_buffer);
  //TODO: Zbadać, dlaczego tu powstaje Gtk-Critical x2
  scrolled_window->add(*configuration_text);

  layout->add(*scrolled_window);
    
  Gtk::Button* commit_button = manage(new Gtk::Button("Ci"));
  commit_button->signal_clicked().connect( sigc::mem_fun(*this, &Application::onCommitClicked));
  layout->add(*commit_button);

  m_statusbar = manage(new Gtk::Statusbar);
  m_statusbar->push("values ...");
  layout->add(*m_statusbar);

  add(*layout);


  Configuration& config = Configuration::getInstance();
  config.signal_changed.connect( sigc::mem_fun(*this, &Application::onConfigurationChanged));

}  

void Application::onCommitClicked()
{
  string configuration_text = m_configuration_buffer->get_text();
  try
    {
      Configuration::getInstance().readString(configuration_text);
    }
  catch(DaoException e)
    {
      Gtk::MessageDialog dlg(*this, "Błąd podczas wczytywania pliku konfiguracyjnego");
      dlg.set_secondary_text(e.what());
      dlg.run();
    }
}

void Application::onConfigurationChanged()
{
  string configuration_text =
    Configuration::getInstance().getData();
  m_configuration_buffer->set_text(configuration_text);
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
      if (p.m_color == 0)
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
