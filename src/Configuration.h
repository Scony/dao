#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <glibmm/keyfile.h>
#include <glibmm/fileutils.h>
#include <sigc++/sigc++.h>

#include "DaoException.h"
#include "Game.h"
#include "Player.h"

struct PlayerConfiguration
{
  std::string m_sectionName;
  int m_color;
  PlayerType m_type;
  PlayerAlgorithm m_algorithm;
  std::string m_name;

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
  int m_firstPlayer;
  PlayerConfiguration m_players[Game::NUM_PLAYERS];

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
