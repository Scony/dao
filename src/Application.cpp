#include <string.h>
#include <string>
#include <gtkmm/hvbox.h>
#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/messagedialog.h>

#include "defines.h"
#include "Application.h"
#include "Configuration.h"
#include "DaoException.h"

using namespace std;

Application::Application()
{
  this->initUI();

  Configuration& config = Configuration::getInstance();  
  string configuration_text = config.getData();
  m_configuration_buffer->set_text(configuration_text);
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
  set_resizable(false);

  Gtk::VBox* layout = manage(new Gtk::VBox);
  add(*layout);

  Gtk::Button* button = manage(new Gtk::Button("Some"));
  layout->add(*button);

  m_gBoard = new GraphicalBoard;
  layout->add(*Glib::wrap(m_gBoard->getDrawingArea()));

  Gtk::TextView* configuration_text = manage(new Gtk::TextView);
  configuration_text->set_size_request(300, 50);
  layout->add(*configuration_text);

  Gtk::Button* commit_button = manage(new Gtk::Button("Ci"));
  commit_button->signal_clicked().connect( sigc::mem_fun(*this, &Application::onCommitClicked));
  layout->add(*commit_button);

  Gtk::Statusbar* statusbar = manage(new Gtk::Statusbar);
  statusbar->push("values ...");
  layout->add(*statusbar);

  Configuration& config = Configuration::getInstance();
  config.signal_changed.connect( sigc::mem_fun(*this, &Application::onConfigurationChanged));

  m_configuration_buffer = configuration_text->get_buffer();
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
