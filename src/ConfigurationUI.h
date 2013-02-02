#ifndef CONFIGURATION_UI_H
#define CONFIGURATION_UI_H

#include <gtkmm/dialog.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/comboboxtext.h>
#include "Configuration.h"

class GameConfigurationUI : public Gtk::Grid
{
public:
  GameConfigurationUI();

  void readValues(Configuration& config);
  void saveValues(Configuration& config);

private:
  Gtk::SpinButton* m_sbTimeout;
  Gtk::ComboBoxText* m_cbFirstPlayer;
};

class ConfigurationUI : public Gtk::Dialog
{
public:
  ConfigurationUI(Gtk::Window& parent);
  ~ConfigurationUI();

  int run();

protected:
  GameConfigurationUI* m_gameConfigurationUI;
  //TODO: Add player configuration UI
  void initUI();
  void onClose(int button_id);
};


#endif
