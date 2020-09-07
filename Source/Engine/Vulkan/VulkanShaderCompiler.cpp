#include <spirv/GlslangToSpv.h>

bool IsGlslangInitialized = false;

static void CompileGLSL()
{
    glslang::InitializeProcess();
}