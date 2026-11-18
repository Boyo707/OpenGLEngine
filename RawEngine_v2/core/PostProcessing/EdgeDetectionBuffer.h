//
// Created by boydv on 18/11/2026.
//
#pragma once
#include "PostProcessItem.h"

namespace PostProcessing {
    class EdgeDetectionBuffer : public PostProcessItem {
    public:
        float edgeIntensity;
        GLint edgeIntensityUniform;

        EdgeDetectionBuffer(const int width, const int height, const unsigned int pBufferProgram);
        void RunProgram() override;
        void Gui() override;
        void CreateBuffer();
    };
}

