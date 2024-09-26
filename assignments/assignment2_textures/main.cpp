#include <stdio.h>
#include <iostream>

#include <caleb/shader.hpp>
#include <caleb/texture.hpp>
#include <ew/external/glad.h>
#include <ew/external/stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int main() {
    printf("Initializing...");
    if (!glfwInit()) {
        printf("GLFW failed to init!");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment Two", NULL, NULL);
    if (window == NULL) {
        printf("GLFW failed to create window");
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
        return 1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float vertices[] = {
        // POSITIONS            // COLORS           // TEXTURE COORDS
         1.0f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // TOP RIGHT
         1.0f, -1.0f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // BOTTOM RIGHT
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // BOTTOM LEFT
        -1.0f,  1.0f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f      // TOP LEFT
    };

    unsigned int indices[] = {
        0, 1, 3,    // FIRST TRIANGLE
        1, 2, 3     // SECOND TRIANGLE
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Texture backgroundTexture1;
    Texture backgroundTexture2;

    backgroundTexture1.TextureJPG("assets/checkerboard.jpg", GL_LINEAR, GL_REPEAT);
    backgroundTexture2.TexturePNG("assets/face.png", GL_LINEAR, GL_REPEAT);

    Texture characterTexture;

    characterTexture.TextureJPG("assets/chest.jpg", GL_NEAREST, GL_CLAMP_TO_EDGE);

    Shader shaderProgram("assets/backgroundVertexShader.vert", "assets/backgroundFragmentShader.frag");
    Shader characterShader("assets/characterVertexShader.vert", "assets/characterFragmentShader.frag");

    shaderProgram.use();

    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        backgroundTexture1.Bind(0);
        backgroundTexture2.Bind(1);
        shaderProgram.setFloat("tiling", 5.0f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        characterShader.use();
        characterTexture.Bind(0);
        characterShader.setInt("characterTexture", 0);
        float characterPosX = 0.0f;
        float characterPosY = 0.0f;
        glUniform2f(glGetUniformLocation(characterShader.ID, "position"), characterPosX, characterPosY);
        float scaleX = 2.0f;
        float scaleY = 2.0f;
        glUniform2f(glGetUniformLocation(characterShader.ID, "scale"), scaleX, scaleY);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    printf("Shutting down...");
    glfwTerminate();
    return 0;
}
