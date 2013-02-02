#include <iostream>
#include <gtkmm/notebook.h>
#include <gtkmm/button.h>
#include "ConfigurationUI.h"
#include "Configuration.h"

using namespace std;

ConfigurationUI::ConfigurationUI(Gtk::Window& parent)
  : Gtk::Dialog("Konfiguracja DAO", parent, true)
{
  add_button("Zamknij", 1);
  signal_response().connect( sigc::mem_fun( *this, 
  					    &ConfigurationUI::onClose));
  initUI();
}

void ConfigurationUI::initUI() {
  Gtk::Notebook* notebook = manage(new Gtk::Notebook());
  m_gameConfigurationUI = manage(new GameConfigurationUI);
  notebook->append_page(*m_gameConfigurationUI, "Gra");

  get_content_area()->pack_start(*notebook);
  get_content_area()->show_all();
}

ConfigurationUI::~ConfigurationUI()
{
}

int ConfigurationUI::run()
{
  Configuration& config = Configuration::getInstance();
  m_gameConfigurationUI->readValues(config);
  //TODO: player conf read
  return Gtk::Dialog::run();
}

void ConfigurationUI::onClose(int button_id)
{
  //TODO: Save configuration
  hide();
}


GameConfigurationUI::GameConfigurationUI() 
{
  set_border_width(12);
  set_column_spacing(5);
  set_row_spacing(5);
  set_hexpand(true);
  Gtk::Label* lblTimeout =
    manage(new Gtk::Label("Opóźnienie ruchu [ms]:"));
  lblTimeout->set_halign(Gtk::ALIGN_START);
  
  m_sbTimeout = manage(new Gtk::SpinButton);
  m_sbTimeout->set_range(0, 10000);
  m_sbTimeout->set_increments(100, 100);
  
  add(*lblTimeout);
  attach_next_to(*m_sbTimeout, *lblTimeout, Gtk::POS_RIGHT, 1, 1);

  Gtk::Label* lblFirstPlayer = 
    manage(new Gtk::Label("Pierwszy gracz:"));
  lblFirstPlayer->set_halign(Gtk::ALIGN_START);
  m_cbFirstPlayer = manage(new Gtk::ComboBoxText);
  m_cbFirstPlayer->append("Gracz 1");
  m_cbFirstPlayer->append("Gracz 2");
  m_cbFirstPlayer->set_active(0);

  attach(*lblFirstPlayer, 0, 1, 1, 1);
  attach_next_to(*m_cbFirstPlayer, *lblFirstPlayer, Gtk::POS_RIGHT,
		 1, 1);
}

void GameConfigurationUI::readValues(Configuration& config)
{
  m_cbFirstPlayer->set_active(static_cast<int>(config.m_firstPlayer));
  m_sbTimeout->set_value(config.m_latency/1000);
}

void GameConfigurationUI::saveValues(Configuration& config)
{
  config.m_latency = m_sbTimeout->get_value() * 1000;
  config.m_firstPlayer =
    static_cast<FieldState>(m_cbFirstPlayer->get_active_row_number());
}
