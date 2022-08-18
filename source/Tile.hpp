#pragma once

#include <optional>

#include "glm/glm.hpp"
#include "raylib.h"

#include "layer/events/EventPayloads.hpp"

class Tile {
private:
    glm::vec3 mPosition;
    float mScale;
    glm::vec4 mColor;
    std::optional<Model> mModel;

public:
    Tile() = default;

    explicit Tile
            (KCore::TilePayloadEvent &payload);

    ~Tile();

    void draw() const;

    void drawPlaceholder
            (const Model &model) const;

    [[nodiscard]] const glm::vec3 &getPositionGlm() const;

    [[nodiscard]] const glm::vec4 &getColorGlm() const;

    [[nodiscard]] Vector3 getPositionRaylib() const;

    [[nodiscard]] Color getColorRaylib() const;

    [[nodiscard]] float getScale() const;

    [[nodiscard]] const std::optional<Model> &getModel() const;

    void setPosition
            (const glm::vec3 &position);

    void setScale
            (float scale);

    void setColor
            (const glm::vec4 &color);

    void setModel
            (const Model &model);

    void setTexture
            (const KCore::ImagePayloadEvent &payload) {
        PixelFormat format;
        switch (payload.format) {
            case KCore::RGB565:
                format = PIXELFORMAT_UNCOMPRESSED_R5G6B5;
                break;
            case KCore::RGB888:
                format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
                break;
            case KCore::RGBA8888:
                format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
                break;
            default:
                throw std::runtime_error("Undefined pixel format!");
        }

        Image checkedIm = {
                .data = payload.data,
                .width = static_cast<int>(payload.width),
                .height = static_cast<int>(payload.height),
                .mipmaps = 1,
                .format = format
        };

        auto tex = LoadTextureFromImage(checkedIm);

        UnloadImage(checkedIm);

        if (mModel.has_value())
            mModel.value().materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;
    }
};