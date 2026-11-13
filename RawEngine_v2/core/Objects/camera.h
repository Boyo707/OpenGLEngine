//
// Created by boydv on 27/10/2026.
//
#pragma once
#include "../transform.h"


namespace object {

    class Camera {
    private:
        glm::vec3* moveDir;
        glm::vec3* rotateDir;
        float* speed;
        float* fov;
    public:
        core::Transform transform;

        void UpdateCamera();
        void SetFOV(float fov);

        float GetCameraFOV() const;
        glm::vec3 GetTargetPosition() const;

        Camera();
        Camera(glm::vec3& pMoveDir, glm::vec3& pRotateDir, float& pSpeed, float& pFOV);
        ~Camera();

    };
}

