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
    }

    void Transform::SetTransMatrix(glm::mat4 matrix) {
        transformMatrix = matrix;
    }

    glm::mat4 Transform::GetTransMatrix() {
        return transformMatrix;
    }

    void Transform::SetPosition(glm::vec3 pPosition) {
        position = pPosition;
        ApplyChanges();
    }

    glm::vec3 Transform::GetPosition() {
        return position;
    }

    void Transform::SetRotation(glm::vec3 pAxis, float pRotationDistance) {

        rotation += pAxis * pRotationDistance;

        ApplyChanges();

        //transformMatrix = glm::rotate(transformMatrix, glm::radians(pRadians), pAxis);
    }

    void Transform::SetRotation(glm::vec3 pRotation) {
        rotation = pRotation;

        //forward = glm::vec3(0.0f, 0.0f, -1.0f);
        //right = glm::normalize(glm::cross( glm::vec3(0.0f, 1.0f, 0.0f), forward));
        //up = glm::cross(forward, right);

        ApplyChanges();
    }

    glm::vec3 Transform::GetRotation() {
        return rotation;
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

    void Transform::SetScale(glm::vec3 pScale) {
        scale = pScale;
        ApplyChanges();
    }

    glm::vec3 Transform::GetScale() {
        return scale;
    }

    void Transform::ApplyChanges() {
        glm::mat4 tempTrans = glm::mat4(1.0f);

        tempTrans = glm::rotate(tempTrans, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
        tempTrans = glm::rotate(tempTrans, glm::radians(rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));

        tempTrans = glm::translate(tempTrans, position);

        tempTrans = glm::scale(tempTrans, scale);

        transformMatrix = tempTrans;
    }

    void Transform::Update() {
        glm::vec3 matPos = glm::vec3(transformMatrix[3].x, transformMatrix[3].y, transformMatrix[3].z);
        //printf("Matrix: %f, %f, %f", matPos.x, matPos.y, matPos.z);
        //printf("normal Pos: %f, %f, %f", position.x, position.y, position.z);
    }

    Transform::~Transform() {

    }
}