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
    float x, y;
    float width, height;
} Rectangle;

// Colors
typedef struct {
    uint8_t r, g, b, a;
} Color;

// Taken from lospec.com/palette-list/slso-clr17 (liked the colors)
#define DARK_PURPLE    (Color){46,  44,  59,  255}
#define DARK_STEEL     (Color){62,  65,  95,  255}
#define STEEL_BLUE     (Color){85,  96,  125, 255}
#define SLATE_GRAY     (Color){116, 125, 136, 255}
#define MINT           (Color){65,  222, 149, 255}
#define TEAL           (Color){42,  164, 170, 255}
#define BLUE           (Color){59,  119, 166, 255}
#define DARK_GREEN     (Color){36,  147, 55,  255}
#define GREEN          (Color){86,  190, 68,  255}
#define LIME           (Color){198, 222, 120, 255}
#define YELLOW         (Color){243, 194, 32,  255}
#define ORANGE         (Color){196, 101, 28,  255}
#define RED            (Color){181, 65,  49,  255}
#define DARK_VIOLET    (Color){97,  64,  122, 255}
#define VIOLET         (Color){143, 61,  167, 255}
#define PINK           (Color){234, 97,  157, 255}
#define ICE_BLUE       (Color){193, 229, 234, 255}

void ClearBackground(Color color);
void DrawPixel(int x, int y, Color color);

#endif // KOLIB_CORE_H
