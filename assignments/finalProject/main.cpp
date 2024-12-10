// BOAT MODEL LICENSE
/* Model Information:
* title:	Gislinge Viking Boat
* source:	https://sketchfab.com/3d-models/gislinge-viking-boat-01098ad7973647a9b558f41d2ebc5193
* author:	Opus Poly (https://sketchfab.com/OpusPoly)
Model License:
* license type:	CC-BY-4.0 (http://creativecommons.org/licenses/by/4.0/)
* requirements:	Author must be credited. Commercial use is allowed.
If you use this 3D model in your project be sure to copy paste this credit wherever you share it:
This work is based on "Gislinge Viking Boat" (https://sketchfab.com/3d-models/gislinge-viking-boat-01098ad7973647a9b558f41d2ebc5193) by Opus Poly (https://sketchfab.com/OpusPoly) licensed under CC-BY-4.0 (http://creativecommons.org/licenses/by/4.0/)*/

// CUBEMAP SOURCE
// https://opengameart.org/content/sky-box-sunny-day

#include <stdio.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <caleb/shader.hpp>
#include <caleb/texture.hpp>
#include <caleb/camera.hpp>
#include <caleb/model.hpp>
#include <ew/external/glad.h>
#include <ew/external/stb_image.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffSet, double yOffSet);
void processInput(GLFWwindow* window);

unsigned int loadTexture(const char* path);
unsigned int loadCubemap(std::vector<std::string> faces);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// Written by Caleb using OpenGL Tutorial
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseActive = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool boatActive = true;
float boatScale = 0.125f;
float rockingAngleIntensity = 2.0f;
float rockingHeightIntensity = 2.0f;
float rockingAngle = 0.0f;
float rockingHeight = 0.0f;

bool cubemapActive = true;

int main() {
    printf("Initializing...");
    if (!glfwInit()) {
        printf("GLFW failed to init!");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Project: High Seas", NULL, NULL);
    if (window == NULL) {
        printf("GLFW failed to create window");
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Cubemap Written by Caleb using OpenGL Tutorials
    Shader skyboxShader("assets/skyboxVertexShader.vert", "assets/skyboxFragmentShader.frag");

    float cubemapVerts[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    unsigned int cubemapVAO, cubemapVBO;
    glGenVertexArrays(1, &cubemapVAO);
    glGenBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVerts), &cubemapVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> cubeFaces = {
        "assets/cubemap/right.jpg", "assets/cubemap/left.jpg", "assets/cubemap/top.jpg",
        "assets/cubemap/bottom.jpg", "assets/cubemap/front.jpg", "assets/cubemap/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(cubeFaces);
    skyboxShader.use();
    skyboxShader.setInt("cubemap", 0);

    // Boat model setup
    stbi_set_flip_vertically_on_load(true);
    Shader boatShader("assets/boatVertexShader.vert", "assets/boatFragmentShader.frag");
    Model boatModel("assets/boatAssets/boat.obj");

    // Ocean floor setup
    Shader floorShader("assets/oceanVertexShader.vert", "assets/oceanFragmentShader.frag");
    unsigned int floorTexture = loadTexture("assets/ocean_floor_texture.jpg");
    float floorVertices[] = {
    // positions           // texture coordinates
    -10.0f, 4.0f,  10.0f,  0.0f, 0.0f, 
     10.0f, 4.0f,  10.0f,  1.0f, 0.0f,
     10.0f, 4.0f, -10.0f,  1.0f, 1.0f,
    -10.0f, 4.0f, -10.0f,  0.0f, 1.0f
};

    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        rockingAngle = rockingAngleIntensity * sin(currentFrame);
        rockingHeight = (rockingHeightIntensity * sin(currentFrame)) / 100.0f;

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the view and projection matrices
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Render ocean floor by JP
        floorShader.use();
        floorShader.setMat4("projection", projection);
        floorShader.setMat4("view", view);
        floorShader.setInt("floorTexture", 0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glm::mat4 floorModel = glm::mat4(1.0f);
        floorModel = glm::translate(floorModel, glm::vec3(0.0f, -0.25f - rockingHeight, 0.0f));
        floorShader.setMat4("model", floorModel);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


 


        //Written by Caleb using OpenGL Tutorial
        if (boatActive)
        {
            boatShader.use();
            glm::mat4 view = cam.GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
            boatShader.setMat4("proj", projection);
            boatShader.setMat4("view", view);
            glm::mat4 boatVertModel = glm::mat4(1.0f);
            boatVertModel = glm::translate(boatVertModel, glm::vec3(0.0f, -(rockingHeight), 0.0f));
            boatVertModel = glm::rotate(boatVertModel, glm::radians(rockingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
            boatVertModel = glm::scale(boatVertModel, glm::vec3(boatScale, boatScale, boatScale));
            boatShader.setMat4("model", boatVertModel);
            boatModel.Draw(boatShader);
        }

        // Written by Caleb using OpenGL Tutorial
        if (cubemapActive)
        {
            glDepthFunc(GL_LEQUAL);
            skyboxShader.use();
            glm::mat4 cubemapView = glm::mat4(glm::mat3(cam.GetViewMatrix()));
            glm::mat4 cubemapProjection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
            skyboxShader.setMat4("view", cubemapView);
            skyboxShader.setMat4("proj", cubemapProjection);
            glBindVertexArray(cubemapVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // Boat & Cubemap Settings Written by Caleb
        ImGui::Begin("Settings");
        ImGui::Checkbox("Boat", &boatActive);
        ImGui::SliderFloat("Boat Rock Angle Intensity", &rockingAngleIntensity, 0.25f, 50.0f);
        ImGui::SliderFloat("Boat Rock Height Intensity", &rockingHeightIntensity, 0.25f, 250.0f);
        ImGui::Checkbox("Cubemap", &cubemapActive);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Written by Caleb using OpenGL Tutorial
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Written by Caleb using OpenGL Tutorial
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = static_cast<float>(xPos);
        lastY = static_cast<float>(yPos);
        firstMouse = false;
    }

    float xOffset = static_cast<float>(xPos) - lastX;
    float yOffset = lastY - static_cast<float>(yPos);
    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);

    if (mouseActive)
        cam.ProcessMouseMovement(xOffset, yOffset);
}

// Written by Caleb using OpenGL Tutorial
void scroll_callback(GLFWwindow* window, double xOffSet, double yOffSet) {
    cam.ProcessMouseScroll(static_cast<float>(yOffSet));
}

// Written by Caleb using OpenGL Tutorial
void processInput(GLFWwindow* window) {
    bool isSprinting = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        isSprinting = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, deltaTime, isSprinting);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARD, deltaTime, isSprinting);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(LEFT, deltaTime, isSprinting);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(RIGHT, deltaTime, isSprinting);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.ProcessKeyboard(UP, deltaTime, isSprinting);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.ProcessKeyboard(DOWN, deltaTime, isSprinting);

    if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseActive = false;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseActive = true;
    }
}

// Written by Caleb using OpenGL Tutorial
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    return textureID;
}

// Written by Caleb using OpenGL Tutorial
unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            printf("Cubemap texture failed to load at path: %s\n", faces[i].c_str());
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}
