//
// Created by boydv on 27/10/2026.
//
#include "camera.h"

namespace object {

    Camera::Camera() {}
    Camera::Camera(glm::vec3& pMoveDir, glm::vec3& pRotateDir, float& pSpeed, float& pFOV) {
        moveDir = &pMoveDir;
        rotateDir = &pRotateDir;
        speed = &pSpeed;
        fov = &pFOV;
    }

    void Camera::UpdateCamera() {
        glm::vec3 finalMoveDir = *moveDir * *speed;
        transform.SetRotation(*rotateDir);
        transform.SetPosition((transform.GetPosition() + transform.GetRight() * finalMoveDir.x + transform.GetUp() * finalMoveDir.y + transform.GetForward() * finalMoveDir.z));
    }

    float Camera::GetCameraFOV() const{
        return *fov;
    }

    glm::vec3 Camera::GetTargetPosition() const{
        return transform.GetPosition() + (transform.GetForward() * 2.0f);
    }

    Camera::~Camera() {

    }

}