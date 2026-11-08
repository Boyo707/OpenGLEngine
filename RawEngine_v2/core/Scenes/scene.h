//
// Created by boydv on 06/11/2026.
//
#pragma once
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include "../Objects/camera.h"
//need list/vector of objects
//need scene ID or names
//an inspector
//
namespace scenes {
    class Scene {

    public:
        std::string name;
        object::camera sceneCamera;

        std::vector<int> test;
        virtual void Start();
        bool hasStarted = false;

        virtual void Update(float deltaTime, glm::vec3 cameraMoveDir, glm::vec3 cameraRotateDir, float cameraSpeed);

        virtual void Gui();

    };
}