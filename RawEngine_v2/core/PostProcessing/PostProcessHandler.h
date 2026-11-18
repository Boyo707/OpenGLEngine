//
// Created by boydv on 18/11/2026.
//

#pragma once
#include <vector>

#include "ColorCorrectionBuffer.h"
#include "EdgeDetectionBuffer.h"
#include "../assimpLoader.h"

namespace PostProcessing {
    class PostProcessHandler {
    public:
        int width, height;
        unsigned int rbo;
        ColorCorrectionBuffer* colorCorrection;
        EdgeDetectionBuffer* edgeDetection;
        core::Model* quadModel;
        std::vector<unsigned int> shaders;
        std::vector<PostProcessItem*> postProcessItems;

        PostProcessHandler(std::vector<unsigned int> pPostProcessPrograms, const int pWidth, const int pHeight);
        ~PostProcessHandler();
        void FirstInitialization();
        void ProcessItems();
        void Gui();
    };
}
