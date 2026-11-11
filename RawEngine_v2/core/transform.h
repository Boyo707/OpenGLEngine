#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>


namespace core {

    class Transform {
    private:
        glm::mat4 transformMatrix;

        void ApplyChanges();

    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        void Update();

        void SetTransMatrix(glm::mat4 matrix);
        glm::mat4 GetTransMatrix();

        void RotateAround(glm::vec3 axis, float radians);

        glm::vec3 GetForward();
        glm::vec3 GetUp();
        glm::vec3 GetRight();

        Transform();
        ~Transform();

    };
}

