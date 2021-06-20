#pragma once

#include <vector>

#include "core/maths/maths.hpp"

using namespace Core::Maths;

namespace LowRenderer
{
    struct CameraInputs
    {
        float deltaX;
        float deltaY;
        bool moveForward;
        bool moveBackward;
        bool strafeLeft;
        bool strafeRight;
        bool moveUp;
        bool moveDown;
    };

    class Camera
    {

        public:
            Camera() = default;
            Camera(int width, int height, std::vector<float>& camInfo);

            Camera(const Camera& camera);
            void operator=(const Camera& camera);

            void                update(const CameraInputs& inputs);
            void                update(const CameraInputs& inputs, const vec3& playerPos);
            void                showImGuiControls();

            mat4   getViewMatrix() const;
            mat4   getProjection() const;
            mat4   getMVP(const mat4& model) const;
            vec3   getCamPos() const;

            float*              getCamInfo() const;

        private:
            void                debug() const;

            float               MOUSE_SENSITIVITY = 0.002f;
            float               SPEED = 1.f;
            float               pitch = 0.f;
            float               yaw = 0.f;
            float               aspect = 1.f; 
            float               fovY = 60.f * TORAD;
            float               near = 0.01f;
            float               far = 10.f;

            float               playerDistanceY = 1.7f;
            float               playerDistanceZ = 4.f;

            bool                orthoView = false;

            vec3   position = { 0.175f, 0.474f, 1.773f };
            
    };
}