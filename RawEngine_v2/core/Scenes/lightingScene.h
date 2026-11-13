//
// Created by boydv on 06/11/2026.
//
#pragma once

#include "scene.h"
#include "../assimpLoader.h"
#include "../model.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "../Objects/lightObject.h"
#include "../Objects/modelObject.h"

#include <unordered_map>

#include "../Materials/wallMaterial.h"

namespace scenes {

    class LightingScene : public Scene {
    public:
        LightingScene(const std::vector<unsigned int>& shaders, object::Camera& camera);
        ~LightingScene();

        void Update(float deltaTime) override;
        void Start() override;
        void Gui() override;

        std::vector<unsigned int> shaders;
        object::ModelObject object;
        object::ModelObject object2;
        object::ModelObject object3;
        object::LightObject lightObject;

        core::Model* quadModel;
        material::WallMaterial* wallMaterial;
    };
}
