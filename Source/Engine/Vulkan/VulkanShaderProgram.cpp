#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanShader.h"
#include "VulkanShaderProgram.h"

VulkanShaderProgram::VulkanShaderProgram(VulkanDevice* inDevice)
    : DescriptorSetLayout(VK_NULL_HANDLE)
    , Device(inDevice)
{
}

VulkanShaderProgram::~VulkanShaderProgram()
{
    if (DescriptorSetLayout != VK_NULL_HANDLE)
        vkDestroyDescriptorSetLayout(Device->GetDeviceHandle(), DescriptorSetLayout, nullptr);
}

void VulkanShaderProgram::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo = {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(Bindings.size()),
        .pBindings    = Bindings.data(),
    };
    vkCreateDescriptorSetLayout(Device->GetDeviceHandle(), &DescriptorSetLayoutInfo, nullptr, &DescriptorSetLayout);
}

void VulkanShaderProgram::CreatePipelineLayout()
{
    VkPipelineLayoutCreateInfo PipelineLayoutInfo = {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount         = 1,
        .pSetLayouts            = &DescriptorSetLayout,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges    = nullptr,
    };
    vkCreatePipelineLayout(Device->GetDeviceHandle(), &PipelineLayoutInfo, nullptr, &PipelineLayout);
}

VulkanGraphicsShaderProgram::VulkanGraphicsShaderProgram(VulkanDevice* inDevice, std::shared_ptr<VulkanVertexShader> InVertexShader, std::shared_ptr<VulkanPixelShader> InPixelShader)
    : VulkanShaderProgram(inDevice)
    , VertexShader(InVertexShader)
    , PixelShader(InPixelShader)
{
    PipelineStageInfos.push_back(VertexShader->GetPipelineShaderStageInfo());
    PipelineStageInfos.push_back(PixelShader->GetPipelineShaderStageInfo());
    VulkanGraphicsShaderProgram::CreateDescriptorSetLayoutBindings();
    CreateDescriptorSetLayout();
    CreatePipelineLayout();
}

std::vector<VkVertexInputAttributeDescription> VulkanGraphicsShaderProgram::GetVertexInputAttributeDescriptions()
{
    ShaderReflectionInfo VSReflection = VertexShader->GetShaderReflection();
    std::vector<VkVertexInputAttributeDescription> Descriptions(VSReflection.InputInfos.size());

    for (uint32_t i = 0; i < Descriptions.size(); ++i)
    {
        Descriptions[i].location = VSReflection.InputInfos[i].Location;
        Descriptions[i].binding  = i;
        Descriptions[i].format   = GlslBasicTypeToVkFormat(VSReflection.InputInfos[i].Type, VSReflection.InputInfos[i].VectorSize);
        Descriptions[i].offset   = 0;
    }

    return Descriptions;
}

void VulkanGraphicsShaderProgram::CreateDescriptorSetLayoutBindings()
{
    auto VertexBindings = std::move(VertexShader->GetDescriptorSetLayoutBindings());
    auto PixelBindings  = std::move(PixelShader->GetDescriptorSetLayoutBindings());

    std::vector<VkDescriptorSetLayoutBinding> AllBindings;
    AllBindings.insert(AllBindings.end(), VertexBindings.begin(), VertexBindings.end());
    AllBindings.insert(AllBindings.end(), PixelBindings.begin(), PixelBindings.end());

    std::map<uint32_t, VkDescriptorSetLayoutBinding> BindingMap;

    for (size_t i = 0; i < AllBindings.size(); i++)
    {
        uint32_t Key = VertexBindings[i].binding;
        if (BindingMap.contains(Key))
        {
            if (BindingMap.at(Key).binding != AllBindings[i].binding || BindingMap.at(Key).descriptorType != AllBindings[i].descriptorType)
                throw std::runtime_error("in compatible binding reflection");
            BindingMap.at(Key).descriptorCount += AllBindings[i].descriptorCount;
            BindingMap.at(Key).stageFlags |= AllBindings[i].stageFlags;
        }
        else
        {
            BindingMap.insert({Key, AllBindings[i]});
        }
    }

    Bindings.clear();

    for (const auto& Binding : BindingMap)
    {
        Bindings.push_back(Binding.second);
    }
}

VkDescriptorSetLayoutCreateInfo VulkanGraphicsShaderProgram::GetDescriptorSetLayoutCreateInfo()
{

    VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo = {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(Bindings.size()),
        .pBindings    = Bindings.data(),
    };

    return DescriptorSetLayoutInfo;
}
