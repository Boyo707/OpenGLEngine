//
// Created by boydv on 01/11/2026.
//
#include "lightObject.h"




namespace core {

    LightObject::LightObject(unsigned int lightProgram) {

        lightColor = glm::vec3(1,1,1);
        lightPosition = glm::vec3(0,0,0);
        objectColor = glm::vec3(1,0,0);;

        ambientIntensity = 0.2f;

        specularColor = glm::vec3(1,1,1);
        specularIntensity = 1.0f;
        specularPower = 3.0f;

        quadraticAttenuation = 1.0f;

        lightPosUniform = glGetUniformLocation(lightProgram, "lightPosition");
        lightingModelMatrixUniform = glGetUniformLocation(lightProgram, "modelMatrix");
        lightingMvpMatrixUniform = glGetUniformLocation(lightProgram, "mvpMatrix");
        lightColorUniform = glGetUniformLocation(lightProgram, "lightColor");
        cameraPosUniform = glGetUniformLocation(lightProgram, "cameraPos");
        objectColorUniform = glGetUniformLocation(lightProgram, "objectColor");
        ambientIntensityUniform = glGetUniformLocation(lightProgram, "ambientIntensity");
        specularColorUniform = glGetUniformLocation(lightProgram, "specularColor");
        specularIntensityUniform = glGetUniformLocation(lightProgram, "specularIntensity");
        specularPowerUniform = glGetUniformLocation(lightProgram, "specularPower");
        attenuationUniform = glGetUniformLocation(lightProgram, "quadraticAttenuation");
    }
    
    void LightObject::UpdateLight(glm::vec3 cameraPos) {

        glUniform3f(lightPosUniform, transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
        glUniform3f(cameraPosUniform, cameraPos.x, cameraPos.y, cameraPos.z);
        glUniform3f(lightColorUniform,  lightColor.x,  lightColor.y,  lightColor.z);
        glUniform3f(objectColorUniform,  objectColor.x,  objectColor.y,  objectColor.z);
        glUniform1f(ambientIntensityUniform,  ambientIntensity);
        glUniform3f(specularColorUniform,  specularColor.x,  specularColor.y,  specularColor.z);
        glUniform1f(specularIntensityUniform,  specularIntensity);
        glUniform1f(specularPowerUniform, specularPower);
        glUniform1f(attenuationUniform, quadraticAttenuation);
    }

    LightObject::~LightObject() {

    }

}
