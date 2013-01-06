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
  //set_resizable(false);

  m_configuration_buffer = Gtk::TextBuffer::create();

  Gtk::VBox* layout = manage(new Gtk::VBox);

  Gtk::Button* button = manage(new Gtk::Button("Some"));
  layout->add(*button);

  m_gBoard = new GraphicalBoard;
  layout->add(*m_gBoard);

  Gtk::ScrolledWindow* scrolled_window =
    manage(new Gtk::ScrolledWindow);
  scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC,
			     Gtk::POLICY_AUTOMATIC);
  scrolled_window->set_size_request(300, 50);

  Gtk::TextView* configuration_text = manage(new Gtk::TextView);
  configuration_text->set_buffer(m_configuration_buffer);
  //TODO: Zbadać, dlaczego tu powstaje Gtk-Critical x2
  scrolled_window->add(*configuration_text);

  layout->add(*scrolled_window);
    
  Gtk::Button* commit_button = manage(new Gtk::Button("Ci"));
  commit_button->signal_clicked().connect( sigc::mem_fun(*this, &Application::onCommitClicked));
  layout->add(*commit_button);

  Gtk::Statusbar* statusbar = manage(new Gtk::Statusbar);
  statusbar->push("values ...");
  layout->add(*statusbar);

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
