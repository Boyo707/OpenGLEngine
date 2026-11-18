//
// Created by boydv on 18/11/2026.
//

#include "PostProcessItem.h"


namespace PostProcessing {
    PostProcessItem::PostProcessItem(const int pWidth, const int pHeight) {
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glGenTextures(1, &frameBufferTexture);
        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pWidth, pHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
    }

    void PostProcessItem::InitBuffer( ) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void PostProcessItem::RunProgram() {
        printf("NoProgram is assigned to %s\n", name.c_str());
    }

    void PostProcessItem::Gui() {}
}
