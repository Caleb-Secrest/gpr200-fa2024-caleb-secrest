// BOAT MODEL LICENSE
/*Model Information:
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
//bool blinn = false;
//bool blinnKeyPressed = false;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseActive = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float boatScale = 0.125f;

float rockingIntensity = 2.0f;
float rockingAngle = 0.0f;

//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
//float ambientStrength = 0.05f;
//float specularStrength = 0.1f;
//float diffuseStrength = 0.1f;
//float shine = 2.0f;

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

    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    //ImGui_ImplOpenGL3_Init();

    // Cubemap Writen by Caleb using OpnelGL Tutorials
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

    std::vector<std::string> cubeFaces
    {
        "assets/cubemap/right.jpg",
        "assets/cubemap/left.jpg",
        "assets/cubemap/top.jpg",
        "assets/cubemap/bottom.jpg",
        "assets/cubemap/front.jpg",
        "assets/cubemap/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(cubeFaces);

    skyboxShader.use();
    skyboxShader.setInt("cubemap", 0);

    // 3D Model Written by Caleb using OpenGL
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    Shader boatShader("assets/boatVertexShader.vert", "assets/boatFragmentShader.frag");
    Model boatModel("assets/boatAssets/boat.obj");

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        rockingAngle = rockingIntensity * sin(currentFrame);

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
        shaderProgram.setInt("blinn", blinn);
        shaderProgram.setFloat("ambient", ambientStrength);
        shaderProgram.setFloat("specularStrength", specularStrength);
        shaderProgram.setFloat("diffuseStrength", diffuseStrength);
        shaderProgram.setFloat("shine", shine);

        lightProgram.use();

        lightProgram.setMat4("projection", projection);
        lightProgram.setMat4("view", view);

        glm::mat4 lightModel = glm::mat4(1.0f);

        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightProgram.setMat4("model", lightModel);
        lightProgram.setVec3("lightColor", lightColor);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
        ImGui::ColorEdit3("Light Color", &lightColor.r);
        ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0f, 1.0f);
        ImGui::SliderFloat("Shine Strength", &shine, 2.0f, 1024.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

        boatShader.use();

        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        boatShader.setMat4("proj", projection);
        boatShader.setMat4("view", view);

        glm::mat4 boatVertModel = glm::mat4(1.0f);
        boatVertModel = glm::translate(boatVertModel, glm::vec3(0.0f, 0.0f, 0.0f));
        boatVertModel = glm::rotate(boatVertModel, glm::radians(rockingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        boatVertModel = glm::scale(boatVertModel, glm::vec3(boatScale, boatScale, boatScale));
        boatShader.setMat4("model", boatVertModel);
        boatModel.Draw(boatShader);

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

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &cubemapVAO);
    glDeleteVertexArrays(1, &cubemapVBO);
    printf("Shutting down...");
    glfwTerminate();
    return 0;
}

// Written by Caleb using OpenGL Tutorial
void processInput(GLFWwindow* window)
{
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

    /*if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }*/

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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Written by Caleb using OpenGL Tutorial
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
    if (mouseActive)
    {
        float xPos = static_cast<float>(xPosIn);
        float yPos = static_cast<float>(yPosIn);

        if (firstMouse)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        float xOffSet = xPos - lastX;
        float yOffSet = lastY - yPos;

        lastX = xPos;
        lastY = yPos;

        cam.ProcessMouseMovement(xOffSet, yOffSet);
    }
}

// Written by Caleb using OpenGL Tutorial
void scroll_callback(GLFWwindow* window, double xOffSet, double yOffSet)
{
    cam.ProcessMouseScroll(static_cast<float>(yOffSet));
}

// Written by Caleb using OpenGL Tutorial
unsigned int loadTexture(char const* path)
{
    unsigned int textID;
    glGenTextures(1, &textID);

    int width, height, nrComps;
    unsigned char* data = stbi_load(path, &width, &height, &nrComps, 0);
    if (data)
    {
        GLenum format;
        if (nrComps == 1)
            format = GL_RED;
        else if (nrComps == 3)
            format = GL_RGB;
        else if (nrComps == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textID;
}

// Written by Caleb using OpenGL Tutorial
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textID;
    glGenTextures(1, &textID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textID);

    int width, height, nrComps;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComps, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textID;
}