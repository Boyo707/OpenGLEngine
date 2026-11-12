//
// Created by boydv on 07/11/2026.
//

#pragma once
#include "object.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "../assimpLoader.h"
#include "imgui.h"


namespace object {
    class ModelObject : public Object{

    public:
        ModelObject();
        ModelObject(const unsigned int& lightProgram, const std::string& objectName);
        void Update(const glm::mat4& view, const glm::mat4& projection, Camera camera) override;
        void Gui() override;
        void Delete() override;

    private:
        unsigned int shaderProgram;
        GLint lightingModelMatrixUniform;
        GLint lightingMvpMatrixUniform;
        GLint objectColorUniform;

        glm::vec3 objectColor;
        core::Model* model;
    };
}