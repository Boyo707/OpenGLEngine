#pragma once
#include "../transform.h"
#include <glad/glad.h>
#include "../mesh.h"
#include "../model.h"
#include <vector>
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>

//
// Created by boydv on 01/11/2026.
//
namespace object {

    class LightObject {
        private:
        unsigned int modelProgram;

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

        public:
        core::Transform transform;

        std::string name;

        glm::vec3 lightColor;

        float ambientIntensity;

        glm::vec3 specularColor;
        float specularIntensity;
        float specularPower;

        float quadraticAttenuation;

        void UpdateLight(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos);

        void Gui();


        LightObject(unsigned int lightProgram, unsigned int modelProgram, std::string name);
        LightObject();
        ~LightObject();
    };
}
