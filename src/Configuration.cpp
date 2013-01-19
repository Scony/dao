#include <string>

#include "Configuration.h"
#include "DaoException.h"

using namespace std;

PlayerType PlayerConfiguration::stringToPlayerType(const std::string& type) const throw(DaoException)
{
  if (type == "human")
    return PLAYER_HUMAN;
  else if (type == "computer")
    return PLAYER_COMPUTER;
  else
    throw DaoException("Unknown player type");
}

PlayerAlgorithm PlayerConfiguration::stringToPlayerAlgorithm(const std::string& a) const throw(DaoException)
{
  if (a == "random")
    return ALGORITHM_RANDOM;
  else if (a == "hill climber")
    return ALGORITHM_HILL_CLIMBER;
  else if (a == "alpha beta")
    return ALGORITHM_ALPHA_BETA;
  else if (a == "alpha beta tt")
    return ALGORITHM_ALPHA_BETA_TT;
  else
    throw DaoException("Unknown algorithm");
}

PlayerColor PlayerConfiguration::stringToPlayerColor(const std::string& c) const throw(DaoException)
{
  if (c == "red")
    return COLOR_RED;
  else if (c == "blue")
    return COLOR_BLUE;
  else
    throw DaoException("Unknown color name");
}

void PlayerConfiguration::readKeyFile(const Glib::KeyFile& key)
  throw(DaoException, Glib::KeyFileError)
{
  string type, algorithm, color;
  type = key.get_string(m_sectionName, "type");
  m_type = stringToPlayerType(type);
  
  color = key.get_string(m_sectionName, "color");
  m_color = stringToPlayerColor(color);

  if (m_type == PLAYER_COMPUTER)
    {
      algorithm = key.get_string(m_sectionName, "algorithm");
      m_algorithm = stringToPlayerAlgorithm(algorithm);
    }

  m_k[0] = key.get_double(m_sectionName, "k1");
  m_k[1] = key.get_double(m_sectionName, "k2");
  m_k[2] = key.get_double(m_sectionName, "k3");
  m_h = key.get_double(m_sectionName, "h");
}

Configuration::Configuration()
{
  m_firstPlayer = FIELD_PLAYER1;
  m_players[0].m_color = COLOR_RED;
  m_players[1].m_color = COLOR_BLUE;
  m_players[0].m_type = PLAYER_HUMAN;
  m_players[1].m_type = PLAYER_HUMAN;
  m_players[0].m_sectionName = "Player1";
  m_players[1].m_sectionName = "Player2";
  m_players[0].m_name = "Gracz 1";
  m_players[1].m_name = "Gracz 2";
  m_latency = 1;
}

Configuration& Configuration::getInstance() throw()
{
  static Configuration configuration;
  return configuration;
}

void Configuration::parseKeyFile()
  throw(DaoException, Glib::KeyFileError)
{
  string first_player = m_keyFile.get_string("Game", "first_move");
  if (first_player == "Player2")
    m_firstPlayer = FIELD_PLAYER2;
  else
    m_firstPlayer = FIELD_PLAYER1;

  if (m_keyFile.has_key("Game", "latency"))
    {
      m_latency = m_keyFile.get_integer("Game", "latency");
    }
  for (int i = 0; i < 2; i++)
    {
      m_players[i].readKeyFile(m_keyFile);
    }

  signal_changed.emit();
}

void Configuration::readString(const string& data)
  throw(DaoException)
{
  try
    {
      m_keyFile.load_from_data(data, Glib::KEY_FILE_KEEP_COMMENTS);
      parseKeyFile();
    }
  catch (Glib::KeyFileError e)
    {
      throw DaoException(e.what());
    }
}

void Configuration::readFile(const string& filename) throw(DaoException, Glib::FileError)
{
  try 
    {
      m_keyFile.load_from_file(filename,
			       Glib::KEY_FILE_KEEP_COMMENTS);
      parseKeyFile();
    }
  catch (Glib::KeyFileError e)
    {
      throw DaoException(e.what());
    }
}

string Configuration::getData()
{
  return m_keyFile.to_data();
}
  
