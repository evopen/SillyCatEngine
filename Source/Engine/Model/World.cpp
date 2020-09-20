#include "Engine/pch.h"

#include "Model.h"
#include "World.h"

void World::AddModel(std::shared_ptr<Model> InModel, glm::vec3 InLocation, glm::vec3 InRotation, glm::vec3 InScale, std::string InName)
{
    std::string Name = std::move(InName);

    bool bModelExistInWorld = Objects.contains(InModel);

    if (Name.empty())
    {
        if (bModelExistInWorld)
            Name = InModel->GetName() + '_' + std::to_string(Objects.at(InModel).size());
        else
            Name = InModel->GetName() + '_' + '0';
    }

    if (bModelExistInWorld)
    {
        Objects.at(InModel).emplace_back(
            ObjectParameters{
                .Name     = Name,
                .Location = InLocation,
                .Rotation = InRotation,
                .Scale    = InScale,
            });
    }
    else
    {
        Objects.insert(
            {InModel,
                {ObjectParameters{
                    .Name     = Name,
                    .Location = InLocation,
                    .Rotation = InRotation,
                    .Scale    = InScale,
                }}});
    }
}
