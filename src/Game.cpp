#include <iostream>

#include "Game.h"
#include "DaoException.h"
#include "Configuration.h"

using namespace std;

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

      m_players[i]->signal_move_proposed.connect( sigc::mem_fun( *this, &Game::performMove));
    }

  //TODO: Implement reading custom initial states
  m_states.clear();
  m_currentPlayer = config.m_firstPlayer;
  State initialState(config.m_firstPlayer);
  m_states.push_back(initialState);
  
  
  signal_new_game.emit(*this);

  m_players[m_currentPlayer]->proposeMove(m_states.back());
}

bool Game::performMove(Player* player, Move move)
{
  //TODO: verify move
  
  cout << "TODO: verify and perform move" << endl;

  const State& currentState = m_states.back();
  State nextState = currentState.move(move);
  m_states.push_back(nextState);
  
  if (!nextState.m_board.isTerminal())
    {
      m_currentPlayer = nextState.m_current;
      signal_state_changed.emit(m_states.back(), 
				*m_players[m_currentPlayer]);
      
      m_players[m_currentPlayer]->proposeMove(nextState);
      return true;
    }
  else
    {
      signal_state_changed.emit(m_states.back(),
				*m_players[m_currentPlayer]);
      signal_game_end.emit(*m_players[m_currentPlayer]);
      return true;
    }
}

const pPlayer* Game::getPlayers() const 
{
  return m_players;
}

const Player* Game::getCurrentPlayer() const
{
  return m_players[m_currentPlayer];
}

State Game::getCurrentState() const
{
  return m_states.back();
}

