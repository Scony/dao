#ifndef PLAYER_H
#define PLAYER_H

#include <glibmm/dispatcher.h>
#include <sigc++/sigc++.h>
#include <string>
#include "State.h"
#include "Move.h"
#include "DaoException.h"

struct GraphicalBoard;
struct PlayerConfiguration;
class Game;

enum PlayerColor
  {
    COLOR_RED = 0,
    COLOR_BLUE,
    COLOR_NUM,
    COLOR_EMPTY
  };

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
  PlayerColor m_color;
  std::string m_name;
  
  Player(const PlayerConfiguration& config, const Game* game=0);
  virtual ~Player();

  virtual void proposeMove(State state) = 0;
  virtual bool isInteractive() const = 0;

  sigc::signal<bool, Player*, Move> signal_move_proposed;
  
protected:
  Move m_proposedMove;

  void getAvailableMoves(MoveSet * moveSet, State * state);
  void onDispatcherMoveProposed();
  //filterCycles(Moveset* ) //korzystalo z this->game
  //filterSymmetic(Moveset* )
  Glib::Dispatcher dispatcher_move_proposed;
};

typedef Player* pPlayer;

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
