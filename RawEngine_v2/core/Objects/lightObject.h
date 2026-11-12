#pragma once
#include <glad/glad.h>
#include "../mesh.h"
#include "../model.h"
#include <vector>
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "object.h"

//
// Created by boydv on 01/11/2026.
//
namespace object {

    class LightObject : public Object{
    public:
        glm::vec3 lightColor;
        float ambientIntensity;
        glm::vec3 specularColor;
        float specularIntensity;
        float specularPower;
        float quadraticAttenuation;

        LightObject(const unsigned int& lightProgram,const unsigned int& modelProgram, const std::string& name);
        LightObject();
        ~LightObject();

        void Update(const glm::mat4& view,const glm::mat4& projection, Camera camera) override;
        void Gui() override;
        void Delete() override;

    private:
        unsigned int modelProgram;
        unsigned int lightProgram;
        GLint mvpMatrixUniform;
        GLint modelMatrixUniform;
        GLint lightPosUniform;
        GLint lightingModelMatrixUniform;
        GLint lightingMvpMatrixUniform;
        GLint lightColorUniform;
        GLint cameraPosUniform;
        GLint ambientIntensityUniform;
        GLint specularColorUniform;
        GLint specularIntensityUniform;
        GLint specularPowerUniform;
        GLint attenuationUniform;
    };
}
