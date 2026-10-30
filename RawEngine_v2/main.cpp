#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"
#include "core/camera.h"
//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

int g_width = 800;
int g_height = 600;

core::camera camera;

glm::vec3 moveDir = glm::vec3(0, 0, 0);
glm::vec3 rotateDir = glm::vec3(0, 0, 0);

float FOV = 45.0f;

float sprintSpeed = 0.0f;

bool firstMouse = true;
bool isLookingAround = false;
bool lockInCenter = false;


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 inputDir = glm::vec3(0, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        inputDir += forward;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        inputDir += right;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        inputDir += up;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        inputDir -= forward;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        inputDir -= right;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        inputDir -= up;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        sprintSpeed = 0.08f;
    }
    else {
        sprintSpeed = 0.0f;
    }



    if (inputDir != glm::vec3(0, 0, 0)) {
        inputDir = glm::normalize(inputDir);
    }
    moveDir = inputDir;

    glm::vec3 rotDir = glm::vec3(0, 0, 0);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        isLookingAround = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
        isLookingAround = false;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        rotDir -= right;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        rotDir += right;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        rotDir += up;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        rotDir -= up;
    }

    rotateDir += rotDir;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        FOV += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        FOV -= 1.0f;
    }
    FOV = std::clamp(FOV, 1.0f, 150.0f);

}

float centerX;
float centerY;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (isLookingAround) {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            if (lockInCenter)
            {
                int windowHeight;
                int windowWidth;

                glfwGetWindowSize( window , &windowWidth, &windowHeight );

                centerX = windowWidth / 2;
                centerY = windowHeight / 2;
                firstMouse = false;
            }
            else {
                centerX = xpos;
                centerY = ypos;
                firstMouse = false;
            }
        }

        glm::vec2 mouseOffset;

        mouseOffset.x = centerX - xpos;
        mouseOffset.y = ypos - centerY;

        float sensitivity = 0.1f;
        mouseOffset.x *= sensitivity;
        mouseOffset.y *= sensitivity;

        rotateDir += glm::vec3(mouseOffset, 0.0f);


        glfwSetCursorPos(window,centerX , centerY);
    }
}

void framebufferSizeCallback(GLFWwindow *window,
                             int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
}

std::string readFileToString(const std::string &filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Could not open file: %s\n", filePath.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

GLuint generateShader(const std::string &shaderPath, GLuint shaderType) {
    printf("Loading shader: %s\n", shaderPath.c_str());
    const std::string shaderText = readFileToString(shaderPath);
    const GLuint shader = glCreateShader(shaderType);
    const char *s_str = shaderText.c_str();
    glShaderSource(shader, 1, &s_str, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
    }
    return shader;
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //Setup platforms
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLuint modelVertexShader = generateShader("shaders/modelVertex.vs", GL_VERTEX_SHADER);
    const GLuint fragmentShader = generateShader("shaders/fragment.fs", GL_FRAGMENT_SHADER);
    const GLuint textureShader = generateShader("shaders/texture.fs", GL_FRAGMENT_SHADER);
    const GLuint lightFragmentShader = generateShader("shaders/lightingFragment.fs", GL_FRAGMENT_SHADER);

    const GLuint bufferVertexShader = generateShader("shaders/bufferVertex.vs", GL_VERTEX_SHADER);
    const GLuint bufferFragmentShader = generateShader("shaders/bufferFragment.fs", GL_FRAGMENT_SHADER);


    int success;
    char infoLog[512];

    const unsigned int modelShaderProgram = glCreateProgram();
    glAttachShader(modelShaderProgram, modelVertexShader);
    glAttachShader(modelShaderProgram, fragmentShader);
    glLinkProgram(modelShaderProgram);
    glGetProgramiv(modelShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(modelShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    const unsigned int lightModelShaderProgram = glCreateProgram();
    glAttachShader(lightModelShaderProgram, modelVertexShader);
    glAttachShader(lightModelShaderProgram, lightFragmentShader);
    glLinkProgram(lightModelShaderProgram);
    glGetProgramiv(lightModelShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(lightModelShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    const unsigned int textureShaderProgram = glCreateProgram();
    glAttachShader(textureShaderProgram, modelVertexShader);
    glAttachShader(textureShaderProgram, textureShader);
    glLinkProgram(textureShaderProgram);
    glGetProgramiv(textureShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(textureShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    const unsigned int frameBufferProgram = glCreateProgram();
    glAttachShader(frameBufferProgram, bufferVertexShader);
    glAttachShader(frameBufferProgram, bufferFragmentShader);
    glLinkProgram(frameBufferProgram);
    glGetProgramiv(frameBufferProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(frameBufferProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glDeleteShader(modelVertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textureShader);
    glDeleteShader(lightFragmentShader);

    glDeleteShader(bufferFragmentShader);
    glDeleteShader(bufferVertexShader);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model quadModel({quad});
    quadModel.translate(glm::vec3(0,0,-5));
    quadModel.scale(glm::vec3(5, 5, 1));

    core::Mesh bufferQuad = core::Mesh::generateQuad();
    core::Model bufferQuadModel({bufferQuad});

    core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    suzanne.translate(glm::vec3(0, 0, -2));
    core::Model otherSuzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    otherSuzanne.translate(glm::vec3(0,0,0));
    //quadModel.scale(glm::vec3(5, 5, 1));

    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);


    float bufferQuadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int bufferQuadVAO, bufferQuadVBO;
    glGenVertexArrays(1, &bufferQuadVAO);
    glGenBuffers(1, &bufferQuadVBO);
    glBindVertexArray(bufferQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bufferQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bufferQuadVertices), &bufferQuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    glm::mat4 view = glm::lookAt(camera.GetCameraPosition(), camera.GetTargetPosition(), camera.GetCameraUp());
    glm::mat4 projection = glm::perspective(glm::radians(FOV), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

    GLint mvpMatrixUniform = glGetUniformLocation(modelShaderProgram, "mvpMatrix");
    GLint modelMatrixUniform = glGetUniformLocation(modelShaderProgram, "modelMatrix");
    GLint textureModelUniform = glGetUniformLocation(textureShaderProgram, "mvpMatrix");
    GLint textureUniform = glGetUniformLocation(textureShaderProgram, "text");


    GLint lightPosUniform = glGetUniformLocation(lightModelShaderProgram, "lightPosition");
    GLint lightColorUniform = glGetUniformLocation(lightModelShaderProgram, "lightColor");
    GLint cameraPosUniform = glGetUniformLocation(lightModelShaderProgram, "cameraPos");
    GLint objectColorUniform = glGetUniformLocation(lightModelShaderProgram, "objectColor");
    GLint ambientIntensityUniform = glGetUniformLocation(lightModelShaderProgram, "ambientIntensity");
    GLint specularColorUniform = glGetUniformLocation(lightModelShaderProgram, "specularColor");
    GLint specularIntensityUniform = glGetUniformLocation(lightModelShaderProgram, "specularIntensity");
    GLint specularPowerUniform = glGetUniformLocation(lightModelShaderProgram, "specularPower");
    GLint attenuationUniform = glGetUniformLocation(lightModelShaderProgram, "quadraticAttenuation");

    GLint bufferTextureUniform = glGetUniformLocation(frameBufferProgram, "screenTexture");
    GLint bufferColor = glGetUniformLocation(frameBufferProgram, "color");
    GLint bufferInvert = glGetUniformLocation(frameBufferProgram, "invert");

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int bufferTexture;
    glGenTextures(1, &bufferTexture);
    glBindTexture(GL_TEXTURE_2D, bufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTexture, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("uh oh");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float rotationStrength = 100.0f;

    //PostProcessing values
    glm::vec3 colors = glm::vec3(0.0f,0.0f,0.0f);
    bool canInvert = false;

    glm::vec3 lightPos = glm::vec3(0.0f,0.0f,-5.0f);
    glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f,1.0f,1.0f);
    float ambIntensity = 0.2f;
    float specularIntensity = 1.0f;
    float specularPower = 3.0f;
    float attenuation = 1.0f;

    while (!glfwWindowShouldClose(window)) {


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        ImGui::Begin("Post Processing Settings");

        ImGui::SliderFloat3("colors", &colors.x, 0.0f, 1.0f);
        ImGui::Checkbox("InvertColors", &canInvert);

        ImGui::Text("Hello :)");

        ImGui::End();

        ImGui::Begin("Lighting Settings");
        ImGui::Text("BaseColors");
        ImGui::SliderFloat3("Light Position", &lightPos.x, -2.0f, 2.0f);
        ImGui::ColorEdit3("Light Color", &lightColor.x);
        ImGui::ColorEdit3("Object Color", &objectColor.x);
        ImGui::Text("Ambient");
        ImGui::SliderFloat("Ambient Intensity", &ambIntensity, 0.001f, 1.0f);

        ImGui::Text("Specular");
        ImGui::ColorEdit3("specularColor", &specularColor.x);
        ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.001f, 1.0f);
        ImGui::SliderFloat("Specular Power", &specularPower, 0.001f, 150.0f);

        ImGui::Text("Attenuation");
        ImGui::SliderFloat("Attenuation", &attenuation, 0.001f, 20.0f);


        ImGui::End();
        ImGui::EndFrame();



        processInput(window);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));
        otherSuzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));

        float speed = 0.05f + sprintSpeed;

        camera.RotateCamera(rotateDir);
        camera.MoveCamera(moveDir * speed);

        view = glm::lookAt(camera.GetCameraPosition(), camera.GetTargetPosition(), camera.GetCameraUp());
        projection = glm::perspective(glm::radians(FOV), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

        //CatTextureQuad----------------------------------------------------------------
        glUseProgram(textureShaderProgram);
        glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(projection * view * quadModel.getModelMatrix()));
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniform, 0);
        glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        quadModel.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        //SuzaneModel----------------------------------------------------------------
        glUseProgram(modelShaderProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * suzanne.getModelMatrix()));
        suzanne.render();
        glBindVertexArray(0);

        glm::vec3 cameraPos = camera.GetCameraPosition();
        //LightingSuzaneModel
        glUseProgram(lightModelShaderProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * otherSuzanne.getModelMatrix()));
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(otherSuzanne.getModelMatrix()));
        glUniform3f(lightPosUniform, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(cameraPosUniform, cameraPos.x, cameraPos.y, cameraPos.z);
        glUniform3f(lightColorUniform, lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(objectColorUniform, objectColor.x, objectColor.y, objectColor.z);
        glUniform1f(ambientIntensityUniform, ambIntensity);
        glUniform3f(specularColorUniform, specularColor.x, specularColor.y, specularColor.z);
        glUniform1f(specularIntensityUniform, specularIntensity);
        glUniform1f(specularPowerUniform, specularPower);
        glUniform1f(attenuationUniform, attenuation);



        otherSuzanne.render();
        glBindVertexArray(0);


        //FrameBuffer----------------------------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        //Maybe try and turn this into a better looking program (make use of the uniform location of texture)
        glUseProgram(frameBufferProgram);
        glUniform4f(bufferColor, colors.x, colors.y, colors.z, 0.01f);
        glUniform1i(bufferInvert, canInvert);
        glBindVertexArray(bufferQuadVAO); //Can't use the quad mesh provided since it doesnt give texcoords in the attribute pointer
        glBindTexture(GL_TEXTURE_2D, bufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /*glUseProgram(frameBufferProgram);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(bufferTextureUniform, 0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        bufferQuad.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);*/


        glfwSwapBuffers(window);
        glfwPollEvents();

        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);
        currentTime = finishFrameTime;
    }

    glDeleteProgram(modelShaderProgram);

    glDeleteProgram(frameBufferProgram);
    glDeleteProgram(lightFragmentShader);
    glDeleteFramebuffers(1, &bufferTexture);
    glDeleteRenderbuffers(1, &rbo);

    glDeleteVertexArrays(1, &bufferQuadVAO);
    glDeleteBuffers(1, &bufferQuadVBO);


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}