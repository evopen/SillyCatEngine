#include "pch.h"

#include "Shader/BaseColor.h"

struct SShading
{
    std::string Name;
    std::filesystem::path Vertex;
    std::filesystem::path Fragment;
};

struct SUiStatus
{
    bool bNeedLoadModel        = false;
    bool bShaderNeedUpdate     = true;
    size_t currentShadingIndex = 0;
};

void LoadModel(std::shared_ptr<Sce::Model>& Model, VulkanMemoryManager* MemoryManager)
{
    std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
    nfdresult_t result                  = NFD_OpenDialog(nullptr, nullptr, outPath.get());
    if (result == NFD_OKAY)
    {
        spdlog::info("Opening {}", *outPath);
        Model.reset(new Sce::Model(*outPath, MemoryManager));
    }
}

void DrawUI(VulkanCommandBuffer* cmdBuffer, const std::shared_ptr<Sce::GUI>& gui, const std::vector<SShading>& ShadingList, SUiStatus& status)
{
    ImGuiIO& io = ImGui::GetIO();
    gui->NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Open", nullptr, &status.bNeedLoadModel);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

    for (int i = 0; i < ShadingList.size(); ++i)
    {
        ImGui::PushID(i);
        if (i == status.currentShadingIndex)
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor(20, 100, 240).Value);
        else
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor(105, 110, 135).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(10, 80, 180).Value);
        if (ImGui::Button(ShadingList[i].Name.c_str()))
        {
            status.currentShadingIndex = i;
            status.bShaderNeedUpdate   = true;
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();
    }

    ImGui::End();
    gui->Render(cmdBuffer);
}


std::vector<SShading> LoadShadingList(std::filesystem::path inJsonPath)
{
    std::vector<SShading> result;
    std::ifstream ifs(inJsonPath);
    auto j = nlohmann::json::parse(ifs).at("ShadingList");
    result.resize(j.size());
    for (size_t i = 0; i < j.size(); i++)
    {
        result[i].Name     = j[i].at("Name").get<std::string>();
        result[i].Vertex   = j[i].at("Vertex").get<std::string>();
        result[i].Fragment = j[i].at("Fragment").get<std::string>();
    }
    return std::move(result);
}

int main()
{
    const std::filesystem::path ShaderBasePath = "App/ModelRenderer/Source/Shader";
    try
    {
        auto ShadingList = LoadShadingList(ShaderBasePath / "ShadingList.json");

        auto instance = std::make_shared<VulkanInstance>(false, true);

        auto [physicalDevice, physicalDeviceProperties] = VulkanDevice::SelectPhysicalDevice(instance);
        VulkanDevice device(instance, physicalDevice, false);

        auto windowSurface = std::make_shared<VulkanWindowSurface>(instance, &device, "SillyCatRenderer", 800, 600);
        VulkanSwapchain swapchain(instance, &device, windowSurface);
        VulkanPresenter presenter(device.GetPresentQueue(), &swapchain);
        VulkanCommandBuffer cmdBuffer(&device, device.GetGraphicsQueue());
        std::shared_ptr<VulkanVertexShader> vertexShader;
        std::shared_ptr<VulkanPixelShader> pixelShader;
        std::shared_ptr<VulkanGraphicsShaderProgram> shaderProgram;
        std::shared_ptr<VulkanGraphicsPipelineState> pipelineState;
        std::shared_ptr<Shading> shading;
        VulkanRenderTargetLayout RTLayout(1);
        auto renderPass = std::make_shared<VulkanRenderPass>(&device, &RTLayout);
        std::shared_ptr<VulkanGraphicsPipeline> pipeline;
        VulkanSemaphore ImageAvailable(&device);
        VulkanSemaphore RenderFinish(&device);
        auto gui = std::make_shared<Sce::GUI>(instance, &device, windowSurface, renderPass, swapchain.GetImageCount());
        ImGui::SetCurrentContext(gui->GetContext());
        std::shared_ptr<Sce::Model> ModelLoaded;
        Sce::Camera Camera(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0));
        SUiStatus uiStatus;

        while (!glfwWindowShouldClose(windowSurface->GetWindowHandle()))
        {
            glfwPollEvents();

            {
                if (uiStatus.bNeedLoadModel)
                {
                    LoadModel(ModelLoaded, device.GetMemoryManager());
                    uiStatus.bNeedLoadModel = false;
                }
                if (uiStatus.bShaderNeedUpdate)
                {
                    vertexShader.reset(new VulkanVertexShader(&device, ShaderBasePath / ShadingList[uiStatus.currentShadingIndex].Vertex));
                    pixelShader.reset(new VulkanPixelShader(&device, ShaderBasePath / ShadingList[uiStatus.currentShadingIndex].Fragment));
                    shaderProgram.reset(new VulkanGraphicsShaderProgram(&device, vertexShader, pixelShader));
                    pipelineState.reset(new VulkanGraphicsPipelineState(shaderProgram));
                    pipeline.reset(new VulkanGraphicsPipeline(&device, renderPass, pipelineState));

                    if (ShadingList[uiStatus.currentShadingIndex].Name == "Base Color")
                        shading.reset(new ShadingBaseColor(&device, shaderProgram, Camera));

                    uiStatus.bShaderNeedUpdate = false;
                }
            }

            std::vector<std::shared_ptr<VulkanFramebuffer>> framebuffers;
            std::vector<std::shared_ptr<VulkanImageView>> swapchainImageViews;
            for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
            {
                swapchainImageViews.emplace_back(std::make_shared<VulkanImageView>(&device, swapchain.GetImage(i)));
                framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(&device, renderPass, std::vector<std::shared_ptr<VulkanImageView>>{swapchainImageViews[i]}, windowSurface->GetWidth(), windowSurface->GetHeight()));
            }

            swapchain.AcquireNextImage(ImageAvailable.GetHandle(), VK_NULL_HANDLE);

            cmdBuffer.Wait();
            cmdBuffer.Begin();
            renderPass->Begin(&cmdBuffer, framebuffers[swapchain.GetRenderIndex()], {{0.3f, 0.3f, 0.3f, 1.0f}});
            VkViewport Viewport = windowSurface->GetSurfaceViewport();
            VkRect2D Scissor    = windowSurface->GetSurfaceScissor();
            vkCmdSetViewport(cmdBuffer.GetHandle(), 0, 1, &Viewport);
            vkCmdSetScissor(cmdBuffer.GetHandle(), 0, 1, &Scissor);
            pipeline->Bind(&cmdBuffer);

            if (ModelLoaded != nullptr)
            {
                for (size_t i = 0; i < ModelLoaded->GetMeshCount(); i++)
                {
                    VkBuffer VertexBuffer = ModelLoaded->GetVertexBuffer();
                    VkBuffer ColorBuffer  = ModelLoaded->GetColorBuffer();
                    std::vector<VkDeviceSize> Offsets(2, 0);
                    std::vector<VkBuffer> VertexBuffers = {VertexBuffer, ColorBuffer};
                    vkCmdBindVertexBuffers(cmdBuffer.GetHandle(), 0, static_cast<uint32_t>(VertexBuffers.size()), VertexBuffers.data(), Offsets.data());
                    VkDescriptorSet descriptorSet = shaderProgram->GetDescriptorSetHandle();
                    vkCmdBindDescriptorSets(cmdBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, shaderProgram->GetPipelineLayoutHandle(), 0, 1, &descriptorSet, 0, nullptr);
                    vkCmdDraw(cmdBuffer.GetHandle(), ModelLoaded->GetVertexCount(), 1, 0, 0);
                }
            }

            DrawUI(&cmdBuffer, gui, ShadingList, uiStatus);

            renderPass->End();
            swapchain.CmdTransitImagePresentSrc(&cmdBuffer);
            cmdBuffer.End();
            cmdBuffer.Submit({ImageAvailable.GetHandle()}, {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT}, {RenderFinish.GetHandle()});
            presenter.Present({RenderFinish.GetHandle()});
        }

        return 0;
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }
}