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

  dispatcher_move_proposed.connect( sigc::mem_fun(*this, &Player::onDispatcherMoveProposed ));
}

Player::~Player()
{
}

void Player::getAvailableMoves(MoveSet * moveSet, State * state)
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
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      if(board->m_fields[i][j] == state->m_current)
	{
	  int k;
	  int from = i + 4 * j;

	  //vertical
	  k = i;
	  while(k - 1 >= 0 && board->m_fields[k - 1][j] == FIELD_EMPTY)
	    k--;
	  if(k != i)
	    moveSet->add(Move(from, k + 4 * j));
	  
	  k = i;
	  while(k + 1 < 4 && board->m_fields[k + 1][j] == FIELD_EMPTY)
	    k++;
	  if(k != i)
	    moveSet->add(Move(from, k + 4 * j));
	}
}

	  /*
	  //horizontal

	  moveSet->moves[msi].from = i + 4 * j;
	  moveSet->moves[msi].to = -1;
	  l = j;
	  while(l - 1 >= 0 && state->m_board.m_fields[i][l - 1] == FIELD_EMPTY)
	    l--;
	  if(l != j)
	    moveSet->moves[msi++].to = i + 4 * l;

	  moveSet->moves[msi].from = i + 4 * j;
	  moveSet->moves[msi].to = -1;
	  l = j;
	  while(l + 1 < 4 && state->m_board.m_fields[i][l + 1] == FIELD_EMPTY)
	    l++;
	  if(l != j)
	    moveSet->moves[msi++].to = i + 4 * l;

	  //right slant

	  moveSet->moves[msi].from = i + 4 * j;
	  moveSet->moves[msi].to = -1;
	  k = i;
	  l = j;
	  while(k - 1 >= 0 && l - 1 >= 0 && state->m_board.m_fields[k - 1][l - 1] == FIELD_EMPTY)
	    {
	      k--;
	      l--;
	    }
	  if(k != i)
	    moveSet->moves[msi++].to = k + 4 * l;

	  moveSet->moves[msi].from = i + 4 * j;
	  moveSet->moves[msi].to = -1;
	  k = i;
	  l = j;
	  while(k + 1 < 4 && l + 1 < 4 && state->m_board.m_fields[k + 1][l + 1] == FIELD_EMPTY)
	    {
	      k++;
	      l++;
	    }
	  if(k != i)
	    moveSet->moves[msi++].to = k + 4 * l;

	  //left slant

	  moveSet->moves[msi].from = i + 4 * j;
	  moveSet->moves[msi].to = -1;
	  k = i;
	  l = j;
	  while(k - 1 >= 0 && l + 1 < 4 && state->m_board.m_fields[k - 1][l + 1] == FIELD_EMPTY)
	    {
	      k--;
	      l++;
	    }
	  if(k != i)
	    moveSet->moves[msi++].to = k + 4 * l;

	  moveSet->moves[msi].from = i + 4 * j;
	  moveSet->moves[msi].to = -1;
	  k = i;
	  l = j;
	  while(k + 1 < 4 && l - 1 >= 0 && state->m_board.m_fields[k + 1][l - 1] == FIELD_EMPTY)
	    {
	      k++;
	      l--;
	    }
	  if(k != i)
	    moveSet->moves[msi++].to = k + 4 * l;
	}

  if(msi > 15 || moveSet->moves[msi].to == -1)
    msi--;
  if(msi >= 0)
    {
      moveSet->moves[msi].next = NULL;
      while(--msi >= 0)
	moveSet->moves[msi].next = &moveSet->moves[msi+1];
    }
	  */

void Player::onDispatcherMoveProposed()
{
  signal_move_proposed.emit(this, m_proposedMove);
}

GraphicalBoard* PlayerFactory::s_gBoard = 0;

Player* PlayerFactory::createPlayer(const PlayerConfiguration& config)
  throw(DaoException)
{
  switch (config.m_type)
    {
    case PLAYER_HUMAN:
      return new Human(config, s_gBoard);
    case PLAYER_COMPUTER:
      return new Random(config);
    default:
      throw DaoException("Player type not implemented");
    }
}

void PlayerFactory::setGBoard(GraphicalBoard* board)
{
  s_gBoard = board;
}
