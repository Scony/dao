#include <string>

#include "Configuration.h"
#include "DaoException.h"

using namespace std;

int PlayerConfiguration::setType(const std::string& type)
{
  if (type == "human")
    m_type = PLAYER_HUMAN;
  else if (type == "computer")
    m_type = PLAYER_COMPUTER;
  else
    return -1;
  return 0;
}

int PlayerConfiguration::setAlgorithm(const std::string& a)
{
  if (a == "random")
    m_algorithm = ALGORITHM_RANDOM;
  else if (a == "hill climber")
    m_algorithm = ALGORITHM_HILL_CLIMBER;
  else
    return -1;
  return 0;
}

int PlayerConfiguration::readKeyFile(const Glib::KeyFile& key)
{
  string type, algorithm;
  
  type = key.get_string(m_sectionName, "type");
  if (setType(type) != 0)
    return -1;
  if (m_type == PLAYER_COMPUTER)
    {
      algorithm = key.get_string(m_sectionName, "algorithm");
      if (setAlgorithm(algorithm) != 0)
	return -1;
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

Configuration& Configuration::getInstance()
{
  static Configuration configuration;
  return configuration;
}

int Configuration::parseKeyFile() throw(Glib::KeyFileError)
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

  //TODO: emit signal
  return 0;
}

int Configuration::readString(const string& data)
{
  m_keyFile.load_from_data(data, Glib::KEY_FILE_KEEP_COMMENTS);
  parseKeyFile();
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
  
