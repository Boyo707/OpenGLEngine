//
// Created by boydv on 18/11/2026.
//
#include "PostProcessHandler.h"

#include "imgui.h"

namespace PostProcessing {

    PostProcessHandler::PostProcessHandler(std::vector<unsigned int> pPostProcessPrograms, const int pWidth, const int pHeight) {
        shaders = pPostProcessPrograms;
        width = pWidth;
        height = pHeight;
        //the order needs to be the same as the list
        colorCorrection = new ColorCorrectionBuffer(width, height, shaders[0]);
        edgeDetection = new EdgeDetectionBuffer(width, height, shaders[1]);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("uh oh");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //the order needs to be the same as the initialization
        postProcessItems.push_back(edgeDetection);
        postProcessItems.push_back(colorCorrection);

        core::Mesh quad = core::Mesh::generateQuad();
        quadModel = new core::Model({quad});
    }

    void PostProcessHandler::FirstInitialization() {
        postProcessItems[0]->InitBuffer();
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
    }

    void PostProcessHandler::ProcessItems() {
        for (int i = 0; i < postProcessItems.size(); i++) {
            if(i + 1 <= postProcessItems.size() - 1) {
                postProcessItems[i + 1]->InitBuffer();
            }
            else {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDisable(GL_DEPTH_TEST);
            }
            postProcessItems[i]->RunProgram();
            quadModel->render();
        }
    }

    void PostProcessHandler::Gui() {
        ImGui::Begin("Post Processing Settings");
        for (int i = 0; i < postProcessItems.size(); i++) {
            postProcessItems[i]->Gui();
        }
        ImGui::End();
    }

    PostProcessHandler::~PostProcessHandler() {
        glDeleteRenderbuffers(1, &rbo);
        for (int i = 0; i < postProcessItems.size(); i++) {
            glDeleteFramebuffers(1, &postProcessItems[i]->frameBuffer);
            delete postProcessItems[i];
        }
        delete quadModel;
    }
}
