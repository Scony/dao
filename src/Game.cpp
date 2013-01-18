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
	PlayerFactory::createPlayer(config.m_players[i], this);

      m_players[i]->signal_move_proposed.connect( sigc::mem_fun( *this, &Game::performMove));
    }

  //TODO: Implement reading custom initial states
  m_states.clear();
  m_previous_state_hashes.clear();

  m_currentPlayer = config.m_firstPlayer;
  State initialState(config.m_firstPlayer);
  m_states.push_back(initialState);
  m_previous_state_hashes.insert(initialState.getHash());
  
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
  m_previous_state_hashes.insert(nextState.getHash());
  
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

void Game::getAvailableMoves(MoveSet* moveSet, State* state) const
{
  //Wypisywanie stanu
  for(int i = 0; i < 4; i++)
    {
      for(int j = 0; j < 4; j++)
	cout << state->m_board.m_fields[i][j] << " ";
      cout << endl;
    }

  Board* board = &(state->m_board);
  
  moveSet->clear();
  for(int from_x = 0; from_x < 4; from_x++)
    for(int from_y = 0; from_y < 4; from_y++)
      if(board->m_fields[from_y][from_x] == state->m_current)
	{
	  //Iteracja po wszystkich pionkach gracza
	  
	  for(int x_step = -1; x_step <= 1; x_step++)
	    for(int y_step = -1; y_step <= 1; y_step++)
	      {
		//Iteracja po wszystkich kierunkach ruchu
		int to_x = from_x;
		int to_y = from_y;
		
		while(board->isEmptyField(to_x + x_step, to_y + y_step))
		  {
		    to_x += x_step;
		    to_y += y_step;
		  }
		
		if ((to_x != from_x) || (to_y != from_y))
		  moveSet->add(Move(from_x + 4 * from_y,
				    to_x + 4 * to_y));
	      }
	} 
}

void Game::filterCycles(MoveSet* moveSet) const
{
  MoveSet::Iterator it = moveSet->begin();

  State current = getCurrentState();
  
  while(it != moveSet->end())
    {
      cout << "Moving state" << endl;
      cout << "from" << it.at().from << " to " << it.at().to << endl;
      State next = current.move(it.at());
      cout << "Moved state" << endl;
      dao_hash_short hash = next.getHash();
      if (m_previous_state_hashes.find(hash) !=
	  m_previous_state_hashes.end())
	{
	  cout << "removing" << endl;
	  it = moveSet->remove(it);
	  cout << "removed" << endl;
	}
      else
	{
	  it++;
	}
    }
}
