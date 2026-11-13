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

    glm::mat4 Transform::GetTransMatrix() const {
        return transformMatrix;
    }

    glm::vec3 Transform::GetPosition() const {
        return position;
    }

    glm::vec3 Transform::GetRotation() const {
        return rotation;
    }

    glm::vec3 Transform::GetScale() const {
        return scale;
    }

    glm::vec3 Transform::GetRight() const {
        return transformMatrix[0];
    }

    glm::vec3 Transform::GetUp() const {
        return transformMatrix[1];
    }

    glm::vec3 Transform::GetForward() const {
        return transformMatrix[2];
    }

    void Transform::SetTransMatrix(glm::mat4 pMatrix) {
        transformMatrix = pMatrix;
    }

    void Transform::SetPosition(glm::vec3 pPosition) {
        position = pPosition;
        ApplyChanges();
    }

    void Transform::SetRotation(glm::vec3 pRotation) {
        rotation = pRotation;
        ApplyChanges();
    }

    void Transform::RotateAround(glm::vec3 pAxis, float pRotationDistance) {
        rotation += pAxis * pRotationDistance;
        ApplyChanges();
    }

    void Transform::SetScale(glm::vec3 pScale) {
        scale = pScale;
        ApplyChanges();
    }

    void Transform::SetAll(glm::vec3 pPosition, glm::vec3 pRotation, glm::vec3 pScale) {
        position = pPosition;
        rotation = pRotation;
        scale = pScale;
        ApplyChanges();
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