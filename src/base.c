#include "./constants.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int renderer_running = FALSE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture* textures[1];

int last_frame_time = 0;
int width;
int paddle_x_pos;
int score;

struct vector2 {
  float x;
  float y;
} speed;

struct shape {
  float x;
  float y;
  float width;
  float height;
} ball, paddle;

int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return FALSE;
  }

  window =
      SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return FALSE;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    fprintf(stderr, "Error creating SDl renderer.\n");
    return FALSE;
  }

  return TRUE;
}

void setup() {
  srand(time(0));

  ball.x = rand() % WINDOW_WIDTH;
  ball.y = 0;
  ball.width = 12;
  ball.height = 12;

  width = 200;
  paddle_x_pos = WINDOW_WIDTH / 2 - width / 2;

  paddle.y = WINDOW_HEIGHT - 25;
  paddle.height = 10;

  speed.x = 5.0;
  speed.y = 5.0;
  score = 0;
}

void input_switch(SDL_Keysym keysym) {
  switch (keysym.sym) {
    case SDLK_ESCAPE:
      renderer_running = FALSE;
      break;
    case SDLK_RIGHT:
      paddle_x_pos += 15;
      break;
    case SDLK_LEFT:
      paddle_x_pos -= 15;
      break;
    case SDLK_r:
      setup();
      break;
  }
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      renderer_running = FALSE;
      break;
    case SDL_KEYDOWN:
      input_switch(event.key.keysym);
      break;
    case SDL_KEYUP:
      input_switch(event.key.keysym);
      break;
  }
}

void check_collision() {
  float y_pos = ball.y + (ball.height / 2);
  float x_pos = ball.x + (ball.width / 2);

  if(y_pos > WINDOW_HEIGHT){
    printf("Your score: %i\n", score);
    setup();
  }

  if(y_pos <= 0){
    speed.y = -speed.y;
  }

  if((y_pos >= WINDOW_HEIGHT - 25 && (x_pos >= paddle_x_pos && x_pos <= paddle_x_pos + width ))) {
    speed.y = -speed.y;
    score += 1;
    width -= 2;
    speed.y *= 1.05;
    speed.x *= 1.05;
  }

  if(x_pos <= 0 || x_pos >= WINDOW_WIDTH){
    speed.x = -speed.x;
  }
}

void update() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME))
    ;

  last_frame_time = SDL_GetTicks();

  ball.x += speed.x;
  ball.y += speed.y;

  check_collision();
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect ball_rect = {ball.x, ball.y, ball.width, ball.height};
  SDL_Rect paddle_rect = {paddle_x_pos, paddle.y, width, paddle.height};

  SDL_RenderFillRect(renderer, &ball_rect);
  SDL_RenderFillRect(renderer, &paddle_rect);

  SDL_RenderPresent(renderer);
}

void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main() {
  renderer_running = initialize_window();

  setup();

  while (renderer_running) {
    process_input();
    update();
    render();
  }

  printf("Your score: %i\n", score);

  destroy_window();

  return FALSE;
}
