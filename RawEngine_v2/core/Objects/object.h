//
// Created by boydv on 12/11/2026.
//
#pragma once
#include <string>
#include "camera.h"
#include "../model.h"
#include "../transform.h"

namespace object {
    class Object {
    public:
        std::string name;
        core::Transform transform;
        virtual void Update(const glm::mat4& view,const glm::mat4& projection, Camera camera);
        virtual void Gui();
        virtual void Delete();
    protected:
        glm::vec3 objPos;
        glm::vec3 objRot;
        glm::vec3 objScale;
    };
}
