//
// Created by boydv on 06/11/2026.
//
#pragma once

#include "scene.h"
#include "../assimpLoader.h"
#include "../model.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <unordered_map>

namespace scenes {

    class LightingScene : public Scene {

    public:
        void Update(float deltaTime, glm::vec3 cameraMoveDir, glm::vec3 cameraRotateDir, float cameraSpeed) override;
        void Start() override;
        void Gui() override;

        core::Model otherSuzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
        std::vector<unsigned int> shaders;

        LightingScene(std::vector<unsigned int> &shaders);

    };
}
