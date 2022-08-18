#include "Tile.hpp"

#include <random>

Tile::Tile(KCore::TilePayloadEvent &payload) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> uni(0.0f, 1.0f);

    mPosition = glm::vec3{-1 * payload.center[0], 0.0f, payload.center[1]};
    mScale = payload.scale;
    mColor = glm::vec4{uni(rng), uni(rng), uni(rng), 1.0f};
    mModel = std::nullopt;
}

Tile::~Tile() {
    if (mModel.has_value())
        UnloadModel(mModel.value());
}

void Tile::draw() const {
    auto position = getPositionRaylib();
    DrawModelEx(mModel.value(), position, Vector3{0, 1, 0}, 180.0f,
                Vector3{-mScale, 1.0f, mScale}, WHITE);
}

void Tile::drawPlaceholder(const Model &model) const {
    auto color = getColorRaylib();
    auto position = getPositionRaylib();
    DrawModel(model, position, mScale, color);
}

void Tile::setPosition(const glm::vec3 &position) {
    mPosition = position;
}

void Tile::setScale(float scale) {
    mScale = scale;
}

void Tile::setColor(const glm::vec4 &color) {
    mColor = color;
}

void Tile::setModel(const Model &model) {
    mModel = model;
}

const glm::vec3 &Tile::getPositionGlm() const {
    return mPosition;
}

float Tile::getScale() const {
    return mScale;
}

const glm::vec4 &Tile::getColorGlm() const {
    return mColor;
}

Vector3 Tile::getPositionRaylib() const {
    return {mPosition.x, mPosition.y, mPosition.z};
}

Color Tile::getColorRaylib() const {
    auto multed_vec = mColor * 255.0f;
    return {static_cast<unsigned char>(multed_vec.x),
            static_cast<unsigned char>(multed_vec.y),
            static_cast<unsigned char>(multed_vec.z),
            static_cast<unsigned char>(multed_vec.w)};
}

const std::optional<Model> &Tile::getModel() const {
    return mModel;
}
