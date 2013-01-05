#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <glibmm/keyfile.h>
#include <glibmm/fileutils.h>
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

  PlayerType stringToPlayerType(const std::string& type)
    const throw(DaoException);

  PlayerAlgorithm stringToPlayerAlgorithm(const std::string& algorithm)
    const throw(DaoException);

  void readKeyFile(const Glib::KeyFile& key) 
    throw(DaoException, Glib::KeyFileError);
};

class Configuration
{
public:
  static const int NUM_PLAYERS = 2;
  int m_firstPlayer;
  PlayerConfiguration m_players[NUM_PLAYERS];

  static Configuration& getInstance()
    throw();

  void readString(const std::string& s)
    throw(DaoException);

  void readFile(const std::string& filename)
    throw(DaoException, Glib::FileError);

  std::string getData();

public: //SIGNALS
  sigc::signal<void> signal_changed;

private:
  Glib::KeyFile m_keyFile;

  //Prevent object from being copied
  Configuration();
  Configuration(const Configuration& c) {};
  Configuration& operator=(const Configuration& c) {
    return *this;
  };
  void parseKeyFile()
    throw(DaoException, Glib::KeyFileError);
};

#endif
