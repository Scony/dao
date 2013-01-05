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
  else
    throw DaoException("Unknown algorithm");
}

void PlayerConfiguration::readKeyFile(const Glib::KeyFile& key)
  throw(DaoException, Glib::KeyFileError)
{
  string type, algorithm;
  
  type = key.get_string(m_sectionName, "type");
  m_type = stringToPlayerType(type);

  if (m_type == PLAYER_COMPUTER)
    {
      algorithm = key.get_string(m_sectionName, "algorithm");
      m_algorithm = stringToPlayerAlgorithm(algorithm);
    }
}

Configuration::Configuration()
{
  m_firstPlayer = 0;
  m_players[0].m_color = 0;
  m_players[1].m_color = 1;
  m_players[0].m_type = PLAYER_HUMAN;
  m_players[1].m_type = PLAYER_HUMAN;
  m_players[0].m_sectionName = "Player1";
  m_players[1].m_sectionName = "Player2";
}

Configuration& Configuration::getInstance() throw()
{
  static Configuration configuration;
  return configuration;
}

void Configuration::parseKeyFile() throw(Glib::KeyFileError)
{
  string first_player = m_keyFile.get_string("Game", "first_move");
  if (first_player == "Player2")
    m_firstPlayer = 1;
  else
    m_firstPlayer = 0;

  for (int i = 0; i < 2; i++)
    {
      m_players[i].readKeyFile(m_keyFile);
    }
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

void Configuration::readFile(const string& filename) throw(DaoException)
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
  
