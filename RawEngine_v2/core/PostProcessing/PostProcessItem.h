//
// Created by boydv on 18/11/2026.
//

#pragma once
#include <vcruntime.h>
#include <glad/glad.h>
#include <cstdio>
#include <string>

namespace PostProcessing {
    class PostProcessItem {
    public:
        std::string name;
        unsigned int frameBuffer;
        PostProcessItem(const int width, const int height);
        void InitBuffer();
        virtual void RunProgram();
        virtual void Gui();
    protected:
        unsigned int frameBufferTexture;
        unsigned int rbo;
        unsigned int bufferProgram;
        GLint textureUniform;
    };
}
