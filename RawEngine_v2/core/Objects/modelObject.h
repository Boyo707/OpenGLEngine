//
// Created by boydv on 07/11/2026.
//

#pragma once
#include "object.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "../assimpLoader.h"
#include "imgui.h"
#include "../texture.h"
#include "../Materials/material.h"


namespace object {
    class ModelObject : public Object{

    public:
        ModelObject();
        ModelObject(const unsigned int& lightProgram, const std::string& objectName);
        ModelObject(const unsigned int& lightProgram, const std::string& objectName, core::Model& pModel);
        ModelObject(const unsigned int& lightProgram, const std::string& objectName, core::Model& pModel, material::Material& pMaterial);
        void Update(const glm::mat4& view, const glm::mat4& projection, Camera camera) override;
        void Gui() override;
        void Delete() override;

    private:
        unsigned int shaderProgram;
        GLint lightingModelMatrixUniform;
        GLint lightingMvpMatrixUniform;
        GLint albedoColorUniform;
        GLint albedoTextureUniform;
        GLint normalMapUniform;
        GLint albedoBool;
        GLint normalBool;
        GLint specularColorUniform;
        GLint specularIntensityUniform;
        GLint specularPowerUniform;

        core::Model* model;
        material::Material* meshMaterial;
    };
}
