#include "VulkanShader.h"

#include "Engine/Filesystem/FileUtil.h"
#include "VulkanDevice.h"
#include "VulkanShaderCompiler.h"
#include "VulkanUtil.h"

VulkanShader::VulkanShader(VulkanDevice* InDevice, std::filesystem::path InFilePath)
    : Device(InDevice)
      , FilePath(std::move(InFilePath))
      , ShaderModule(VK_NULL_HANDLE)
{
    ShaderType = ToShaderType(FilePath.extension().string());
    GLSL       = LoadFile(FilePath, false);

    Spirv = CompileGLSL(ShaderType, GLSL);

    VkShaderModuleCreateInfo ShaderModuleInfo = {};
    ShaderModuleInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ShaderModuleInfo.codeSize                 = Spirv.size() * 4;
    ShaderModuleInfo.pCode                    = Spirv.data();

    

    vkCreateShaderModule(Device->GetDeviceHandle(), &ShaderModuleInfo, nullptr, &ShaderModule);
}
