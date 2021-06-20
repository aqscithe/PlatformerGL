#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "lowrenderer/camera.hpp"
#include "core/debug/log.hpp"
#include "time.hpp"

using namespace LowRenderer;
using namespace Core::Debug;
using namespace Core::Maths;

Camera::Camera(int width, int height, std::vector<float>& camInfo)
{
    aspect = (float)width / (float)height;
    position = { camInfo[0], camInfo[1], camInfo[2] };
    pitch = camInfo[3];
    yaw = camInfo[4];
    fovY = camInfo[5] * TORAD;
    near = camInfo[6];
    far = camInfo[7];
    MOUSE_SENSITIVITY = camInfo[8];
    SPEED = camInfo[9];

    debug();

}

Camera::Camera(const Camera& camera)
{
    *this = camera;
}

void Camera::operator=(const Camera& camera)
{
    MOUSE_SENSITIVITY = camera.MOUSE_SENSITIVITY;
    SPEED = camera.SPEED;
    pitch = camera.pitch;
    yaw = camera.yaw;
    aspect = camera.aspect;
    fovY = camera.fovY;
    near = camera.near;
    far = camera.far;
    orthoView = camera.orthoView;
    position = camera.position;
}

void Camera::debug() const
{
    std::string statement1 = "Aspect Ratio: " + std::to_string(aspect);
    std::string statement2 = "Mouse sensitivity: " + std::to_string(MOUSE_SENSITIVITY);
    std::string statement3 = "Mouse speed: " + std::to_string(SPEED);
    Log::print(statement1, LogType::DEBUG);
    Log::print(statement2, LogType::DEBUG);
    Log::print(statement3, LogType::DEBUG);
}

void    Camera::update(const CameraInputs& inputs, const vec3& playerPos)
{
    position.x = playerPos.x;
    position.y = playerPos.y + playerDistanceY;
    position.z = playerPos.z + playerDistanceZ;
    pitch = 0.5f;
    yaw = 0.f;
}

void Camera::update(const CameraInputs& inputs)
{
    yaw += inputs.deltaX * MOUSE_SENSITIVITY;
    pitch += inputs.deltaY * MOUSE_SENSITIVITY;

    float deltaTime = Time::deltaTime();

    float forwardMovement = 0.f;
    if (inputs.moveForward)
        forwardMovement += SPEED * deltaTime;
    if (inputs.moveBackward)
        forwardMovement -= SPEED * deltaTime;

    float strafeMovement = 0.f;
    if (inputs.strafeLeft)
        strafeMovement -= SPEED * deltaTime;
    if (inputs.strafeRight)
        strafeMovement += SPEED * deltaTime;

    float verticalMovement = 0.f;
    if (inputs.moveUp)
        verticalMovement += SPEED * deltaTime;
    if (inputs.moveDown)
        verticalMovement -= SPEED * deltaTime;

    position.x += Core::Maths::sin(yaw) * forwardMovement;
    position.z -= Core::Maths::cos(yaw) * forwardMovement;
                  
    position.x += Core::Maths::cos(yaw) * strafeMovement;
    position.z += Core::Maths::sin(yaw) * strafeMovement;

    position.y += verticalMovement;
}

mat4 Camera::getViewMatrix() const
{
    return rotateX(pitch) * rotateY(yaw) * translate(-position);
}

mat4 Camera::getProjection() const
{
    return perspective(fovY, aspect, near, far, orthoView);
}

mat4 Camera::getMVP(const mat4& model) const
{
    return getProjection() * getViewMatrix() * model;
}

vec3 Camera::getCamPos() const
{
    return position;
}

float* Camera::getCamInfo() const
{
    float info[10];

    info[0] = position.x;
    info[1] = position.y;
    info[2] = position.z;
    info[3] = pitch;
    info[4] = yaw;
    info[5] = fovY * TODEG;
    info[6] = near;
    info[7] = far;
    info[8] = MOUSE_SENSITIVITY;
    info[9] = SPEED;

    return info;
}

void Camera::showImGuiControls()
{
    ImGui::NextColumn();
    if (ImGui::Checkbox("Orthographic View", &orthoView))
    {
        if (orthoView)
            fovY = 3.127f;
        else
            fovY = 1.047f;
    }
    ImGui::DragFloat3("pos", position.e);
    ImGui::DragFloat("pitch", &pitch);
    ImGui::DragFloat("yaw", &yaw);
    ImGui::DragFloat("fovY", &fovY);
    ImGui::DragFloat("near", &near);
    ImGui::DragFloat("far", &far);
    ImGui::NextColumn();
    
    
}
