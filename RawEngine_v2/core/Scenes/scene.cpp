//
// Created by boydv on 06/11/2026.
//
#include "scene.h"


namespace scenes {

    void Scene::Start() {
        hasStarted = true;
    }

    void Scene::Update(float deltaTime, glm::vec3 cameraMoveDir, glm::vec3 cameraRotateDir, float cameraSpeed) {
        if (!hasStarted) {
            Start();
        }

        sceneCamera.RotateCamera(cameraRotateDir);
        sceneCamera.MoveCamera(cameraMoveDir * cameraSpeed);
    }

    void Scene::Gui() {

    }

}