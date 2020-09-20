#include "Engine/pch.h"

#include "Model.h"

Model::Model(std::filesystem::path InFilePath)
{
    Assimp::Importer Importer;
    const aiScene* Scene = Importer.ReadFile(InFilePath.string().c_str(), aiProcess_Triangulate);
    if (!Scene)
    {
        throw std::runtime_error(Importer.GetErrorString());
    }
    if (Scene->mNumMeshes != 1)
    {
        throw std::runtime_error("not supported yet");
    }

    aiMesh* Mesh           = Scene->mMeshes[0];
    Name = Mesh->mName.C_Str();
    unsigned VerticesCount = Mesh->mNumVertices;

    Vertices.resize(VerticesCount);

    for (size_t i = 0; i < VerticesCount; i++)
    {
        Vertices[i].x = Mesh->mVertices[i].x;
        Vertices[i].y = Mesh->mVertices[i].y;
        Vertices[i].z = Mesh->mVertices[i].z;
    }
}
