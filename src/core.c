#include "core.h"
#include <stdint.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    int should_close;
} Ctx;

static Ctx ctx = {0};

void Present(void) {
    SDL_RenderPresent(ctx.renderer);
}

int InitWindow(const char* title, int w, int h) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not be initialized: %s", SDL_GetError());
        return 1;
    }

    ctx.window = SDL_CreateWindow(title, w, h, 0);
    if (!ctx.window) {
        SDL_Log("SDL could not create window: %s", SDL_GetError());
        return 1;
    }

    ctx.renderer = SDL_CreateRenderer(ctx.window, NULL);
    if (!ctx.renderer) {
        SDL_Log("SDL could not create renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
    Present();

    return 0;
}

int WindowShouldClose(void) {
    SDL_PollEvent(&ctx.event);
    if (ctx.event.type == SDL_EVENT_QUIT)
        ctx.should_close = 1;
    return ctx.should_close;
}

void CloseWindow(void) {
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();
}

SDL_Window* GetWindowPtr(void) {
    return ctx.window;
}
SDL_Renderer* GetRendererPtr(void) {
    return ctx.renderer;
}

static uint64_t _lastTime = 0;
static float _deltaTime = 0.0f;
static uint64_t now;
float GetDeltaTime(void) {
    now = SDL_GetTicks();
    _deltaTime = (_lastTime == 0) ? 0.010f : (float)(now - _lastTime) / 1000000000.0f;
    _lastTime = now;
    return _deltaTime;
}

void TargetFPS(int number) {
    if (number <= 0) return;
    uint64_t frameEnd = SDL_GetTicksNS();
    uint64_t frameTime = frameEnd - now;
    uint64_t targetTime = 1000000000ULL / (uint64_t)number;

    if (frameTime < targetTime) {
      SDL_DelayNS(targetTime - frameTime);
    }
}

void ClearBackground(Color color) {
    SDL_SetRenderDrawColor(ctx.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(ctx.renderer);
}

void DrawPixel(int x, int y, Color color) {
    SDL_SetRenderDrawColor(ctx.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderPoint(ctx.renderer, x, y);
}
