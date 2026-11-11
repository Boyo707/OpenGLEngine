//
// Created by boydv on 27/10/2026.
//
#pragma once
#include "../transform.h"


namespace object {

    class camera {
    private:
        const float targetDistance = 1;
        float fov;

    public:
        core::Transform transform;

        void RotateCamera(glm::vec3 rotation);
        void MoveCamera(glm::vec3 position);
        void SetFOV(float fov);

        float GetCameraFOV();
        glm::vec3 GetTargetPosition();

        camera();
        ~camera();

    };
}

