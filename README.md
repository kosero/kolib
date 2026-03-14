# kolib
 
Inspired by Raylib. Built on SDL3 + OpenGL, written in C99 for my own games. Feel free to use it — just know I made this for myself, so there might be rough edges and messy code. I'm not a pro, just someone who likes making games.

---

## Requirements
 
- CMake 3.5+
- SDL3
- A C99-compatible compiler

---
 
## Building
 
```bash
git clone --recurse-submodules https://github.com/kosero/kolib
cd kolib
cmake -B build
cmake --build build
```

---

### Basic window Example
 
```c
#include <kolib.h>
 
int main(void) {
    InitWindow("Basic Window", 800, 600);
    TargetFPS(60);
 
    while (!WindowShouldClose()) {
        float dt = GetDeltaTime();
 
        ClearBackground((Color){30, 30, 30, 255});
        DrawPixel(400, 300, (Color){255, 0, 128, 255});
        
        Present();
    }
 
    CloseWindow();
    return 0;
}
```
