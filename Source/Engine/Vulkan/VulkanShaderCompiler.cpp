#include "Engine/pch.h"

#include "Engine/Filesystem/FileUtil.h"
#include "Engine/Render/Definitions.h"
#include "VulkanShaderCompiler.h"


bool IsGlslangInitialized                      = false;
const TBuiltInResource DefaultTBuiltInResource = {
    .maxLights                                 = 32,
    .maxClipPlanes                             = 6,
    .maxTextureUnits                           = 32,
    .maxTextureCoords                          = 32,
    .maxVertexAttribs                          = 64,
    .maxVertexUniformComponents                = 4096,
    .maxVaryingFloats                          = 64,
    .maxVertexTextureImageUnits                = 32,
    .maxCombinedTextureImageUnits              = 80,
    .maxTextureImageUnits                      = 32,
    .maxFragmentUniformComponents              = 4096,
    .maxDrawBuffers                            = 32,
    .maxVertexUniformVectors                   = 128,
    .maxVaryingVectors                         = 8,
    .maxFragmentUniformVectors                 = 16,
    .maxVertexOutputVectors                    = 16,
    .maxFragmentInputVectors                   = 15,
    .minProgramTexelOffset                     = -8,
    .maxProgramTexelOffset                     = 7,
    .maxClipDistances                          = 8,
    .maxComputeWorkGroupCountX                 = 65535,
    .maxComputeWorkGroupCountY                 = 65535,
    .maxComputeWorkGroupCountZ                 = 65535,
    .maxComputeWorkGroupSizeX                  = 1024,
    .maxComputeWorkGroupSizeY                  = 1024,
    .maxComputeWorkGroupSizeZ                  = 64,
    .maxComputeUniformComponents               = 1024,
    .maxComputeTextureImageUnits               = 16,
    .maxComputeImageUniforms                   = 8,
    .maxComputeAtomicCounters                  = 8,
    .maxComputeAtomicCounterBuffers            = 1,
    .maxVaryingComponents                      = 60,
    .maxVertexOutputComponents                 = 64,
    .maxGeometryInputComponents                = 64,
    .maxGeometryOutputComponents               = 128,
    .maxFragmentInputComponents                = 128,
    .maxImageUnits                             = 8,
    .maxCombinedImageUnitsAndFragmentOutputs   = 8,
    .maxCombinedShaderOutputResources          = 8,
    .maxImageSamples                           = 0,
    .maxVertexImageUniforms                    = 0,
    .maxTessControlImageUniforms               = 0,
    .maxTessEvaluationImageUniforms            = 0,
    .maxGeometryImageUniforms                  = 0,
    .maxFragmentImageUniforms                  = 8,
    .maxCombinedImageUniforms                  = 8,
    .maxGeometryTextureImageUnits              = 16,
    .maxGeometryOutputVertices                 = 256,
    .maxGeometryTotalOutputComponents          = 1024,
    .maxGeometryUniformComponents              = 1024,
    .maxGeometryVaryingComponents              = 64,
    .maxTessControlInputComponents             = 128,
    .maxTessControlOutputComponents            = 128,
    .maxTessControlTextureImageUnits           = 16,
    .maxTessControlUniformComponents           = 1024,
    .maxTessControlTotalOutputComponents       = 4096,
    .maxTessEvaluationInputComponents          = 128,
    .maxTessEvaluationOutputComponents         = 128,
    .maxTessEvaluationTextureImageUnits        = 16,
    .maxTessEvaluationUniformComponents        = 1024,
    .maxTessPatchComponents                    = 120,
    .maxPatchVertices                          = 32,
    .maxTessGenLevel                           = 64,
    .maxViewports                              = 16,
    .maxVertexAtomicCounters                   = 0,
    .maxTessControlAtomicCounters              = 0,
    .maxTessEvaluationAtomicCounters           = 0,
    .maxGeometryAtomicCounters                 = 0,
    .maxFragmentAtomicCounters                 = 8,
    .maxCombinedAtomicCounters                 = 8,
    .maxAtomicCounterBindings                  = 1,
    .maxVertexAtomicCounterBuffers             = 0,
    .maxTessControlAtomicCounterBuffers        = 0,
    .maxTessEvaluationAtomicCounterBuffers     = 0,
    .maxGeometryAtomicCounterBuffers           = 0,
    .maxFragmentAtomicCounterBuffers           = 1,
    .maxCombinedAtomicCounterBuffers           = 1,
    .maxAtomicCounterBufferSize                = 16384,
    .maxTransformFeedbackBuffers               = 4,
    .maxTransformFeedbackInterleavedComponents = 64,
    .maxCullDistances                          = 8,
    .maxCombinedClipAndCullDistances           = 8,
    .maxSamples                                = 4,
    .limits =
        {
            .nonInductiveForLoops                 = 1,
            .whileLoops                           = 1,
            .doWhileLoops                         = 1,
            .generalUniformIndexing               = 1,
            .generalAttributeMatrixVectorIndexing = 1,
            .generalVaryingIndexing               = 1,
            .generalSamplerIndexing               = 1,
            .generalVariableIndexing              = 1,
            .generalConstantMatrixVectorIndexing  = 1,
        },
};

EShLanguage ToGlslangType(EShaderType EngineType)
{
    const std::map<EShaderType, EShLanguage> ShaderTypeMap = {{EShaderType::Vertex, EShLangVertex},
        {EShaderType::Pixel, EShLangFragment}, {EShaderType::Compute, EShLangCompute}};
    return ShaderTypeMap.at(EngineType);
}

std::vector<unsigned> CompileGLSL(EShaderType ShaderType, std::vector<char> GLSLCode)
{
    if (!IsGlslangInitialized)
    {
        glslang::InitializeProcess();
        IsGlslangInitialized = true;
    }
    const auto GlslangShaderType = ToGlslangType(ShaderType);
    glslang::TShader Shader(GlslangShaderType);
    const char* Str          = GLSLCode.data();
    const int DefaultVersion = 110;
    Shader.setStrings(&Str, 1);
    Shader.setEnvInput(glslang::EShSourceGlsl, GlslangShaderType, glslang::EShClientVulkan, DefaultVersion);
    Shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1);
    Shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);
    const EShMessages messages = EShMsgDefault;

    if (!Shader.parse(&DefaultTBuiltInResource, DefaultVersion, ENoProfile, false, false, messages))
    {
        spdlog::error("Parse GLSL failed.");
        spdlog::error(Shader.getInfoLog());
        spdlog::error(Shader.getInfoDebugLog());
        throw std::runtime_error("Parse GLSL failed.");
    }
    glslang::TProgram Program;
    Program.addShader(&Shader);
    if (!Program.link(messages))
    {
        spdlog::error("Parse GLSL failed.d");
        spdlog::error(Shader.getInfoLog());
        spdlog::error(Shader.getInfoDebugLog());
        throw std::runtime_error("Parse GLSL failed.");
    }
    std::vector<unsigned> spirv;
    glslang::SpvOptions SpvOptions;
    SpvOptions.validate         = true;
    SpvOptions.disableOptimizer = true;
    glslang::GlslangToSpv(*Program.getIntermediate(GlslangShaderType), spirv, &SpvOptions);

    return spirv;
}
