#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

namespace Sce
{
    class Camera
    {
    public:
        //API Camera(glm::vec3 inLocation, glm::vec3 inRotation, float inAspect = 4.0 / 3.0, float inFov = 60);
        API Camera(glm::vec3 inLocation, glm::vec3 inLookAt, glm::vec3 inUp = {0, 1, 0}, float inAspect = 4.0 / 3.0, float inFov = 60);
        API glm::mat4 GetViewMatrix() const { return ViewMatrix; }
        API glm::mat4 GetProjectionMatrix() const { return ProjectionMatrix; }

    private:
        glm::vec3 Location;
        glm::vec3 Rotation;
        float Fov;  //in degree
        float Aspect;

        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
    };
}
