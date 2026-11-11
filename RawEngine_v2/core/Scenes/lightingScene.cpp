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


        //CHANGE THE TRANSFORM FUNCTIONS TO JUST UPDATE.
        //Check every call if the variable has changed. -- DOne

        //maybe make a parent class for the objects, this includes the update. and start etc, all the base object things, maybe a transform?
        //will most likely still apply the same things as below with this idea.

        //Make a ImGui for the camera? - for FOV
        object.transform.position = glm::vec3(-2,0,0);
        object2.transform.position = glm::vec3(2,0,0);
    }

    void LightingScene::Update(float deltaTime, glm::vec3 cameraMoveDir, glm::vec3 cameraRotateDir, float cameraSpeed) {
        Scene::Update(deltaTime, cameraMoveDir, cameraRotateDir, cameraSpeed);

        glm::mat4 view = glm::lookAt(sceneCamera.transform.position, sceneCamera.GetTargetPosition(), sceneCamera.transform.GetUp());
        glm::mat4 projection = glm::perspective(glm::radians(sceneCamera.GetCameraFOV()), static_cast<float>(1400) / static_cast<float>(1000), 0.1f, 100.0f);

        object2.transform.RotateAround(glm::vec3(1.0f, 0.0f, 0.0f), 10 * deltaTime);

        object.Update(view, projection);
        object2.Update(view, projection);


        lightObject.UpdateLight(view, projection, sceneCamera.transform.position);
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
