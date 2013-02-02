#ifndef CONFIGURATION_UI_H
#define CONFIGURATION_UI_H

#include <gtkmm/dialog.h>

class ConfigurationUI : public Gtk::Dialog
{
public:
  ConfigurationUI(Gtk::Window& parent);
  ~ConfigurationUI();
};


#endif
