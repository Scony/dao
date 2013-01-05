#ifndef BOARD_H
#define BOARD_H

enum FieldState
  {
    FIELD_EMPTY,
    FIELD_COLOR0,
    FIELD_COLOR1
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
  int getWinner();
  bool isTerminal();

  static Board initialBoard();

};

#endif
