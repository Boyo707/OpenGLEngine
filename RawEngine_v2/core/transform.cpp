#include "transform.h"
#include <glm/ext/matrix_transform.hpp>

//
// Created by boydv on 27/10/2026.
//

namespace core {

    transform::transform() {
        transformMatrix = glm::mat4(1.0f);

        position = glm::vec3(0.0f,0.0f,0.0f);
        rotation = glm::vec3(0.0f,0.0f,0.0f);
    }

    void transform::SetTransMatrix(glm::mat4 matrix) {
        transformMatrix = matrix;
    }

    glm::mat4 transform::GetTransMatrix() {
        return transformMatrix;
    }

    void transform::SetPosition(glm::vec3 pPosition) {
        position = pPosition;
        ApplyChanges();
    }

    glm::vec3 transform::GetPosition() {
        return position;
    }

    void transform::SetRotation(glm::vec3 pAxis, float pRadians) {
        //needs to reset position to 0 later
        transformMatrix = glm::rotate(transformMatrix, glm::radians(pRadians), pAxis);

        //needs to place position back to origin
    }

    void transform::SetRotation(glm::vec3 pRotation) {
        rotation = pRotation;

        //forward = glm::vec3(0.0f, 0.0f, -1.0f);
        //right = glm::normalize(glm::cross( glm::vec3(0.0f, 1.0f, 0.0f), forward));
        //up = glm::cross(forward, right);

        ApplyChanges();
    }

    glm::vec3 transform::GetRotation() {
        return rotation;
    }

    glm::vec3 transform::GetRight() {
        return transformMatrix[0];
    }

    glm::vec3 transform::GetUp() {
        return transformMatrix[1];
    }

    glm::vec3 transform::GetForward() {
        return transformMatrix[2];
    }

    void transform::SetScale(glm::vec3 scale) {
        transformMatrix = glm::scale(transformMatrix, scale);
    }

    void transform::ApplyChanges() {
        glm::mat4 tempTrans = glm::mat4(1.0f);

        tempTrans = glm::rotate(tempTrans, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
        tempTrans = glm::rotate(tempTrans, glm::radians(rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));

        tempTrans = glm::translate(tempTrans, position);

        transformMatrix = tempTrans;
    }

    transform::~transform() {

    }
}