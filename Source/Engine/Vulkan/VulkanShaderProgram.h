#pragma once

#include "Engine/pch.h"

class VulkanVertexShader;
class VulkanPixelShader;
class VulkanComputeShader;
class VulkanDevice;

class VulkanShaderProgram
{
public:
    API VulkanShaderProgram(VulkanDevice* inDevice);
    API ~VulkanShaderProgram();
    API VkPipelineLayout GetPipelineLayoutHandle() { return PipelineLayout; }

    std::vector<VkDescriptorSetLayoutBinding> GetDescriptorSetLayoutBindings() { return Bindings; };
    API VkDescriptorSetLayout GetDescriptorSetLayoutHandle() { return DescriptorSetLayout; }
    API VkDescriptorSet GetDescriptorSetHandle() { return DescriptorSet; }

protected:
    std::vector<VkDescriptorSetLayoutBinding> Bindings;
    virtual void CreateDescriptorSetLayoutBindings() = 0;
    void CreateDescriptorSetLayout();
    void CreatePipelineLayout();
    void CreateDescriptorSet();

    VkDescriptorSetLayout DescriptorSetLayout;
    VkDescriptorSet DescriptorSet;
    VulkanDevice* Device;
    VkPipelineLayout PipelineLayout;
};

class VulkanGraphicsShaderProgram : public VulkanShaderProgram
{
public:
    API VulkanGraphicsShaderProgram(VulkanDevice* inDevice, std::shared_ptr<VulkanVertexShader> InVertexShader, std::shared_ptr<VulkanPixelShader> InPixelShader);

    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() { return PipelineStageInfos.data(); }
    uint32_t GetStageCount() const { return static_cast<uint32_t>(PipelineStageInfos.size()); }

    std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescriptions();
    VkDescriptorSetLayoutCreateInfo GetDescriptorSetLayoutCreateInfo();

private:
    std::shared_ptr<VulkanVertexShader> VertexShader;
    std::shared_ptr<VulkanPixelShader> PixelShader;
    std::vector<VkPipelineShaderStageCreateInfo> PipelineStageInfos;
    void CreateDescriptorSetLayoutBindings() override;
};

class VulkanComputeShaderProgram : public VulkanShaderProgram
{
public:
    API VulkanComputeShaderProgram(VulkanComputeShader* InComputeShader);
    VkPipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo() { return PipelineStageInfo; }

private:
    VulkanComputeShader* ComputeShader;
    VkPipelineShaderStageCreateInfo PipelineStageInfo;
};
