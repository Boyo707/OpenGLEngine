#include "transform.h"

//
// Created by boydv on 27/10/2026.
//

namespace core {

    Transform::Transform() {
        transformMatrix = glm::mat4(1.0f);

        position = glm::vec3(0.0f,0.0f,0.0f);
        rotation = glm::vec3(0.0f,0.0f,0.0f);
        scale = glm::vec3(1.0f,1.0f,1.0f);

        //forward = glm::vec3(0.0f, 0.0f, 1.0f);
        //right = glm::normalize(glm::cross( glm::vec3(0.0f, 1.0f, 0.0f), forward));
        //up = glm::cross(forward, right);
    }

    void Transform::SetTransMatrix(glm::mat4 matrix) {
        transformMatrix = matrix;
    }

    glm::mat4 Transform::GetTransMatrix() {
        return transformMatrix;
    }


    void Transform::RotateAround(glm::vec3 pAxis, float pRotationDistance) {

        rotation += pAxis * pRotationDistance;

        ApplyChanges();
    }

    glm::vec3 Transform::GetRight() {
        return transformMatrix[0];
    }

    glm::vec3 Transform::GetUp() {
        return transformMatrix[1];
    }

    glm::vec3 Transform::GetForward() {
        return transformMatrix[2];
    }

    void Transform::ApplyChanges() {

        glm::mat4 tempTrans = glm::mat4(1.0f);

        //translate first before rotate.. or else it will rotate around a point that i moved away from.
        tempTrans = glm::translate(tempTrans, position);

        tempTrans = glm::rotate(tempTrans, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
        tempTrans = glm::rotate(tempTrans, glm::radians(rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));
        tempTrans = glm::rotate(tempTrans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        tempTrans = glm::scale(tempTrans, scale);

        transformMatrix = tempTrans;
    }

    void Transform::Update() {
        ApplyChanges();
    }

    Transform::~Transform() {

    }
}