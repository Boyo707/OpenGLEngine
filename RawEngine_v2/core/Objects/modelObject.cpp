//
// Created by boydv on 07/11/2026.
//

#include "modelObject.h"



namespace object {
    //make a list of pointers for the model.
    ModelObject::ModelObject() {

    }

    ModelObject::ModelObject(const unsigned int& lightProgram, const std::string& objectName) {
        shaderProgram = lightProgram;
        name = objectName;

        objRot = glm::vec3(0,0,0);
        objScale = glm::vec3(1,1,1);
        objectColor = glm::vec3(1,0,0);;

        lightingModelMatrixUniform = glGetUniformLocation(lightProgram, "modelMatrix");
        lightingMvpMatrixUniform = glGetUniformLocation(lightProgram, "mvpMatrix");
        objectColorUniform = glGetUniformLocation(lightProgram, "objectColor");

        model = new core::Model(core::AssimpLoader::loadModel("models/nonormalmonkey.obj"));
        transform.SetTransMatrix(model->getModelMatrix());
    }

    void ModelObject::Update(const glm::mat4& view,const glm::mat4& projection, Camera camera) {
        objPos = transform.GetPosition();
        objRot = transform.GetRotation();
        objScale = transform.GetScale();
        model->setModelMatrix(transform.GetTransMatrix());

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(lightingMvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * model->getModelMatrix()));
        glUniformMatrix4fv(lightingModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(model->getModelMatrix()));
        glUniform3f(objectColorUniform,  objectColor.x,  objectColor.y,  objectColor.z);
        model->render();
        glBindVertexArray(0);
    }

    void ModelObject::Gui() {

        ImGui::PushID(this);
        ImGui::SeparatorText(name.c_str());

        if (ImGui::DragFloat3("Position", &objPos.x, 0.1f)) {
            printf("Changed Pos------------ \n");
            transform.SetPosition(objPos);
        }
        if (ImGui::DragFloat3("Rotation", &objRot.x, 0.1f)) {
            printf("Changed Rot------------ \n");
            transform.SetRotation(objRot);
        }
        if (ImGui::DragFloat3("Scale", &objScale.x, 0.1f)) {
            printf("Changed Scale------------ \n");
            transform.SetScale(objScale);
        }
        ImGui::ColorEdit3("Object Color", &objectColor.x);
        ImGui::PopID();
    }

    void ModelObject::Delete() {
        delete model;
    }
}
