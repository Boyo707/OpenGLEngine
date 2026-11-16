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
#include <unordered_map>

#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"
#include "core/Objects/camera.h"
#include "core/Objects/lightObject.h"
#include "core/Scenes/lightingScene.h"
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

int g_width = 1200;
int g_height = 1000;


glm::vec3 moveDir = glm::vec3(0, 0, 0);
glm::vec3 rotateDir = glm::vec3(0, 0, 0);

float FOV = 45.0f;

float cameraSpeed = 0;

bool firstMouse = true;
bool isLookingAround = false;
bool lockInCenter = false;


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraDefaultSpeed = 0.05f;
    const float cameraSprintSpeed = 0.08f;
    const glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);

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
        cameraSpeed = cameraDefaultSpeed + cameraSprintSpeed;
    }
    else {
        cameraSpeed = cameraDefaultSpeed;
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
    const GLuint edgeDetectionShader = generateShader("shaders/edgeDetection.fs", GL_FRAGMENT_SHADER);
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
    glAttachShader(frameBufferProgram, edgeDetectionShader);
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
    glDeleteShader(edgeDetectionShader);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model bufferQuad({quad});

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);

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

    //PostProcessing values
    glm::vec3 colors = glm::vec3(0.0f,0.0f,0.0f);
    bool canInvert = false;

    std::vector<unsigned int> shaderList;
    shaderList.push_back(lightModelShaderProgram);
    shaderList.push_back(modelShaderProgram);
    shaderList.push_back(textureShaderProgram);

    object::Camera camera = object::Camera(moveDir, rotateDir, cameraSpeed, FOV);
    scenes::LightingScene lightScene = scenes::LightingScene(shaderList, camera);

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        lightScene.Gui();

        //Eventually add more variables to change, or make a script that handles it all for me.
        ImGui::Begin("Post Processing Settings");
        ImGui::SliderFloat3("colors", &colors.x, 0.0f, 1.0f);
        ImGui::Checkbox("InvertColors", &canInvert);
        ImGui::Text("Hello :)");
        ImGui::End();

        processInput(window);

        //First pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, g_width, g_height);
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        //scene update - Eventually turn this into a list that i can cycle through instead. - Maybe add a starting camera pos & rot
        lightScene.Update(deltaTime);

        //Second pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glUseProgram(frameBufferProgram);
        glUniform4f(bufferColor, colors.x, colors.y, colors.z, 0.01f); // this could be a texture later.
        glUniform1i(bufferInvert, canInvert);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(bufferTextureUniform, 1);
        glBindTexture(GL_TEXTURE_2D, bufferTexture);
        bufferQuad.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}