//
// Created by boydv on 18/11/2026.
//

#include "EdgeDetectionBuffer.h"

#include <glm/vec3.hpp>

#include "imgui.h"

namespace PostProcessing {
    EdgeDetectionBuffer::EdgeDetectionBuffer(const int width, const int height, const unsigned int pBufferProgram) : PostProcessItem(width, height) {
        name = "edgeDetection";
        bufferProgram = pBufferProgram;
        edgeIntensity = 8.0f;
        edgeIntensityUniform = glGetUniformLocation(bufferProgram, "edgeIntensity");
        textureUniform = glGetUniformLocation(bufferProgram, "screenTexture");
    }

    void EdgeDetectionBuffer::RunProgram() {
        glUseProgram(bufferProgram);
        glUniform1f(edgeIntensityUniform, edgeIntensity);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(textureUniform, 1);
        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    }

    void EdgeDetectionBuffer::Gui() {
        ImGui::SeparatorText(name.c_str());
        ImGui::SliderFloat("Edge Intensity", &edgeIntensity, 0.0f, 24.0f);
    }
}
