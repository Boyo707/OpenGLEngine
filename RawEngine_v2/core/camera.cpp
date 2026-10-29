//
// Created by boydv on 27/10/2026.
//
#include "camera.h"

namespace core {

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

    glm::vec3 camera::GetCameraPosition() {
        return transform.GetPosition();
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