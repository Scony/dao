#ifndef GAME_H
#define GAME_H

#include <sigc++/sigc++.h>
#include <vector>
#include <set>
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
  bool gameStarted() const;

  const pPlayer* getPlayers() const;
  const Player* getCurrentPlayer() const;
  State getCurrentState() const;

  void getAvailableMoves(MoveSet* dest, const State* state) const;
  void filterCycles(MoveSet* moveSet) const;
  //filterSymmetic(Moveset* )
  /* SLOTS */
  bool performMove(Player* player, Move move);

  std::ostream& write(std::ostream& out) const;
  std::istream& read(std::istream& in);
  

public: /* SIGNALS */
  sigc::signal<void, const Game&> signal_new_game;
  sigc::signal<void, State, const Player&> signal_state_changed;
  sigc::signal<void, const Player&> signal_game_end;
  
private:
  Player* m_players[NUM_PLAYERS];
  std::vector<State> m_states;
  std::set<dao_hash_short> m_previous_state_hashes;
  int m_currentPlayer;
  bool m_gameStarted;
};

#endif
