//
// Created by boydv on 18/11/2026.
//

#include "ColorCorrectionBuffer.h"

#include "imgui.h"

namespace PostProcessing {

    ColorCorrectionBuffer::ColorCorrectionBuffer(const int width, const int height, const unsigned int pBufferProgram) : PostProcessItem(width, height) {
        name = "ColorCorrectionBuffer";
        bufferProgram = pBufferProgram;
        colors = glm::vec3(0.0f);
        canInvert = false;
        textureUniform = glGetUniformLocation(bufferProgram, "screenTexture");
        colorUniform = glGetUniformLocation(bufferProgram, "color");
        invertUniform = glGetUniformLocation(bufferProgram, "invert");
    }

    void ColorCorrectionBuffer::RunProgram() {
        glUseProgram(bufferProgram);
        glUniform4f(colorUniform, colors.x, colors.y, colors.z, 0.01f); // this could be a texture later.
        glUniform1i(invertUniform, canInvert);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(textureUniform, 1);
        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    }

    void ColorCorrectionBuffer::Gui() {
        ImGui::SeparatorText(name.c_str());
        ImGui::SliderFloat3("colors", &colors.x, 0.0f, 1.0f);
        ImGui::Checkbox("InvertColors", &canInvert);
    }
}
