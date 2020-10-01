#pragma once

#include "pch.h"

#include "Engine/Vulkan/VulkanSampler.h"

class BaseColorShading : public Sce::Shading
{
public:
    BaseColorShading(VulkanDevice* inDevice, std::shared_ptr<VulkanGraphicsShaderProgram> inShaderProgram, Sce::ShadingInfo inShadingInfo, std::shared_ptr<VulkanGraphicsPipelineState>& inPipelineState, Sce::Camera* inCamera)
        : Shading(inDevice, inShaderProgram, inShadingInfo, inPipelineState)
        , Camera(inCamera)
        , MVP_Buffer(VK_NULL_HANDLE)
        , ShaderProgram(inShaderProgram)
    {
        MVP.View       = inCamera->GetViewMatrix();
        MVP.Projection = inCamera->GetProjectionMatrix();
        MVP.Model      = glm::identity<glm::mat4>();

        MVP_Buffer = inDevice->GetMemoryManager()->CreateBuffer(&MVP, sizeof(S_MVP), VMA_MEMORY_USAGE_CPU_TO_GPU, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

        VkDescriptorBufferInfo BufferInfo = {
            .buffer = MVP_Buffer,
            .offset = 0,
            .range  = VK_WHOLE_SIZE,
        };

        BaseColorSampler.reset(new VulkanSampler(Device, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER));

        VkWriteDescriptorSet WriteDescriptorSet = {
            .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet          = inShaderProgram->GetDescriptorSetHandle(),
            .dstBinding      = 0,
            .descriptorCount = 1,
            .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo     = &BufferInfo,
        };

        vkUpdateDescriptorSets(inDevice->GetDeviceHandle(), 1, &WriteDescriptorSet, 0, nullptr);
    }

    void SetMesh(std::shared_ptr<Sce::Mesh> inMesh)
    {
        Mesh = inMesh;
        BaseColorImageView.reset(new VulkanImageView(Device, Mesh->GetMaterial()->GetDiffuseTexture()->GetVkImageHandle(Device->GetMemoryManager())));
        VkDescriptorImageInfo BaseColorImageInfo = {
            .sampler     = BaseColorSampler->GetSamplerHandle(),
            .imageView   = BaseColorImageView->GetHandle(),
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkWriteDescriptorSet WriteSampler = {
            .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet          = ShaderProgram->GetDescriptorSetHandle(),
            .dstBinding      = 1,
            .descriptorCount = 1,
            .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo      = &BaseColorImageInfo,
        };

        vkUpdateDescriptorSets(Device->GetDeviceHandle(), 1, &WriteSampler, 0, nullptr);
    }


    void UpdateDescriptor() override
    {
        MVP.View       = Camera->GetViewMatrix();
        MVP.Projection = Camera->GetProjectionMatrix();
        MVP.Model      = glm::identity<glm::mat4>();

        void* data = Device->GetMemoryManager()->MapBuffer(MVP_Buffer);
        memcpy(data, &MVP, sizeof(MVP));
        Device->GetMemoryManager()->UnMapBuffer(MVP_Buffer);
    }

    ~BaseColorShading();

private:
    struct S_MVP
    {
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Projection;
    } MVP;
    VkBuffer MVP_Buffer;
    Sce::Camera* Camera;

    std::shared_ptr<VulkanGraphicsShaderProgram> ShaderProgram;

    std::unique_ptr<VulkanSampler> BaseColorSampler;

    std::unique_ptr<VulkanImageView> BaseColorImageView;

    std::shared_ptr<Sce::Mesh> Mesh;
};

inline BaseColorShading::~BaseColorShading()
{
    if (MVP_Buffer != VK_NULL_HANDLE)
        Device->GetMemoryManager()->FreeBuffer(MVP_Buffer);
}
