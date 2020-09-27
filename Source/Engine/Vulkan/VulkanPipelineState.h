#pragma once

#include "Engine/Model/Model.h"
#include "Engine/Platform/Platform.h"
#include "VulkanShaderProgram.h"


class VulkanPipelineState
{
public:
    API VulkanPipelineState();


protected:
};

class VulkanGraphicsPipelineState : public VulkanPipelineState
{
public:
    API VulkanGraphicsPipelineState(std::shared_ptr<VulkanGraphicsShaderProgram> InShaderProgram);
    uint32_t GetStageCount() const { return ShaderProgram->GetStageCount(); }
    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() const { return ShaderProgram->GetPipelineShaderStageCreateInfos(); }

    static std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions() { return Sce::Model::GetVertexInputBindingDescriptions(); }
    std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescriptions() { return ShaderProgram->GetVertexInputAttributeDescriptions(); }
    API VkPipelineLayout GetPipelineLayoutHandle() { return ShaderProgram->GetPipelineLayoutHandle(); }
    API VkDescriptorSetLayout GetDescriptorSetLayoutHandle() { return ShaderProgram->GetDescriptorSetLayoutHandle(); }

    API void SetPolygonMode(VkPolygonMode inPolygonMode) { PolygonMode = inPolygonMode; }
    API VkPolygonMode GetPolygonMode() { return PolygonMode; }

private:
    std::shared_ptr<VulkanGraphicsShaderProgram> ShaderProgram;
    VkPolygonMode PolygonMode;
};

class VulkanComputePipelineState : public VulkanPipelineState
{
public:
    API VulkanComputePipelineState(std::shared_ptr<VulkanComputeShaderProgram> InShaderProgram);

    VkPipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo() const { return ShaderProgram->GetPipelineShaderStageCreateInfo(); }
    VkPipelineLayout GetPipelineLayoutHandle() { return ShaderProgram->GetPipelineLayoutHandle(); }

private:
    std::shared_ptr<VulkanComputeShaderProgram> ShaderProgram;
};