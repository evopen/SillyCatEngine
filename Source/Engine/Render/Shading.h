#pragma once

class VulkanDevice;
class VulkanGraphicsShaderProgram;
class VulkanGraphicsPipelineState;

namespace Sce
{
    struct ShadingInfo
    {
        std::string name;
        std::filesystem::path vertex_shader;
        std::filesystem::path fragment_shader;
        VkPolygonMode polygon_mode;
        std::optional<float> line_width;

        API static ShadingInfo LoadShadingInfo(const std::filesystem::path& inJsonPath);
    };

    class Shading : public std::enable_shared_from_this<Shading>
    {
    public:
        virtual ~Shading() = default;
        API Shading(VulkanDevice* inDevice, std::shared_ptr<VulkanGraphicsShaderProgram> inShaderProgram, ShadingInfo inShadingInfo, std::shared_ptr<VulkanGraphicsPipelineState>& inPipelineState);
        virtual API void UpdateDescriptor(){};

        API void BindDescriptorSet(VulkanCommandBuffer* inCmdBuf);

    protected:
        VulkanDevice* Device;
        std::shared_ptr<VulkanGraphicsShaderProgram> ShaderProgram;
    };


}
