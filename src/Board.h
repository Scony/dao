#ifndef BOARD_H
#define BOARD_H

enum FieldState
  {
    FIELD_PLAYER1 = 0,
    FIELD_PLAYER2 = 1,
    FIELD_EMPTY = 2,
  };

class Board
{
public:
  static const int SIZE = 4;

  FieldState m_fields[SIZE][SIZE];
  
  Board();
  Board(const Board& b);
  ~Board();

  Board rotate180();

  FieldState getWinner();
  bool isTerminal();

  static Board initialBoard();

};

#endif
