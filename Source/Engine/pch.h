#pragma once

#include <SPIRV/GlslangToSpv.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glslang/Public/ShaderLang.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vk_mem_alloc.h>
#include <stb_image.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <nlohmann/json.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <set>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <optional>
#include <thread>