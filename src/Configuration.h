#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <glibmm/keyfile.h>
#include <sigc++/sigc++.h>

#include "DaoException.h"

enum PlayerType
  {
    PLAYER_HUMAN,
    PLAYER_COMPUTER
  };

enum PlayerAlgorithm
  {
    ALGORITHM_RANDOM,
    ALGORITHM_HILL_CLIMBER
  };

struct PlayerConfiguration
{
  std::string m_sectionName;
  int m_color;
  PlayerType m_type;
  PlayerAlgorithm m_algorithm;

  int setType(const std::string& type);
  int setAlgorithm(const std::string& algorithm);
  int readKeyFile(const Glib::KeyFile& key);
};

class Configuration
{
public:
  static const int NUM_PLAYERS = 2;
  int m_firstPlayer;
  PlayerConfiguration m_players[NUM_PLAYERS];

  static Configuration& getInstance();
  int readString(const std::string& s);
  void readFile(const std::string& filename) throw(DaoException);
  std::string getData();
private:
  Glib::KeyFile m_keyFile;

  //Prevent object from being copied
  Configuration();
  Configuration(const Configuration& c) {};
  Configuration& operator=(const Configuration& c) {
    return *this;
  };
  int parseKeyFile() throw(Glib::KeyFileError);
};

#endif
