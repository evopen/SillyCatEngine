#pragma once

#include "pch.h"

class VertexColorShading : public Sce::Shading
{
public:
    VertexColorShading(VulkanDevice* inDevice, std::shared_ptr<VulkanGraphicsShaderProgram> inShaderProgram, Sce::ShadingInfo inShadingInfo, std::shared_ptr<VulkanGraphicsPipelineState>& inPipelineState, Sce::Camera* inCamera)
        : Shading(inDevice, inShaderProgram, inShadingInfo, inPipelineState)
        , Camera(inCamera)
        , MVP_Buffer(VK_NULL_HANDLE)
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

    void UpdateDescriptor() override
    {
        MVP.View       = Camera->GetViewMatrix();
        MVP.Projection = Camera->GetProjectionMatrix();
        MVP.Model      = glm::identity<glm::mat4>();

        void* data = Device->GetMemoryManager()->MapBuffer(MVP_Buffer);
        memcpy(data, &MVP, sizeof(MVP));
        Device->GetMemoryManager()->UnMapBuffer(MVP_Buffer);
    }

    ~VertexColorShading();

private:
    struct S_MVP
    {
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Projection;
    } MVP;
    VkBuffer MVP_Buffer;
    Sce::Camera* Camera;
};

inline VertexColorShading::~VertexColorShading()
{
    if (MVP_Buffer != VK_NULL_HANDLE)
        Device->GetMemoryManager()->FreeBuffer(MVP_Buffer);
}
