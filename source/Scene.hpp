#pragma once

#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "raylib.h"

#include "layer/LayerInterface.hpp"
#include "elevation/srtm/SRTMElevation.hpp"

#include "Tile.hpp"
#include "CustomCamera.hpp"

class Scene {
private:
    KCore::LayerInterface *mLayer;
    KCore::SRTMElevation *mElevation;

public:
    CustomCamera mCamera;
    std::map<std::string, Tile> mTiles;

public:
    Scene();

    void update();

    void draw() const;
};
