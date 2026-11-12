#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>


namespace core {
    class Transform {
    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 transformMatrix;

        void ApplyChanges();
    public:

        glm::mat4 GetTransMatrix() const;
        glm::vec3 GetPosition() const;
        glm::vec3 GetRotation() const;
        glm::vec3 GetScale() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;
        glm::vec3 GetForward() const;

        void SetTransMatrix(glm::mat4 matrix);
        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void RotateAround(glm::vec3 axis, float radians);
        void SetScale(glm::vec3 scale);
        void SetAll(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        void Update();
        Transform();
        ~Transform();

    };
}

