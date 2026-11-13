#include "material.h"


//
// Created by boydv on 13/11/2026.
//
namespace material {
    Material::Material() {
        name = "defaultMat";
        albedo = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        specularIntensity = 1.0f;
        specularPower = 3.0f;

        albedoTexture = nullptr;
        normalMap = nullptr;
    }

    void Material::Gui() {
        ImGui::PushID(this);
        std::string guiName = name + " Settings";
        if (ImGui::TreeNode(guiName.c_str())) {
            ImGui::ColorEdit4("Albedo", &albedo.x);
            ImGui::ColorEdit4("Specular Color", &specularColor.x);
            ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.001f, 1.0f);
            ImGui::SliderFloat("Specular Power", &specularPower, 0.001f, 150.0f);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    void Material::Delete() {
        delete albedoTexture;
        delete normalMap;
    }
}