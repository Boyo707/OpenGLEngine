//
// Created by boydv on 06/11/2026.
//
#include "lightingScene.h"


#include "../Objects/lightObject.h"
#include "../Objects/modelObject.h"

namespace scenes {

    object::LightObject lightObject;

    object::ModelObject object;
    object::ModelObject object2;


    LightingScene::LightingScene(std::vector<unsigned int> &shaders) {
        this->shaders = shaders;
    }

    void LightingScene::Start() {
        Scene::Start();
        lightObject = object::LightObject(shaders.at(0), shaders.at(1), "light1");
        object = object::ModelObject(shaders.at(0), "TestObject");
        object2 = object::ModelObject(shaders.at(0), "TestObject2");

    }

    void LightingScene::Update(float deltaTime, glm::vec3 cameraMoveDir, glm::vec3 cameraRotateDir, float cameraSpeed) {
        Scene::Update(deltaTime, cameraMoveDir, cameraRotateDir, cameraSpeed);

        glm::mat4 view = glm::lookAt(sceneCamera.GetCameraPosition(), sceneCamera.GetTargetPosition(), sceneCamera.GetCameraUp());
        glm::mat4 projection = glm::perspective(glm::radians(sceneCamera.GetCameraFOV()), static_cast<float>(1400) / static_cast<float>(1000), 0.1f, 100.0f);

        object2.transform.SetRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.8f);

        object.Update(view, projection);
        object2.Update(view, projection);


        lightObject.UpdateLight(view, projection, sceneCamera.GetCameraPosition());
    }

    void LightingScene::Gui() {
        ImGui::Begin("Lighting Scene");
        ImGui::Text("Hello");

        object.Gui(0);
        object2.Gui(1);
        lightObject.Gui();
        ImGui::End();
    }
}
