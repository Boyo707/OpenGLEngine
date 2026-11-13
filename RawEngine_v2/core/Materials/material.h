//
// Created by boydv on 13/11/2026.
//
#pragma once
#include <glm/vec4.hpp>
#include "imgui.h"
#include "../texture.h"
#include <glm/vec3.hpp>

namespace material {
    class Material {
    public:
        std::string name;
        glm::vec4 albedo;
        glm::vec3 specularColor;
        float specularIntensity;
        float specularPower;
        core::Texture* albedoTexture;
        core::Texture* normalMap;

        Material();
        virtual void Gui();
        virtual void Delete();
    };
}
