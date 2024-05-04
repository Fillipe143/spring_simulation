#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <raylib.h>

const int screen_width = 800;
const int screen_height = 800;
const int target_fps = 60;

const float gravity_force = 0.3;
const float default_k_value = 0.05f;
const float default_rest_length = 200;

const float particle_radius = 20;
int particle_hue = -30;

void loop();

struct Particle {
    Vector2 pos, spd, acc;
    Color color;
    bool fixed;
    float mass;
};

struct Spring {
    float k, rest_length;
    Particle *a, *b;
};

Particle new_particle(float x, float y, bool fixed = false) {
    particle_hue = (particle_hue + 30) % 360;
    return Particle {
        .pos = Vector2 { x, y },
        .spd = Vector2 { 0, 0 },
        .acc = Vector2 { 0, 0 },
        .color = ColorFromHSV(particle_hue++, 0.8, 1.0),
        .fixed = fixed,
        .mass = 1
    };
}

Spring new_spring(float k, float rest_length, Particle* a, Particle* b) {
    return Spring { k, rest_length, a, b };
}

void render_particle(Particle* self) {
    DrawCircle(self->pos.x, self->pos.y, particle_radius, self->color);
}

void render_spring(Spring* self) {
    DrawLine(self->a->pos.x, self->a->pos.y, self->b->pos.x, self->b->pos.y, WHITE);
}

void apply_force(Particle* self, Vector2 force) {
    force.x /= self->mass;
    force.y /= self->mass;

    self->acc.x += force.x;
    self->acc.y += force.y;
}

void update_particle(Particle* self, float delta_time) {
    if (self->fixed) return;
    apply_force(self, Vector2 { 0, gravity_force });
    // slow down
    self->spd.x *= 0.99;
    self->spd.y *= 0.99;

    // Speed += Acceleration
    self->spd.x += self->acc.x;
    self->spd.y += self->acc.y;

    // Position += Speed
    self->pos.x += self->spd.x * delta_time;
    self->pos.y += self->spd.y * delta_time;

    self->acc = { 0, 0 };
}

void update_spring(Spring* self) {
    Vector2 force = { self->b->pos.x - self->a->pos.x, self->b->pos.y - self->a->pos.y };

    float magnitude = sqrt(force.x * force.x + force.y * force.y);
    float x = magnitude - self->rest_length;

    // Normalize force
    force.x /= magnitude;
    force.y /= magnitude;

    // Apply hooke's law
    force.x *= self->k * x;
    force.y *= self->k * x;

    apply_force(self->a, force);

    force.x *= -1;
    force.y *= -1;
    apply_force(self->b, force);
}

int main() {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(screen_width, screen_height, "SpringSimulation");
    SetTargetFPS(target_fps);

    while (!WindowShouldClose()) loop();
    CloseWindow();

    return 0;
}

Particle particles[] = {
    new_particle(screen_width / 2.0f, particle_radius, true),
    new_particle(screen_width / 2.0f, default_rest_length),
    new_particle(100, 100)
};

Spring springs[] = {
    new_spring(default_k_value, default_rest_length, &particles[0], &particles[1]),
    new_spring(default_k_value, default_rest_length, &particles[1], &particles[2]),
    new_spring(default_k_value, default_rest_length, &particles[2], &particles[0])
};

Particle* selected_particle = nullptr;
void loop() {
    // Update Spring and particles
    for (auto& spring : springs) update_spring(&spring);
    for (auto& particle : particles) update_particle(&particle, pow(0.99, GetFrameTime()));

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Change position of particle that mouse clicked
        for (auto& p : particles) {
            float delta_x = p.pos.x - GetMouseX();
            float delta_y = p.pos.y - GetMouseY();
            float distance_from_mouse = sqrt(delta_x * delta_x + delta_y * delta_y);

            // Selected particle
            if (distance_from_mouse <= particle_radius)
                selected_particle = &p;
        }
    } else {
        selected_particle = nullptr;
    }

    // Change particle position and stop
    if (selected_particle != nullptr) {
        selected_particle->pos.x = GetMouseX();
        selected_particle->pos.y = GetMouseY();
        selected_particle->spd = { 0, 0 };

        // Fixes and unfixes particle
        if (GetCharPressed() == 'f') selected_particle->fixed ^= true;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // Render Spring and particles
    for (auto spring : springs) render_spring(&spring);
    for (auto particle : particles) render_particle(&particle);
    EndDrawing();
}
