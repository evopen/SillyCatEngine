#pragma once

#include "pch.h"

class ShadingBaseColor
{
public:
    ShadingBaseColor(const Sce::Camera& inCamera)
    {
        MVP.View       = inCamera.GetViewMatrix();
        MVP.Projection = inCamera.GetProjectionMatrix();
        MVP.Model      = glm::identity<glm::mat4>();
    }

private:
    struct SMVP
    {
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Projection;
    } MVP;
};