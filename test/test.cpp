#include "snake.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten.h>

#include <cmath>

const size_t kTableX = 600;
const size_t kTableY = 600;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

TTF_Font *font = nullptr;

Snake my_snake;
Apple my_apple = {100, 100};

int cur_tick = 0;

void render_text(const char *message, int x, int y) {
  SDL_Color color = {0, 0, 0, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, message, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  int width = 0;
  int height = 0;
  SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
  SDL_Rect dst_rect = {x, y, width, height};
  SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);
  SDL_DestroyTexture(texture);
}

float start_x = 0.0f;
float start_y = 0.0f;

void main_loop() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN && my_snake.state == GameState::Playing) {
      switch (event.key.keysym.sym) {
      case SDLK_UP:
        change_direction(my_snake, 0, -1);
        break;
      case SDLK_DOWN:
        change_direction(my_snake, 0, 1);
        break;
      case SDLK_LEFT:
        change_direction(my_snake, -1, 0);
        break;
      case SDLK_RIGHT:
        change_direction(my_snake, 1, 0);
        break;
      }
    } else if (event.type == SDL_FINGERDOWN) {
      start_x = event.tfinger.x;
      start_y = event.tfinger.y;
    } else if (event.type == SDL_FINGERUP &&
               my_snake.state == GameState::Playing) {
      float end_x = event.tfinger.x;
      float end_y = event.tfinger.y;

      float vec_x = end_x - start_x;
      float vec_y = end_y - start_y;

      if (!(abs(vec_x) < 0.1 && abs(vec_y) < 0.1)) {
        if (abs(vec_x) > abs(vec_y)) {
          if (vec_x > 0) {
            change_direction(my_snake, 1, 0);
          } else {
            change_direction(my_snake, -1, 0);
          }
        } else {
          if (vec_y > 0) {
            change_direction(my_snake, 0, 1);
          } else {
            change_direction(my_snake, 0, -1);
          }
        }
      }
    }
  }

  const int tick = SDL_GetTicks();
  if (tick - cur_tick >= 200) {
    if (my_snake.state == GameState::Playing) {
      move_snake(my_snake, my_apple);
    }
    cur_tick = tick;
  }
  SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);

  for (const auto &seg : my_snake.body) {
    SDL_Rect rect = {seg.first, seg.second, 20, 20};
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect apple_rect = {my_apple.x, my_apple.y, 20, 20};
  SDL_RenderFillRect(renderer, &apple_rect);

  SDL_RenderPresent(renderer);

  if (my_snake.state == GameState::Won) {
    render_text("You won!", 150, 180);
  } else if (my_snake.state == GameState::Lost) {
    render_text("You lost!", 150, 180);
  } else {
    SDL_SetWindowTitle(window, "Snake");
  }
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  font = TTF_OpenFont("aboba.otf", 40);

  SDL_CreateWindowAndRenderer(kTableX, kTableY, 0, &window, &renderer);

  init_snake(my_snake);

  emscripten_set_main_loop(main_loop, 0, 1);
  return 0;
}
