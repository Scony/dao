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
  m_gameConfigurationUI = manage(new GameConfigurationUI);
  m_playerConfigurationUI[0] = manage(new PlayerConfigurationUI);
  m_playerConfigurationUI[1] = manage(new PlayerConfigurationUI);
  Gtk::Notebook* notebook = manage(new Gtk::Notebook());

  notebook->append_page(*m_gameConfigurationUI, "Gra");
  notebook->append_page(*(m_playerConfigurationUI[0]), "Gracz 1");
  notebook->append_page(*(m_playerConfigurationUI[1]), "Gracz 2");

  get_content_area()->pack_start(*notebook);
  get_content_area()->show_all();
}

ConfigurationUI::~ConfigurationUI()
{
}

void ConfigurationUI::readValues()
{
  Configuration& config = Configuration::getInstance();
  m_gameConfigurationUI->readValues(config);
  for(int i = 0; i < Game::NUM_PLAYERS; i++)
    m_playerConfigurationUI[i]->readValues(config.m_players[i]);
}  

void ConfigurationUI::onClose(int button_id)
{
  Configuration& config = Configuration::getInstance();  
  m_gameConfigurationUI->saveValues(config);
  for(int i = 0; i < Game::NUM_PLAYERS; i++)
    m_playerConfigurationUI[i]->saveValues(config.m_players[i]);  
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

PlayerConfigurationUI::PlayerConfigurationUI() 
{
  set_border_width(12);
  set_column_spacing(5);
  set_row_spacing(5);
  set_hexpand(true);

  /* Kolor */
  Gtk::Label* lblColor =
    manage(new Gtk::Label("Kolor:"));
  lblColor->set_halign(Gtk::ALIGN_START);
  
  m_cbColor = manage(new Gtk::ComboBoxText);
  m_cbColor->append("Czerwony");
  m_cbColor->append("Niebieski");

  attach(*lblColor, 0, 0, 1, 1);
  attach(*m_cbColor, 1, 0, 1, 1);

  /* Typ */
  Gtk::Label* lblType =
    manage(new Gtk::Label("Typ:"));
  lblType->set_halign(Gtk::ALIGN_START);
  
  m_cbType = manage(new Gtk::ComboBoxText);
  m_cbType->append("Człowiek");
  m_cbType->append("Komputer");

  attach(*lblType, 0, 1, 1, 1);
  attach(*m_cbType, 1, 1, 1, 1);

  /* Algorytm */
  Gtk::Label* lblAlgorithm =
    manage(new Gtk::Label("Algorytm:"));
  lblAlgorithm->set_halign(Gtk::ALIGN_START);
  
  m_cbAlgorithm = manage(new Gtk::ComboBoxText);
  m_cbAlgorithm->append("Losowy");
  m_cbAlgorithm->append("Hill climber");
  m_cbAlgorithm->append("Alfa beta");
  m_cbAlgorithm->append("Alfa beta + TT");

  attach(*lblAlgorithm, 0, 2, 1, 1);
  attach(*m_cbAlgorithm, 1, 2, 1, 1);

  /* Glebokosc */
  Gtk::Label* lblDepth = 
    manage(new Gtk::Label("Głębokość przeszukiwania:"));
  lblDepth->set_halign(Gtk::ALIGN_START);  

  m_sbDepth = manage(new Gtk::SpinButton);
  m_sbDepth->set_range(1, 7);
  m_sbDepth->set_increments(1, 1);
  
  attach(*lblDepth, 0, 3, 1, 1);
  attach(*m_sbDepth, 1, 3, 1, 1);

  /* K1 */
  Gtk::Label* lblK = 
    manage(new Gtk::Label("K1:"));
  lblK->set_halign(Gtk::ALIGN_START);  

  m_sbK1 = manage(new Gtk::SpinButton);
  m_sbK1->set_range(0, 1000);
  m_sbK1->set_increments(10, 100);
  
  attach(*lblK, 0, 4, 1, 1);
  attach(*m_sbK1, 1, 4, 1, 1);
  
  /* K2 */
  lblK = 
    manage(new Gtk::Label("K2:"));
  lblK->set_halign(Gtk::ALIGN_START);    

  m_sbK2 = manage(new Gtk::SpinButton);
  m_sbK2->set_range(0, 1000);
  m_sbK2->set_increments(10, 100);
  
  attach(*lblK, 0, 5, 1, 1);
  attach(*m_sbK2, 1, 5, 1, 1);

  /* K3 */
  lblK = 
    manage(new Gtk::Label("K3:"));
  lblK->set_halign(Gtk::ALIGN_START);    

  m_sbK3 = manage(new Gtk::SpinButton);
  m_sbK3->set_range(0, 1000);
  m_sbK3->set_increments(10, 100);
  
  attach(*lblK, 0, 6, 1, 1);
  attach(*m_sbK3, 1, 6, 1, 1);

  /* H */
  lblK = 
    manage(new Gtk::Label("H:"));
  lblK->set_halign(Gtk::ALIGN_START);    

  m_sbH = manage(new Gtk::SpinButton);
  m_sbH->set_digits(2);
  m_sbH->set_range(0, 1);
  m_sbH->set_increments(0.01, 0.1);
  
  attach(*lblK, 0, 7, 1, 1);
  attach(*m_sbH, 1, 7, 1, 1);
}

void PlayerConfigurationUI::readValues(PlayerConfiguration& config)
{
  m_cbColor->set_active(static_cast<int>(config.m_color));
  m_cbType->set_active(static_cast<int>(config.m_type));
  m_cbAlgorithm->set_active(static_cast<int>(config.m_algorithm));
  m_sbDepth->set_value(config.m_depth);
  m_sbK1->set_value(config.m_k[0]);
  m_sbK2->set_value(config.m_k[1]);
  m_sbK3->set_value(config.m_k[2]);
  m_sbH->set_value(config.m_h);
}

void PlayerConfigurationUI::saveValues(PlayerConfiguration& config)
{
  config.m_color =
    static_cast<PlayerColor>(m_cbColor->get_active_row_number());
  config.m_type =
    static_cast<PlayerType>(m_cbType->get_active_row_number());
  config.m_algorithm =
    static_cast<PlayerAlgorithm>(m_cbAlgorithm->get_active_row_number());
  config.m_depth = m_sbDepth->get_value();
  config.m_k[0] = m_sbK1->get_value();
  config.m_k[1] = m_sbK2->get_value();
  config.m_k[2] = m_sbK3->get_value();
  config.m_h = m_sbH->get_value();
}
