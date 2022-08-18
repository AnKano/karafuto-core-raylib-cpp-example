#include "CustomCamera.hpp"
#include "glm/gtx/transform.hpp"

CustomCamera::CustomCamera
        (const glm::vec3 &position,
         const glm::vec3 &target,
         const glm::vec3 &up,
         const float &fovy) {
    mCamera = Camera{
            .position = {position.x, position.y, position.z},
            .target = {target.x, target.y, target.z},
            .up = {up.x, up.y, up.z},
            .fovy = fovy,
            .projection = CAMERA_PERSPECTIVE
    };
    SetCameraMode(mCamera, CAMERA_FREE);
}

void CustomCamera::update() {
    UpdateCamera(&mCamera);
}

glm::mat4 CustomCamera::getViewMatrix() const {
    auto camPos = glm::vec3(-mCamera.position.x, mCamera.position.y, mCamera.position.z);
    auto camTarget = glm::vec3(-mCamera.target.x, mCamera.target.y, mCamera.target.z);
    auto camUp = glm::vec3(-mCamera.up.x, mCamera.up.y, mCamera.up.z);

    return glm::lookAt(camPos, camTarget, camUp);
}

glm::mat4 CustomCamera::getProjectionMatrix(const float& aspectRatio) const {
    return glm::perspective(glm::radians(mCamera.fovy), aspectRatio, mCameraNear, mCameraFear);
}

const Camera & CustomCamera::getRawCamera() const {
    return mCamera;
}
