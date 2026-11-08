#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>


namespace core {

    class Transform {
    private:
        glm::mat4 transformMatrix;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        void ApplyChanges();

    public:
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        void Update();

        void SetTransMatrix(glm::mat4 matrix);
        glm::mat4 GetTransMatrix();

        void SetPosition(glm::vec3 position);
        glm::vec3 GetPosition();

        void SetScale(glm::vec3 scale);
        glm::vec3 GetScale();

        void SetRotation(glm::vec3 axis, float radians);
        void SetRotation(glm::vec3 axis);
        glm::vec3 GetRotation();
        glm::vec3 GetForward();
        glm::vec3 GetUp();
        glm::vec3 GetRight();

        Transform();
        ~Transform();

    };
}

