#include "Engine/pch.h"

#include "Engine/Logger/Logger.h"
#include "Engine/Texture/Texture.h"
#include "Material.h"

namespace Sce
{
    Material::Material(aiMaterial* inMaterial, aiTexture** inTextures)
    {
        aiString aiPath;
        inMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);

        std::string path = aiPath.C_Str();

        if (path[0] == '*')
        {
            size_t textureIndex = std::atoi(path.substr(1, path.size() - 1).c_str());
            Logger::Get()->info(inTextures[textureIndex]->achFormatHint);
            if (inTextures[textureIndex]->mHeight == 0)
            {
                Diffuse.reset(new Texture(inTextures[textureIndex]->pcData, inTextures[textureIndex]->mWidth));
            }
        }
        else
        {
            Diffuse.reset(new Texture(path));
        }
    }
}
