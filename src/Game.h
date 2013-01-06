#ifndef GAME_H
#define GAME_H

#include <sigc++/sigc++.h>
#include <vector>
#include "State.h"
#include "Player.h"
#include "Move.h"
#include "DaoException.h"

class Game : public sigc::trackable
{
public:
  static const int NUM_PLAYERS = 2;

  Game();
  ~Game();

  void newGame()
    throw(DaoException);
  
  /* SLOTS */
  bool performMove(Player* player, Move move);
  

public: /* SIGNALS */
  sigc::signal<void, State, const Player&> signal_new_game;
  sigc::signal<void, State, const Player&> signal_state_changed;
  
private:
  Player* m_players[NUM_PLAYERS];
  std::vector<State> m_states;
  int m_currentPlayer;
};

#endif
