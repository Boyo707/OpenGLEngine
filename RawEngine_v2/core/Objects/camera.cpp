//
// Created by boydv on 27/10/2026.
//
#include "camera.h"

namespace object {

    camera::camera() {

    }

    void camera::RotateCamera(glm::vec3 rotation) {
        transform.rotation = rotation;
        transform.Update();
    }

    void camera::MoveCamera(glm::vec3 direction) {

        transform.position = (transform.position + transform.GetRight() * direction.x + transform.GetUp() * direction.y + transform.GetForward() * direction.z);

        transform.Update();
    }

    void camera::SetFOV(float fov) {
        this->fov = fov;
    }

    float camera::GetCameraFOV() {
        return fov;
    }

    glm::vec3 camera::GetTargetPosition() {
        return transform.position + (transform.GetForward() * 2.0f);
    }


    camera::~camera() {

    }

}