#include <iostream>

#include <gtkmm/messagedialog.h>
#include <glibmm/fileutils.h>

#include "Application.h"
#include "Configuration.h"
#include "DaoException.h"
#include "defines.h"

using namespace std;

int main(int argc, char ** argv)
{
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv);

  try
    {
      Configuration::getInstance().readFile(CONFIG_FILE);
    }
  catch(Glib::FileError e)
    {
      Gtk::MessageDialog dlg("Błąd podczas wczytywania"
			     " konfiguracji"
			     ". Proszę sprawdzić, czy w katalogu"
			     " bieżącym znajduje się podkatalog "
			     "data", false,
			     Gtk::MESSAGE_ERROR);
      dlg.set_secondary_text(e.what());
      dlg.run();
      return 1;
    }
  catch(DaoException e)
    {
      Gtk::MessageDialog dlg("Niepoprawny format pliku konfigura"
			     "cyjnego", false,
			     Gtk::MESSAGE_ERROR);
      dlg.set_secondary_text(e.what());
      dlg.run();
      return 1;
    }

  Application* mainWindow = new Application;
  mainWindow->show_all();

  app->run(*mainWindow);
  return 0;
}
