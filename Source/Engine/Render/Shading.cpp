#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanPipelineState.h"
#include "Engine/Vulkan/VulkanShaderProgram.h"
#include "Shading.h"

namespace Sce
{
    ShadingInfo ShadingInfo::LoadShadingInfo(const std::filesystem::path& inJsonPath)
    {
        std::map<std::string, VkPolygonMode> polygonModeMap = {{"Fill", VK_POLYGON_MODE_FILL},
            {"Line", VK_POLYGON_MODE_LINE},
            {"Point", VK_POLYGON_MODE_POINT}};
        if (!std::filesystem::is_regular_file(inJsonPath) || inJsonPath.extension() != ".json")
            throw std::runtime_error("not a valid json file");

        std::ifstream ifs(inJsonPath);
        auto json = nlohmann::json::parse(ifs);

        ShadingInfo result = {
            .name            = json.at("name").get<std::string>(),
            .vertex_shader   = inJsonPath.parent_path() / json.at("vertex_shader").get<std::string>(),
            .fragment_shader = inJsonPath.parent_path() / json.at("fragment_shader").get<std::string>(),
            .polygon_mode    = polygonModeMap.at(json.at("polygon_mode").get<std::string>()),
        };

        if (result.polygon_mode == VK_POLYGON_MODE_LINE)
            result.line_width = json.at("line_width").get<float>();

        return result;
    }

    Shading::Shading(VulkanDevice* inDevice, std::shared_ptr<VulkanGraphicsShaderProgram> inShaderProgram, ShadingInfo inShadingInfo, std::shared_ptr<VulkanGraphicsPipelineState>& inPipelineState)
        : Device(inDevice)
        , ShaderProgram(inShaderProgram)
    {
        inPipelineState->SetPolygonMode(inShadingInfo.polygon_mode);
    }

    void Shading::BindDescriptorSet(VulkanCommandBuffer* inCmdBuf)
    {
        VkDescriptorSet descriptorSet = ShaderProgram->GetDescriptorSetHandle();
        vkCmdBindDescriptorSets(inCmdBuf->GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderProgram->GetPipelineLayoutHandle(), 0, 1, &descriptorSet, 0, nullptr);
        inCmdBuf->PossessObject(shared_from_this());
    }
}
