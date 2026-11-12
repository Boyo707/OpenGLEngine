//
// Created by boydv on 06/11/2026.
//
#include "lightingScene.h"



namespace scenes {
    LightingScene::LightingScene(const std::vector<unsigned int>& pShaders, object::Camera& pCamera) {
        shaders = pShaders;
        sceneCamera = pCamera;
    }

    void LightingScene::Start() {
        Scene::Start();
        lightObject = object::LightObject(shaders.at(0), shaders.at(1), "light1");
        object = object::ModelObject(shaders.at(0), "TestObject");
        object2 = object::ModelObject(shaders.at(0), "TestObject2");

        //CHANGE THE TRANSFORM FUNCTIONS TO JUST UPDATE.
        //Check every call if the variable has changed. -- DOne

        //maybe make a parent class for the objects, this includes the update. and start etc, all the base object things, maybe a transform?
        //will most likely still apply the same things as below with this idea.

        //Make a ImGui for the camera? - for FOV
        object.transform.SetPosition(glm::vec3(-2,0,0));
        object2.transform.SetPosition(glm::vec3(2,0,0));

        sceneObjects.push_back(&lightObject);
        sceneObjects.push_back(&object);
        sceneObjects.push_back(&object2);
    }

    void LightingScene::Update(float deltaTime) {
        Scene::Update(deltaTime);
        viewMatrix = glm::lookAt(sceneCamera.transform.GetPosition(), sceneCamera.GetTargetPosition(), sceneCamera.transform.GetUp());
        projectionMatrix = glm::perspective(glm::radians(sceneCamera.GetCameraFOV()), static_cast<float>(1400) / static_cast<float>(1000), 0.1f, 100.0f);

        object2.transform.RotateAround(glm::vec3(1.0f, 0.0f, 0.0f), 10 * deltaTime);
    }

    void LightingScene::Gui() {
        ImGui::Begin("Lighting Scene");
        for (int i = 0; i < sceneObjects.size(); ++i) {
            sceneObjects[i]->Gui();
        }
        ImGui::End();
    }

    LightingScene::~LightingScene() {
        for (int i = 0; i < sceneObjects.size(); ++i) {
            sceneObjects[i]->Delete();
        }
    }
}
