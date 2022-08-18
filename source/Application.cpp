#include "Application.hpp"

#include "raylib.h"

Application::Application() {
    InitWindow(mScreenWidth, mScreenHeight, "KarafutoLayer C++ Example");

    SetTargetFPS(60);
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    mScene = new Scene();
}

Application::~Application() {
    CloseWindow();
}

void Application::start() {
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        mScene->update();
        mScene->draw();

        EndDrawing();
    }
}

