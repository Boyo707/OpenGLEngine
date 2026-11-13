//
// Created by boydv on 06/11/2026.
//
#include "scene.h"


namespace scenes {
    Scene::Scene() {}

    void Scene::Start() {
        hasStarted = true;
    }

    void Scene::Update(float deltaTime) {
        if (!hasStarted) { Start(); }
        sceneCamera.UpdateCamera();

        for (int i = 0; i < sceneObjects.size(); ++i) {
            sceneObjects[i]->Update(viewMatrix, projectionMatrix, sceneCamera);
        }
    }

    void Scene::Gui() {}
}
