#include "Engine/pch.h"

#include "Camera.h"

namespace Sce
{
    Camera::Camera(glm::vec3 inLocation, glm::vec3 inLookAt, glm::vec3 inUp, float inAspect, float inFov)
        : Location(inLocation)
        , Rotation(glm::vec3(0))
        , Fov(inFov)
        , Aspect(inAspect)
    {
        ViewMatrix       = glm::lookAt(Location, inLookAt, inUp);
        ProjectionMatrix = glm::perspective(glm::radians(Fov), Aspect, 0.1f, 10.f);
    }
}
