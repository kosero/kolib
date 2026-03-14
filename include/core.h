#ifndef KOLIB_CORE_H
#define KOLIB_CORE_H

#include <SDL3/SDL.h>

int InitWindow(const char* title, int w, int h);
void CloseWindow(void);
void Present(void);
int WindowShouldClose(void);
SDL_Window* GetWindowPtr(void);
SDL_Renderer* GetRendererPtr(void);
float GetDeltaTime(void);
void TargetFPS(int number);

typedef struct {
    uint8_t r, g, b, a;
} Color;

void ClearBackground(Color color);
void DrawPixel(int x, int y, Color color);

#endif // KOLIB_CORE_H
