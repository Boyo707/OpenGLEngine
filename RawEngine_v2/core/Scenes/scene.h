//
// Created by boydv on 06/11/2026.
//
#pragma once
#include <vector>
#include <string>
#include "../Objects/camera.h"
#include "../Objects/object.h"
//need list/vector of objects
//an inspector
//
namespace scenes {
    class Scene {

    public:
        Scene();
        std::string name;
        object::Camera sceneCamera;
        std::vector<object::Object*> sceneObjects;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        bool hasStarted = false;

        virtual void Start();
        virtual void Update(float deltaTime);
        virtual void Gui();
    };
}