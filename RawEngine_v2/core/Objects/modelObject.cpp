//
// Created by boydv on 07/11/2026.
//

#include "modelObject.h"



namespace object {
    GLint lightingModelMatrixUniform;
    GLint lightingMvpMatrixUniform;

    GLint objectColorUniform;

    core::Model myModel = core::Model(0);

    ModelObject::ModelObject() {

    }

    ModelObject::ModelObject(unsigned int lightProgram, std::string objectName) {
        shaderProgram = lightProgram;
        name = objectName;

        objPos = glm::vec3(0,0,0);
        objRot = glm::vec3(0,0,0);
        objScale = glm::vec3(1,1,1);

        lightingModelMatrixUniform = glGetUniformLocation(lightProgram, "modelMatrix");
        lightingMvpMatrixUniform = glGetUniformLocation(lightProgram, "mvpMatrix");

        objectColorUniform = glGetUniformLocation(lightProgram, "objectColor");

        objectColor = glm::vec3(1,0,0);;

        myModel = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");

        transform.SetTransMatrix(myModel.getModelMatrix());
    }

    void ModelObject::Update(glm::mat4 view, glm::mat4 projection) {

        transform.Update();
        myModel.setModelMatrix(transform.GetTransMatrix());
        //transform.Update();

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(lightingMvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * myModel.getModelMatrix()));
        glUniformMatrix4fv(lightingModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(myModel.getModelMatrix()));
        glUniform3f(objectColorUniform,  objectColor.x,  objectColor.y,  objectColor.z);
        myModel.render();
        glBindVertexArray(0);
    }

    void ModelObject::Gui(int id) {

        ImGui::PushID(this);
        ImGui::SeparatorText(name.c_str());

        ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
        ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.1f);
        ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
        ImGui::ColorEdit3("Object Color", &objectColor.x);

        ImGui::PopID();

    }
}
