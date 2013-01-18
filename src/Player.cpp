#include "Player.h"
#include "Configuration.h"
#include "Human.h"
#include "Computer.h"

#include <iostream>
using namespace std;

Player::Player(const PlayerConfiguration& config, const Game* game)
{
  //TODO: Zapamietywanie game
  m_color = config.m_color;
  m_name = config.m_name;  
}

Player::~Player()
{
}

GraphicalBoard* PlayerFactory::s_gBoard = 0;

Player* PlayerFactory::createPlayer(const PlayerConfiguration& config,
				    const Game* game)
  throw(DaoException)
{
  switch (config.m_type)
    {
    case PLAYER_HUMAN:
      return new Human(config, s_gBoard);
    case PLAYER_COMPUTER:
      {
	AIStrategy* strategy;
	switch(config.m_algorithm)
	  {
	  case ALGORITHM_RANDOM:
	    strategy = new Random(game, config);
	    return new Computer(config, game, strategy);
	  case ALGORITHM_HILL_CLIMBER:
	    strategy = new HillClimber(game, config);
	    return new Computer(config, game, strategy);
	  default:
	    throw DaoException("Algorithm type not implemented");
	  }
      }
    default:
      throw DaoException("Player type not implemented");
    }
}

void PlayerFactory::setGBoard(GraphicalBoard* board)
{
  s_gBoard = board;
}
