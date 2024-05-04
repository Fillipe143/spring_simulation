#include <raylib.h>

const int screen_width = 800;
const int screen_height = 800;
const int target_fps = 60;

void loop();

int main() {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(screen_width, screen_height, "SpringSimulation");
    SetTargetFPS(target_fps);

    while (!WindowShouldClose()) loop();
    CloseWindow();

    return 0;
}

void loop() {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
}
