#ifndef CONFIGURATION_UI_H
#define CONFIGURATION_UI_H

#include <gtkmm/dialog.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/comboboxtext.h>
#include "Configuration.h"
#include "Game.h"

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


class PlayerConfigurationUI : public Gtk::Grid
{
public:
  PlayerConfigurationUI();

  void readValues(PlayerConfiguration& config);
  void saveValues(PlayerConfiguration& config);

private:
  Gtk::ComboBoxText* m_cbColor;
  Gtk::ComboBoxText* m_cbType;
  Gtk::ComboBoxText* m_cbAlgorithm;
  Gtk::SpinButton* m_sbDepth;
  Gtk::SpinButton* m_sbK1;
  Gtk::SpinButton* m_sbK2;
  Gtk::SpinButton* m_sbK3;
  Gtk::SpinButton* m_sbH;
};


class ConfigurationUI : public Gtk::Dialog
{
public:
  ConfigurationUI(Gtk::Window& parent);
  ~ConfigurationUI();

  void readValues();

protected:
  GameConfigurationUI* m_gameConfigurationUI;
  PlayerConfigurationUI* m_playerConfigurationUI[Game::NUM_PLAYERS];
  //TODO: Add player configuration UI
  void initUI();
  void onClose(int button_id);
};


#endif
