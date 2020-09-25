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

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <set>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <optional>
#include <thread>