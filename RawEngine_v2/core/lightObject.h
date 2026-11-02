#pragma once
#include "transform.h"
#include <glad/glad.h>
#include "mesh.h"
#include "model.h"
#include <vector>

//
// Created by boydv on 01/11/2026.
//
namespace core {

    class LightObject {
        private:
        GLint lightPosUniform;
        GLint lightingModelMatrixUniform;
        GLint lightingMvpMatrixUniform;
        GLint lightColorUniform;
        GLint cameraPosUniform;
        GLint objectColorUniform;
        GLint ambientIntensityUniform;
        GLint specularColorUniform;
        GLint specularIntensityUniform;
        GLint specularPowerUniform;
        GLint attenuationUniform;

        void SetUpLight(unsigned int lightProgram);

        public:
        core::Transform transform;

        std::vector<int> test;

        glm::vec3 lightColor;
        glm::vec3 lightPosition;
        glm::vec3 objectColor;

        float ambientIntensity;

        glm::vec3 specularColor;
        float specularIntensity;
        float specularPower;

        float quadraticAttenuation;

        void UpdateLight(glm::vec3 cameraPos);



        LightObject(unsigned int lightProgram);
        ~LightObject();
    };
}
