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
	//TODO: Wybor strategii
	AIStrategy* strategy = new Random(game, config);
	return new Computer(config, game, strategy);
      }
    default:
      throw DaoException("Player type not implemented");
    }
}

void PlayerFactory::setGBoard(GraphicalBoard* board)
{
  s_gBoard = board;
}
