#ifndef KOLIB_CORE_H
#define KOLIB_CORE_H

#include <SDL3/SDL.h>
#include "komath.h"


int InitWindow(const char* title, int w, int h);
void CloseWindow(void);
void Present(void);
int WindowShouldClose(void);
SDL_Window* GetWindowPtr(void);
SDL_Renderer* GetRendererPtr(void);
float GetDeltaTime(void);
void TargetFPS(int number);

int IsKeyDown(int key);
int IsKeyPressed(int key);

Vec2 GetMousePosition(void);
int IsMouseButtonDown(int button);

// Mouse Buttons
#define MOUSE_BUTTON_LEFT      1
#define MOUSE_BUTTON_MIDDLE    2
#define MOUSE_BUTTON_RIGHT     3

// Key Definitions
#define KEY_UNKNOWN            0
#define KEY_A                  4
#define KEY_B                  5
#define KEY_C                  6
#define KEY_D                  7
#define KEY_E                  8
#define KEY_F                  9
#define KEY_G                  10
#define KEY_H                  11
#define KEY_I                  12
#define KEY_J                  13
#define KEY_K                  14
#define KEY_L                  15
#define KEY_M                  16
#define KEY_N                  17
#define KEY_O                  18
#define KEY_P                  19
#define KEY_Q                  20
#define KEY_R                  21
#define KEY_S                  22
#define KEY_T                  23
#define KEY_U                  24
#define KEY_V                  25
#define KEY_W                  26
#define KEY_X                  27
#define KEY_Y                  28
#define KEY_Z                  29
#define KEY_1                  30
#define KEY_2                  31
#define KEY_3                  32
#define KEY_4                  33
#define KEY_5                  34
#define KEY_6                  35
#define KEY_7                  36
#define KEY_8                  37
#define KEY_9                  38
#define KEY_0                  39
#define KEY_RETURN             40
#define KEY_ESCAPE             41
#define KEY_BACKSPACE          42
#define KEY_TAB                43
#define KEY_SPACE              44
#define KEY_MINUS              45
#define KEY_EQUALS             46
#define KEY_LEFTBRACKET        47
#define KEY_RIGHTBRACKET       48
#define KEY_BACKSLASH          49
#define KEY_NONUSHASH          50
#define KEY_SEMICOLON          51
#define KEY_APOSTROPHE         52
#define KEY_GRAVE              53
#define KEY_COMMA              54
#define KEY_PERIOD             55
#define KEY_SLASH              56
#define KEY_CAPSLOCK           57
#define KEY_F1                 58
#define KEY_F2                 59
#define KEY_F3                 60
#define KEY_F4                 61
#define KEY_F5                 62
#define KEY_F6                 63
#define KEY_F7                 64
#define KEY_F8                 65
#define KEY_F9                 66
#define KEY_F10                67
#define KEY_F11                68
#define KEY_F12                69
#define KEY_PRINTSCREEN        70
#define KEY_SCROLLLOCK         71
#define KEY_PAUSE              72
#define KEY_INSERT             73
#define KEY_HOME               74
#define KEY_PAGEUP             75
#define KEY_DELETE             76
#define KEY_END                77
#define KEY_PAGEDOWN           78
#define KEY_RIGHT              79
#define KEY_LEFT               80
#define KEY_DOWN               81
#define KEY_UP                 82
#define KEY_NUMLOCKCLEAR       83
#define KEY_KP_DIVIDE          84
#define KEY_KP_MULTIPLY        85
#define KEY_KP_MINUS           86
#define KEY_KP_PLUS            87
#define KEY_KP_ENTER           88
#define KEY_KP_1               89
#define KEY_KP_2               90
#define KEY_KP_3               91
#define KEY_KP_4               92
#define KEY_KP_5               93
#define KEY_KP_6               94
#define KEY_KP_7               95
#define KEY_KP_8               96
#define KEY_KP_9               97
#define KEY_KP_0               98
#define KEY_KP_PERIOD          99
#define KEY_LCTRL              224
#define KEY_LSHIFT             225
#define KEY_LALT               226
#define KEY_LGUI               227
#define KEY_RCTRL              228
#define KEY_RSHIFT             229
#define KEY_RALT               230
#define KEY_RGUI               231

typedef struct {
    float x, y;
    float width, height;
} Rectangle;

// Colors
typedef struct {
    uint8_t r, g, b, a;
} Color;

typedef struct {
    SDL_Texture* handle;
    int width;
    int height;
} Texture;

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
void DrawRectangle(Rectangle rect, Color color);

Texture LoadTexture(const char* fileName);
void DrawTexture(Texture texture, int x, int y, Color tint);
void UnloadTexture(Texture texture);

#endif // KOLIB_CORE_H
