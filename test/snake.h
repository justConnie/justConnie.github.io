#pragma once

#include <deque>
#include <utility>

enum class GameState { Playing, Won, Lost };

struct Snake {
  int x;
  int y;
  int dx;
  int dy;
  GameState state;
  std::deque<std::pair<int, int>> body;
};

struct Apple {
  int x;
  int y;
};

void init_snake(Snake &snake);
void change_direction(Snake &snake, int new_dx, int new_dy);
void move_snake(Snake &snake, Apple &apple);
