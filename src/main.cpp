#include <raylib.h>

const int screen_width = 800;
const int screen_height = 800;
const int target_fps = 60;

const float gravity_force = 1;

void loop();

struct Particle {
    Vector2 pos, spd, acc;
    float mass;
};

struct Spring {
    float k, rest_length;
    Particle a, b;
};

Particle new_particle(float x, float y) {
    return Particle {
        .pos = Vector2 { x, y },
        .spd = Vector2 { 0, 0 },
        .acc = Vector2 { 0, 0 },
        .mass = 1
    };
}

Spring new_spring(float k, float rest_length, Particle a, Particle b) {
    return Spring { k, rest_length, a, b };
}

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
