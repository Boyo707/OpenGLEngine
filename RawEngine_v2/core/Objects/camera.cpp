//
// Created by boydv on 27/10/2026.
//
#include "camera.h"

namespace object {

    camera::camera() {

    }

    void camera::RotateCamera(glm::vec3 rotation) {
        targetTransform.SetRotation(rotation);
        targetTransform.SetPosition(transform.GetPosition() + targetTransform.GetForward() * targetDistance);
        transform.SetRotation(rotation);
    }

    void camera::MoveCamera(glm::vec3 direction) {

        transform.SetPosition(transform.GetPosition() + transform.GetRight() * direction.x + transform.GetUp() * direction.y + transform.GetForward() * direction.z);

        targetTransform.SetPosition(transform.GetPosition() + targetTransform.GetForward() * targetDistance);
    }

    void camera::SetFOV(float fov) {
        this->fov = fov;
    }

    glm::vec3 camera::GetCameraPosition() {
        return transform.GetPosition();
    }

    float camera::GetCameraFOV() {
        return fov;
    }

    glm::vec3 camera::GetTargetPosition() {
        return targetTransform.GetPosition();
    }

    glm::vec3 camera::GetCameraUp() {
        return transform.GetUp();
    }

    camera::~camera() {

    }

}