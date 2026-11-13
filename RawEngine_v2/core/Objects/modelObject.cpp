//
// Created by boydv on 07/11/2026.
//

#include "modelObject.h"

#include "../Materials/wallMaterial.h"


namespace object {
    ModelObject::ModelObject() {}

    ModelObject::ModelObject(const unsigned int& lightProgram, const std::string& objectName) {
        shaderProgram = lightProgram;
        name = objectName;

        objRot = glm::vec3(0,0,0);
        objScale = glm::vec3(1,1,1);

        lightingModelMatrixUniform = glGetUniformLocation(lightProgram, "modelMatrix");
        lightingMvpMatrixUniform = glGetUniformLocation(lightProgram, "mvpMatrix");
        albedoColorUniform = glGetUniformLocation(lightProgram, "objectColor");
        albedoTextureUniform = glGetUniformLocation(lightProgram, "albedoMap");
        normalMapUniform = glGetUniformLocation(lightProgram, "normalMap");
        albedoBool = glGetUniformLocation(lightProgram, "hasAlbedoMap");
        normalBool = glGetUniformLocation(lightProgram, "hasNormalMap");
        specularColorUniform = glGetUniformLocation(lightProgram, "specularColor");
        specularIntensityUniform = glGetUniformLocation(lightProgram, "specularIntensity");
        specularPowerUniform = glGetUniformLocation(lightProgram, "specularPower");

        model = new core::Model(core::AssimpLoader::loadModel("models/nonormalmonkey.obj"));
        meshMaterial = new material::Material();

        transform.SetTransMatrix(model->getModelMatrix());
    }
    ModelObject::ModelObject(const unsigned int& lightProgram, const std::string& objectName, core::Model& pModel) {
        shaderProgram = lightProgram;
        name = objectName;

        objRot = glm::vec3(0,0,0);
        objScale = glm::vec3(1,1,1);

        lightingModelMatrixUniform = glGetUniformLocation(lightProgram, "modelMatrix");
        lightingMvpMatrixUniform = glGetUniformLocation(lightProgram, "mvpMatrix");
        albedoColorUniform = glGetUniformLocation(lightProgram, "objectColor");
        albedoTextureUniform = glGetUniformLocation(lightProgram, "albedoMap");
        normalMapUniform = glGetUniformLocation(lightProgram, "normalMap");
        albedoBool = glGetUniformLocation(lightProgram, "hasAlbedoMap");
        normalBool = glGetUniformLocation(lightProgram, "hasNormalMap");
        specularColorUniform = glGetUniformLocation(lightProgram, "specularColor");
        specularIntensityUniform = glGetUniformLocation(lightProgram, "specularIntensity");
        specularPowerUniform = glGetUniformLocation(lightProgram, "specularPower");

        meshMaterial = new material::Material();
        model = &pModel;
        transform.SetTransMatrix(model->getModelMatrix());
    }

    ModelObject::ModelObject(const unsigned int& lightProgram, const std::string& objectName, core::Model& pModel, material::Material& pMaterial) {
        shaderProgram = lightProgram;
        name = objectName;

        objRot = glm::vec3(0,0,0);
        objScale = glm::vec3(1,1,1);

        lightingModelMatrixUniform = glGetUniformLocation(lightProgram, "modelMatrix");
        lightingMvpMatrixUniform = glGetUniformLocation(lightProgram, "mvpMatrix");
        albedoColorUniform = glGetUniformLocation(lightProgram, "objectColor");
        albedoTextureUniform = glGetUniformLocation(lightProgram, "albedoMap");
        normalMapUniform = glGetUniformLocation(lightProgram, "normalMap");
        albedoBool = glGetUniformLocation(lightProgram, "hasAlbedoMap");
        normalBool = glGetUniformLocation(lightProgram, "hasNormalMap");
        specularColorUniform = glGetUniformLocation(lightProgram, "specularColor");
        specularIntensityUniform = glGetUniformLocation(lightProgram, "specularIntensity");
        specularPowerUniform = glGetUniformLocation(lightProgram, "specularPower");

        meshMaterial = &pMaterial;
        model = &pModel;
        meshMaterial = &pMaterial;
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
        glUniform3f(albedoColorUniform,  meshMaterial->albedo.x,  meshMaterial->albedo.y,  meshMaterial->albedo.z);
        glUniform3f(specularColorUniform,  meshMaterial->specularColor.x, meshMaterial->specularColor.y, meshMaterial->specularColor.z);
        glUniform1f(specularIntensityUniform,  meshMaterial->specularIntensity);
        glUniform1f(specularPowerUniform, meshMaterial->specularPower);

        bool hasAlbedo = meshMaterial->albedoTexture != nullptr;
        bool hasNormal = meshMaterial->normalMap != nullptr;
        glUniform1i(albedoBool, hasAlbedo);
        glUniform1i(normalBool, hasNormal);
        if(hasAlbedo) {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(albedoTextureUniform, 0);
            glBindTexture(GL_TEXTURE_2D, meshMaterial->albedoTexture->getId());
        }
        if (hasNormal) {
            glActiveTexture(GL_TEXTURE1);
            glUniform1i(normalMapUniform, 1);
            glBindTexture(GL_TEXTURE_2D, meshMaterial->normalMap->getId());
        }
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
        meshMaterial->Gui();
        ImGui::PopID();
    }

    void ModelObject::Delete() {
        meshMaterial->Delete();
        delete meshMaterial;
        delete model;
    }
}
