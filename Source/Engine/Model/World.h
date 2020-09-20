#pragma once

class Model;

class World
{
    struct ObjectParameters
    {
        std::string Name;
        glm::vec3 Location;
        glm::vec3 Rotation;
        glm::vec3 Scale;
    };

public:
    API void AddModel(std::shared_ptr<Model> InModel, glm::vec3 InLocation, glm::vec3 InRotation, glm::vec3 InScale, std::string InName = "");

private:
    std::unordered_map<std::shared_ptr<Model>, std::vector<ObjectParameters>> Objects;
};