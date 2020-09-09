#pragma once

#include <vector>

enum class EShaderType;

std::vector<unsigned> CompileGLSL(EShaderType ShaderType, std::vector<char> GLSLCode);
