#include <raylib.h>

const int screen_width = 800;
const int screen_height = 800;
const int target_fps = 60;

const float gravity_force = 1;
const float default_k_value = 0.1;
const float default_rest_length = 200;

const float particle_radius = 20;

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

void render_particle(Particle* self) {
    DrawCircle(self->pos.x, self->pos.y, particle_radius, BLUE);
}

void render_spring(Spring* self) {
    DrawLine(self->a.pos.x, self->a.pos.y, self->b.pos.x, self->b.pos.y, WHITE);
}

int main() {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(screen_width, screen_height, "SpringSimulation");
    SetTargetFPS(target_fps);

    while (!WindowShouldClose()) loop();
    CloseWindow();

    return 0;
}

Particle particles[] = { new_particle(screen_width / 2.0f, particle_radius), new_particle(screen_width / 2.0f, 200) };
Spring springs[] = {new_spring(default_k_value, default_rest_length, particles[0], particles[1])};

void loop() {
    BeginDrawing();
    ClearBackground(BLACK);
    for (auto spring : springs) render_spring(&spring);
    for (auto particle : particles) render_particle(&particle);
    EndDrawing();
}
