#include "camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Pos = pos;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Pos = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Pos, Pos + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement dir, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    switch (dir) {
    case FORWARD:
        Pos += Front * velocity;
        break;
    case BACKWARD:
        Pos -= Front * velocity;
        break;
    case LEFT:
        Pos -= Right * velocity;
        break;
    case RIGHT:
        Pos += Right * velocity;
        break;
    }
}

void Camera::ProcessMouseMovement(float xOffSet, float yOffSet, GLboolean constrainPitch)
{
    xOffSet *= MouseSensitivity;
    yOffSet *= MouseSensitivity;

    Yaw += xOffSet;
    Pitch += yOffSet;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffSet)
{
    Zoom -= (float)yOffSet;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
