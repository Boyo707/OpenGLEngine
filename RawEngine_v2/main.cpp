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
#include "core/PostProcessing/PostProcessHandler.h"
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

    const unsigned int edgeDetectionBufferProgram = glCreateProgram();
    glAttachShader(edgeDetectionBufferProgram, bufferVertexShader);
    glAttachShader(edgeDetectionBufferProgram, edgeDetectionShader);
    glLinkProgram(edgeDetectionBufferProgram);
    glGetProgramiv(edgeDetectionBufferProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(edgeDetectionBufferProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    const unsigned int colorCorrectionBufferProgram = glCreateProgram();
    glAttachShader(colorCorrectionBufferProgram, bufferVertexShader);
    glAttachShader(colorCorrectionBufferProgram, bufferFragmentShader);
    glLinkProgram(colorCorrectionBufferProgram);
    glGetProgramiv(colorCorrectionBufferProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(colorCorrectionBufferProgram, 512, NULL, infoLog);
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

    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;

    std::vector<unsigned int> shaderList;
    shaderList.push_back(lightModelShaderProgram);
    shaderList.push_back(modelShaderProgram);
    shaderList.push_back(textureShaderProgram);

    object::Camera camera = object::Camera(moveDir, rotateDir, cameraSpeed, FOV);
    scenes::LightingScene lightScene = scenes::LightingScene(shaderList, camera);

    std::vector<unsigned int> ppPrograms;
    ppPrograms.push_back(colorCorrectionBufferProgram);
    ppPrograms.push_back(edgeDetectionBufferProgram);

    PostProcessing::PostProcessHandler ppHandler = PostProcessing::PostProcessHandler(ppPrograms, g_width, g_height);

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        lightScene.Gui();
        ppHandler.Gui();

        processInput(window);

        ppHandler.FirstInitialization();
        lightScene.Update(deltaTime);
        ppHandler.ProcessItems();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);
        currentTime = finishFrameTime;
    }

    glDeleteProgram(modelShaderProgram);
    glDeleteProgram(edgeDetectionBufferProgram);
    glDeleteProgram(colorCorrectionBufferProgram);
    glDeleteProgram(lightFragmentShader);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}