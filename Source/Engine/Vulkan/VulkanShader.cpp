#include "VulkanShader.h"

#include "Engine/Filesystem/FileUtil.h"
#include "VulkanDevice.h"
#include "VulkanShaderCompiler.h"
#include "VulkanUtil.h"

VulkanShader::VulkanShader(VulkanDevice* InDevice, std::filesystem::path InFilePath)
    : Device(InDevice)
    , FilePath(std::move(InFilePath))
    , ShaderModule(nullptr)
{
    ShaderType = ToShaderType(FilePath.extension().string());
    GLSL       = LoadFile(FilePath, false);
    Spirv      = CompileGLSL(ShaderType, GLSL);

    VkShaderModuleCreateInfo ShaderModuleInfo = {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = Spirv.size() * 4,
        .pCode    = Spirv.data(),
    };
    CheckResult(vkCreateShaderModule(Device->GetDeviceHandle(), &ShaderModuleInfo, nullptr, &ShaderModule));
}
