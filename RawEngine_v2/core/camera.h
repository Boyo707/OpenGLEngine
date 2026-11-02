//
// Created by boydv on 27/10/2026.
//
#pragma once
#include "transform.h"


namespace core {

    class camera {
    private:
        core::Transform transform;
        core::Transform targetTransform;
        const float targetDistance = 1;


    public:

        void RotateCamera(glm::vec3 rotation);
        void MoveCamera(glm::vec3 position);

        glm::vec3 GetCameraPosition();
        glm::vec3 GetCameraUp();
        glm::vec3 GetTargetPosition();

        camera();
        ~camera();

    };
}

