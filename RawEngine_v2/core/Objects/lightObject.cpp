//
// Created by boydv on 01/11/2026.
//
#include "lightObject.h"
namespace object {

    core::Model model = core::Model(0);

    LightObject::LightObject(const unsigned int& lightProgram, const unsigned int& modelProgram, const std::string& name) {
        this->modelProgram = modelProgram;
        this->lightProgram = lightProgram;
        this->name = name;

        mvpMatrixUniform = glGetUniformLocation(modelProgram, "mvpMatrix");
        modelMatrixUniform = glGetUniformLocation(modelProgram, "modelMatrix");

        lightColor = glm::vec3(1,1,1);
        ambientIntensity = 0.2f;
        quadraticAttenuation = 1.0f;

        lightPosUniform = glGetUniformLocation(lightProgram, "lightPos");
        lightingModelMatrixUniform = glGetUniformLocation(lightProgram, "modelMatrix");
        lightingMvpMatrixUniform = glGetUniformLocation(lightProgram, "mvpMatrix");
        lightColorUniform = glGetUniformLocation(lightProgram, "lightColor");
        cameraPosUniform = glGetUniformLocation(lightProgram, "cameraPos");
        ambientIntensityUniform = glGetUniformLocation(lightProgram, "ambientIntensity");
        attenuationUniform = glGetUniformLocation(lightProgram, "quadraticAttenuation");

        core::Mesh lightQuad = core::Mesh::generateQuad();
        model = core::Model({lightQuad});

        transform.SetTransMatrix(model.getModelMatrix());
        transform.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
        objPos = transform.GetPosition();
    }

    LightObject::LightObject() {
        lightColor = glm::vec3(1,1,1);
        ambientIntensity = 0.2f;
        quadraticAttenuation = 1.0f;
    }
    
    void LightObject::Update(const glm::mat4& view, const glm::mat4& projection, Camera camera) {
        model.setModelMatrix(transform.GetTransMatrix());

        glUseProgram(lightProgram);
        glUniform3f(lightPosUniform, transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
        glUniform3f(cameraPosUniform, camera.transform.GetPosition().x, camera.transform.GetPosition().y, camera.transform.GetPosition().z);
        glUniform3f(lightColorUniform,  lightColor.x,  lightColor.y,  lightColor.z);
        glUniform1f(ambientIntensityUniform,  ambientIntensity);
        glUniform1f(attenuationUniform, quadraticAttenuation);

        glUseProgram(modelProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * model.getModelMatrix()));
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(model.getModelMatrix()));
        model.render();
        glBindVertexArray(0);
    }

    void LightObject::Gui() {
        ImGui::PushID(this);
        ImGui::SeparatorText(name.c_str());
        if (ImGui::DragFloat3("Light Position", &objPos.x, 0.1f)) {
            transform.SetPosition(objPos);
        }
        ImGui::ColorEdit3("Light Color", &lightColor.x);
        ImGui::Text("Ambient");
        ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.001f, 1.0f);
        ImGui::Text("Attenuation");
        ImGui::SliderFloat("Attenuation", &quadraticAttenuation, 0.001f, 20.0f);
        ImGui::PopID();
    }
    LightObject::~LightObject() {}
}
