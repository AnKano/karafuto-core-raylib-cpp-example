#pragma once

#include "glm/glm.hpp"
#include "raylib.h"

class CustomCamera {
private:
    static constexpr float mCameraNear = 0.1f;
    static constexpr float mCameraFear = 100.0f;

    Camera mCamera;
public:
    CustomCamera(const glm::vec3 &position,
                 const glm::vec3 &target,
                 const glm::vec3 &up,
                 const float &fovy);

    void update();

    [[nodiscard]] const Camera &getRawCamera() const;

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::mat4 getProjectionMatrix(const float &aspectRatio) const;
};