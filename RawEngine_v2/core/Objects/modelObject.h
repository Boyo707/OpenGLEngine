//
// Created by boydv on 07/11/2026.
//

#ifndef RAWENGINE_MODELOBJECT_H
#define RAWENGINE_MODELOBJECT_H
#include "../transform.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "lightObject.h"
#include "../assimpLoader.h"
#include "imgui.h"


namespace object {
    class ModelObject {

    public:
        std::string name;
        core::Transform transform;
        glm::vec3 objectColor;

        void Update(glm::mat4 view, glm::mat4 projection);
        ModelObject();
        ModelObject(unsigned int lightProgram, std::string objectName);

        void Gui(int id);

    private:
        glm::vec3 objPos;
        glm::vec3 objRot;
        glm::vec3 objScale;
        unsigned int shaderProgram;
    };
}


#endif //RAWENGINE_MODELOBJECT_H
