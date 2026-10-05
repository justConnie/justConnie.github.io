#include "snake.h"
#include <algorithm>
#include <random>

thread_local std::mt19937_64 rng{std::random_device{}()};
std::uniform_int_distribution<size_t> dist(0, 19);

const size_t kTableX = 400;
const size_t kTableY = 600;

void init_snake(Snake &snake) {
  snake.x = kTableX / 2;
  snake.y = kTableY / 2;
  snake.dx = 1;
  snake.dy = 0;
  snake.state = GameState::Playing;
  snake.body.clear();
  snake.body.push_back({snake.x, snake.y});
}

void change_direction(Snake &snake, int new_dx, int new_dy) {
  if (!(snake.body.size() > 1 && ((snake.dx != 0 && snake.dx + new_dx == 0) ||
                                  (snake.dy != 0 && snake.dy + new_dy == 0)))) {
    snake.dx = new_dx;
    snake.dy = new_dy;
  }
}

static bool contains(const int x, const int y,
                     const std::deque<std::pair<int, int>> &body) {
  return std::any_of(body.begin(), body.end(),
                     [x, y](const std::pair<int, int> &p) {
                       return p.first == x && p.second == y;
                     });
}

void move_snake(Snake &snake, Apple &apple) {
  if (snake.state != GameState::Playing) {
    return;
  }

  if (snake.dx > 0) {
    snake.x = snake.x + 20 <= kTableX - 20 ? snake.x + 20 : 0;
  } else if (snake.dx < 0) {
    snake.x = snake.x - 20 >= 0 ? snake.x - 20 : kTableX - 20;
  } else if (snake.dy > 0) {
    snake.y = snake.y + 20 <= kTableY - 20 ? snake.y + 20 : 0;
  } else if (snake.dy < 0) {
    snake.y = snake.y - 20 >= 0 ? snake.y - 20 : kTableY - 20;
  }
  if (contains(snake.x, snake.y, snake.body)) {
    snake.state = GameState::Lost;
    return;
  }
  snake.body.push_back({snake.x, snake.y});
  if (snake.x == apple.x && snake.y == apple.y) {
    if (snake.body.size() == kTableX) {
      snake.state = GameState::Won;
      return;
    }
    int new_x = dist(rng) * 20;
    int new_y = dist(rng) * 20;
    while (contains(new_x, new_y, snake.body)) {
      new_x = dist(rng) * 20;
      new_y = dist(rng) * 20;
    }
    apple.x = new_x;
    apple.y = new_y;
  } else {
    snake.body.pop_front();
  }
}
