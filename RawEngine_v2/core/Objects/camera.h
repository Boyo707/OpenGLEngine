//
// Created by boydv on 27/10/2026.
//
#pragma once
#include "../transform.h"
#include "imgui.h"


namespace object {

    class Camera {
    private:
        glm::vec3* moveDir;
        glm::vec3* rotateDir;
        float* speed;
        float* fov;

        glm::vec3 camPos;
        glm::vec3 camRot;
    public:
        core::Transform transform;

        float GetCameraFOV() const;
        glm::vec3 GetTargetPosition() const;

        Camera();
        Camera(glm::vec3& pMoveDir, glm::vec3& pRotateDir, float& pSpeed, float& pFOV);
        ~Camera();
        void UpdateCamera();
        void SetFOV(float fov);
        void Gui();
    };
}

