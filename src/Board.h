#ifndef BOARD_H
#define BOARD_H

enum FieldState
  {
    FIELD_STATE_EMPTY,
    FIELD_STATE_COLOR_0,
    FIELD_STATE_COLOR_1
  };

class Board
{
public:
  static const int SIZE = 4;

  FieldState m_board[SIZE][SIZE];
  
  Board();
  Board(const Board& b);
  ~Board();

  Board rotate180();
  int getWinner();
  bool isTerminal();

  static Board initialBoard();

};

#endif
