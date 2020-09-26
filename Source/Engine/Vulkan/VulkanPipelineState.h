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
    API VulkanGraphicsPipelineState(VulkanGraphicsShaderProgram* InShaderProgram);
    uint32_t GetStageCount() const { return static_cast<VulkanGraphicsShaderProgram*>(ShaderProgram)->GetStageCount(); }
    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() const { return ShaderProgram->GetPipelineShaderStageCreateInfos(); }

    static std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions() { return Sce::Model::GetVertexInputBindingDescriptions(); }
    std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescriptions() { return ShaderProgram->GetVertexInputAttributeDescriptions(); }
    API VkPipelineLayout GetPipelineLayoutHandle() { return ShaderProgram->GetPipelineLayoutHandle(); }
    API VkDescriptorSetLayout GetDescriptorSetLayoutHandle() { return ShaderProgram->GetDescriptorSetLayoutHandle(); }


private:
    VulkanGraphicsShaderProgram* ShaderProgram;
};

class VulkanComputePipelineState : public VulkanPipelineState
{
public:
    API VulkanComputePipelineState(VulkanComputeShaderProgram* InShaderProgram);

    VkPipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo() const { return ShaderProgram->GetPipelineShaderStageCreateInfo(); }
    VkPipelineLayout GetPipelineLayoutHandle() { return ShaderProgram->GetPipelineLayoutHandle(); }

private:
    VulkanComputeShaderProgram* ShaderProgram;
};