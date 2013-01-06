#ifndef PLAYER_H
#define PLAYER_H

#include <sigc++/sigc++.h>
#include <string>
#include "State.h"
#include "Move.h"
#include "DaoException.h"

struct GraphicalBoard;
struct PlayerConfiguration;

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

class Player : public sigc::trackable
{
public:
  int m_color;
  std::string m_name;
  
  Player(const PlayerConfiguration& config);
  virtual ~Player();

  virtual void proposeMove(State state) = 0;
  virtual bool isInteractive() const = 0;

public: /* SIGNALS */
  sigc::signal<bool, Player*, Move> signal_move_proposed;

};


class PlayerFactory
{
public:
  static Player* createPlayer(const PlayerConfiguration& config)
    throw(DaoException);
  static void setGBoard(GraphicalBoard* gBoard);

private:
  static GraphicalBoard* s_gBoard;
  PlayerFactory();
};


#endif
