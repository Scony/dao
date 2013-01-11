#include <iostream>

#include <gtkmm/messagedialog.h>
#include <gtkmm/main.h>
#include <glibmm/fileutils.h>

#include "Application.h"
#include "Configuration.h"
#include "DaoException.h"
#include "defines.h"

using namespace std;

int main(int argc, char ** argv)
{
  Gtk::Main app(argc, argv);

  try
    {
      Configuration::getInstance().readFile(CONFIG_FILE);
    }
  catch(Glib::FileError e)
    {
      Gtk::MessageDialog dlg("Blad podczas wczytywania"
			     " konfiguracji"
			     ". Prosze sprawdzic, czy w katalogu"
			     " biezacym znajduje sie podkatalog "
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

  app.run(*mainWindow);
  return 0;
}
