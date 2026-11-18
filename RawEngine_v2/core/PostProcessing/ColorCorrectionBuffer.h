//
// Created by boydv on 18/11/2026.
//
#pragma once
#include <glm/vec3.hpp>

#include "PostProcessItem.h"

namespace PostProcessing {
    class ColorCorrectionBuffer : public PostProcessItem {
    public:
        ColorCorrectionBuffer(const int width, const int height, const unsigned int pBufferProgram);
        void RunProgram() override;
        void Gui() override;
    private:
        glm::vec3 colors;
        bool canInvert;
        GLint colorUniform;
        GLint invertUniform;
    };
}

