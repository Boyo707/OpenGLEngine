//
// Created by boydv on 01/11/2026.
//
#include "lightObject.h"



namespace object {

    core::Model model = core::Model(0);

    GLint mvpMatrixUniform;
    GLint modelMatrixUniform;

    LightObject::LightObject(unsigned int lightProgram, unsigned int modelProgram, std::string name) {
        this->modelProgram = modelProgram;
        this->name = name;

        mvpMatrixUniform = glGetUniformLocation(modelProgram, "mvpMatrix");
        modelMatrixUniform = glGetUniformLocation(modelProgram, "modelMatrix");

        lightColor = glm::vec3(1,1,1);
        lightPosition = glm::vec3(0,0,0);

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
        ambientIntensityUniform = glGetUniformLocation(lightProgram, "ambientIntensity");
        specularColorUniform = glGetUniformLocation(lightProgram, "specularColor");
        specularIntensityUniform = glGetUniformLocation(lightProgram, "specularIntensity");
        specularPowerUniform = glGetUniformLocation(lightProgram, "specularPower");
        attenuationUniform = glGetUniformLocation(lightProgram, "quadraticAttenuation");

        core::Mesh lightQuad = core::Mesh::generateQuad();
        model = core::Model({lightQuad});


        transform.SetTransMatrix(model.getModelMatrix());
        transform.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

    }

    LightObject::LightObject() {

        lightColor = glm::vec3(1,1,1);
        lightPosition = glm::vec3(0,0,0);
        //objectColor = glm::vec3(1,0,0);;

        ambientIntensity = 0.2f;

        specularColor = glm::vec3(1,1,1);
        specularIntensity = 1.0f;
        specularPower = 3.0f;

        quadraticAttenuation = 1.0f;
    }
    
    void LightObject::UpdateLight(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos) {

        transform.SetPosition(lightPosition);
        model.setModelMatrix(transform.GetTransMatrix());

        glUniform3f(lightPosUniform, transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
        glUniform3f(cameraPosUniform, cameraPos.x, cameraPos.y, cameraPos.z);
        glUniform3f(lightColorUniform,  lightColor.x,  lightColor.y,  lightColor.z);
        glUniform1f(ambientIntensityUniform,  ambientIntensity);
        glUniform3f(specularColorUniform,  specularColor.x,  specularColor.y,  specularColor.z);
        glUniform1f(specularIntensityUniform,  specularIntensity);
        glUniform1f(specularPowerUniform, specularPower);
        glUniform1f(attenuationUniform, quadraticAttenuation);

        glUseProgram(modelProgram);

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * model.getModelMatrix()));
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(model.getModelMatrix()));
        model.render();
        glBindVertexArray(0);


    }

    void LightObject::Gui() {
        ImGui::SeparatorText(name.c_str());
        ImGui::Text("BaseColors");
        ImGui::DragFloat3("Light Position", &lightPosition.x, 0.1f);
        ImGui::ColorEdit3("Light Color", &lightColor.x);
        ImGui::Text("Ambient");
        ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.001f, 1.0f);

        ImGui::Text("Specular");
        ImGui::ColorEdit3("specularColor", &specularColor.x);
        ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.001f, 1.0f);
        ImGui::SliderFloat("Specular Power", &specularPower, 0.001f, 150.0f);

        ImGui::Text("Attenuation");
        ImGui::SliderFloat("Attenuation", &quadraticAttenuation, 0.001f, 20.0f);
    }

    LightObject::~LightObject() {

    }

}
