#include "pch.h"

#include "Shader/BaseColor/BaseColor.h"
#include "Shader/VertexColor/VertexColor.h"
#include "Shader/WireFrame/Wireframe.h"


struct SUiStatus
{
    bool bNeedLoadModel                = false;
    bool bShaderNeedUpdate             = true;
    size_t currentShadingIndex         = 0;
    spdlog::level::level_enum loglevel = spdlog::level::info;
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

void DrawUI(VulkanCommandBuffer* cmdBuffer, const std::shared_ptr<Sce::GUI>& gui, const std::vector<Sce::ShadingInfo>& ShadingList, const Sce::Camera& inCamera, SUiStatus& status)
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
        if (ImGui::BeginMenu("Option"))
        {
            if (ImGui::BeginMenu("Log Level"))
            {
                static std::array<bool, 7> enabled;
                static const std::vector<std::string> Names = SPDLOG_LEVEL_NAMES;
                for (size_t i = 0; i < enabled.size(); i++)
                {
                    if (ImGui::MenuItem(Names[i].c_str(), nullptr, &enabled[i]))
                    {
                        std::for_each(enabled.begin(), enabled.end(), [](bool& enable) { enable = false; });
                        enabled[i]      = true;
                        status.loglevel = static_cast<spdlog::level::level_enum>(i);
                        Sce::Logger::Get()->set_level(status.loglevel);
                    }
                }
                ImGui::EndMenu();
            }
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
        if (ImGui::Button(ShadingList[i].name.c_str()))
        {
            status.currentShadingIndex = i;
            status.bShaderNeedUpdate   = true;
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();
    }

    ImGui::End();

    ImGui::SetNextWindowPos({50, 200}, ImGuiCond_Once);
    ImGui::Begin("infos", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

    ImGui::Text("Camera Position: %s", glm::to_string(inCamera.GetPosition()).c_str());
    ImGui::Text("Camera Front: %s", glm::to_string(inCamera.GetFront()).c_str());
    ImGui::Text("Camera Pitch: %f", inCamera.GetPitch());
    ImGui::Text("Camera Yaw: %f", inCamera.GetYaw());

    ImGui::End();

    gui->Render(cmdBuffer);
}


std::vector<Sce::ShadingInfo> LoadShadingList(std::filesystem::path inShadingDir)
{
    std::vector<Sce::ShadingInfo> result;

    for (auto& p : std::filesystem::recursive_directory_iterator(inShadingDir))
    {
        if (p.path().extension() == ".json")
        {
            result.emplace_back(Sce::ShadingInfo::LoadShadingInfo(p));
        }
    }
    return std::move(result);
}

int main()
{
    const std::filesystem::path ShaderBasePath = "App/ModelRenderer/Source/Shader";
    try
    {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        spdlog::register_logger(Sce::Logger::Setup(sink));

        auto ShadingList = LoadShadingList(ShaderBasePath);

        auto instance = std::make_shared<VulkanInstance>(false, true);

        auto [physicalDevice, physicalDeviceProperties] = VulkanDevice::SelectPhysicalDevice(instance);
        VulkanDevice device(instance, physicalDevice, false);

        auto windowSurface = std::make_shared<VulkanWindowSurface>(instance, &device, "SillyCatRenderer", 1280, 960);
        VulkanSwapchain swapchain(instance, &device, windowSurface);
        VulkanPresenter presenter(device.GetPresentQueue(), &swapchain);
        VulkanCommandBuffer cmdBuffer(&device, device.GetGraphicsQueue());
        std::shared_ptr<VulkanVertexShader> vertexShader;
        std::shared_ptr<VulkanPixelShader> pixelShader;
        std::shared_ptr<VulkanGraphicsShaderProgram> shaderProgram;
        std::shared_ptr<VulkanGraphicsPipelineState> pipelineState;
        std::shared_ptr<Sce::Shading> shading;
        VulkanRenderTargetLayout RTLayout(1, true);
        auto renderPass = std::make_shared<VulkanRenderPass>(&device, &RTLayout);
        std::shared_ptr<VulkanGraphicsPipeline> pipeline;
        VulkanSemaphore ImageAvailable(&device);
        VulkanSemaphore RenderFinish(&device);
        auto gui = std::make_shared<Sce::GUI>(instance, &device, windowSurface, renderPass, swapchain.GetImageCount());
        ImGui::SetCurrentContext(gui->GetContext());
        std::shared_ptr<Sce::Model> ModelLoaded;
        Sce::Camera Camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0));
        SUiStatus uiStatus;
        windowSurface->InstallCursorCallback(std::bind(&Sce::Camera::CursorPosCallback, &Camera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        Sce::Input Input(windowSurface);
        Input.LinkKeyboardCameraMovement(&Camera, 0.02f);

        VkImage depthImage  = device.GetMemoryManager()->CreateImage(VkExtent3D{
                                                                        .width  = windowSurface->GetWidth(),
                                                                        .height = windowSurface->GetHeight(),
                                                                        .depth  = 1},
            VK_FORMAT_D32_SFLOAT, VMA_MEMORY_USAGE_GPU_ONLY, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        auto depthImageView = std::make_shared<VulkanImageView>(&device, depthImage);


        while (!glfwWindowShouldClose(windowSurface->GetWindowHandle()))
        {
            glfwPollEvents();
            Input.ProcessKeyboard();

            {
                if (uiStatus.bNeedLoadModel)
                {
                    LoadModel(ModelLoaded, device.GetMemoryManager());
                    uiStatus.bNeedLoadModel = false;
                }
                if (uiStatus.bShaderNeedUpdate)
                {
                    vertexShader.reset(new VulkanVertexShader(&device, ShadingList[uiStatus.currentShadingIndex].vertex_shader));
                    pixelShader.reset(new VulkanPixelShader(&device, ShadingList[uiStatus.currentShadingIndex].fragment_shader));
                    shaderProgram.reset(new VulkanGraphicsShaderProgram(&device, vertexShader, pixelShader));
                    pipelineState.reset(new VulkanGraphicsPipelineState(shaderProgram));

                    if (ShadingList[uiStatus.currentShadingIndex].name == "Vertex Color")
                        shading.reset(new VertexColorShading(&device, shaderProgram, ShadingList[uiStatus.currentShadingIndex], pipelineState, &Camera));
                    else if (ShadingList[uiStatus.currentShadingIndex].name == "Base Color")
                        shading.reset(new BaseColorShading(&device, shaderProgram, ShadingList[uiStatus.currentShadingIndex], pipelineState, &Camera));
                    else if (ShadingList[uiStatus.currentShadingIndex].name == "Wireframe")
                        shading.reset(new WireframeShading(&device, shaderProgram, ShadingList[uiStatus.currentShadingIndex], pipelineState, &Camera));

                    pipeline.reset(new VulkanGraphicsPipeline(&device, renderPass, pipelineState));

                    uiStatus.bShaderNeedUpdate = false;
                }
            }

            std::vector<std::shared_ptr<VulkanFramebuffer>> framebuffers;
            std::vector<std::shared_ptr<VulkanImageView>> imageViews;
            for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
            {
                imageViews.emplace_back(std::make_shared<VulkanImageView>(&device, swapchain.GetImage(i)));
                framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(&device, renderPass, std::vector<std::shared_ptr<VulkanImageView>>{imageViews[i], depthImageView}, windowSurface->GetWidth(), windowSurface->GetHeight()));
            }


            swapchain.AcquireNextImage(ImageAvailable.GetHandle(), VK_NULL_HANDLE);

            cmdBuffer.Wait();
            cmdBuffer.Begin();
            renderPass->Begin(&cmdBuffer, framebuffers[swapchain.GetRenderIndex()], {{0.3f, 0.3f, 0.3f, 1.0f},{1.0f, 0}});
            VkViewport Viewport = windowSurface->GetSurfaceViewport();
            VkRect2D Scissor    = windowSurface->GetSurfaceScissor();
            vkCmdSetViewport(cmdBuffer.GetHandle(), 0, 1, &Viewport);
            vkCmdSetScissor(cmdBuffer.GetHandle(), 0, 1, &Scissor);
            pipeline->Bind(&cmdBuffer);

            if (ModelLoaded != nullptr)
            {
                for (size_t i = 0; i < ModelLoaded->GetMeshCount(); i++)
                {
                    auto mesh             = ModelLoaded->GetMesh(i);
                    VkBuffer VertexBuffer = mesh->GetVertexBuffer();
                    VkBuffer ColorBuffer;
                    if (ShadingList[uiStatus.currentShadingIndex].name == "Base Color")
                    {
                        std::dynamic_pointer_cast<BaseColorShading>(shading)->SetMesh(mesh);
                        ColorBuffer = mesh->GetTextureCoordBuffer();
                    }
                    else
                    {
                        ColorBuffer = mesh->GetColorBuffer();
                    }

                    std::vector<VkDeviceSize> Offsets(2, 0);
                    std::vector<VkBuffer> VertexBuffers = {VertexBuffer, ColorBuffer};
                    vkCmdBindVertexBuffers(cmdBuffer.GetHandle(), 0, static_cast<uint32_t>(VertexBuffers.size()), VertexBuffers.data(), Offsets.data());
                    vkCmdBindIndexBuffer(cmdBuffer.GetHandle(), mesh->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
                    VkDescriptorSet descriptorSet = shaderProgram->GetDescriptorSetHandle();
                    shading->UpdateDescriptor();
                    shading->BindDescriptorSet(&cmdBuffer);
                    vkCmdDrawIndexed(cmdBuffer.GetHandle(), mesh->GetIndexCount(), 1, 0, 0, 0);
                }
                cmdBuffer.PossessObject(ModelLoaded);
            }

            DrawUI(&cmdBuffer, gui, ShadingList, Camera, uiStatus);

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