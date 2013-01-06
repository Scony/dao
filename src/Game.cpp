#include "Game.h"
#include "DaoException.h"
#include "Configuration.h"

Game::Game()
{
  m_players[0] = 0;
  m_players[1] = 0;
}

Game::~Game()
{
}

void Game::newGame()
  throw (DaoException)
{
  Configuration& config = Configuration::getInstance();

  //TODO: Check for good colors
  for(int i = 0; i < NUM_PLAYERS; i++)
    {
      if (m_players[i])
	{
	  delete m_players[i];
	  m_players[i] = 0; //This is important since createPlayer
	  //can throw exceptions
	}
      m_players[i] =
	PlayerFactory::createPlayer(config.m_players[i]);
    }

  //TODO: Clear game history
  //TODO: Implement reading custom initial states
  m_states.clear();
  State initialState(config.m_firstPlayer);
  m_states.push_back(initialState);
  
  signal_new_game.emit(m_states.back());
}

    
