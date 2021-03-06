#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

namespace Sce
{
    class Texture;

    class Material
    {
    public:
        API Material(aiMaterial* inMaterial, aiTexture** inTextures);

        std::shared_ptr<Texture> GetDiffuseTexture() { return Diffuse; }

    private:
        std::shared_ptr<Texture> Diffuse;
    };
}
