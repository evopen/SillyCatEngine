#pragma once

#include <vector>

enum class EShaderType;

std::tuple<std::vector<unsigned>, ShaderReflectionInfo> CompileGLSL(EShaderType ShaderType, std::vector<char> GLSLCode);
