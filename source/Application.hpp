#pragma once

#include "Scene.hpp"

class Application {
private:
    int mScreenWidth = 1280, mScreenHeight = 800;
    Scene* mScene;
public:
    Application();

    ~Application();

    void start();
};