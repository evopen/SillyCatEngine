#pragma once

#include "pch.h"

enum class EShading
{
    VertexColor,
    BaseColor,
    WireFrame,
    Depth,
};


inline std::map<EShading, std::map<EShaderType, std::filesystem::path>> ShadingFileMap = 
{
    //{
    //    EShading::VertexColor,
    //    {


    //    }
    //},
    {
        EShading::BaseColor,
        {
            {EShaderType::Vertex, "BaseColor.vert"},
            {EShaderType::Pixel, "BaseColor.frag"}
        }
    },
    {
        EShading::WireFrame,
        {
            {EShaderType::Vertex, "WireFrame.vert"},
            {EShaderType::Pixel, "WireFrame.frag"},
        }
    }
};
