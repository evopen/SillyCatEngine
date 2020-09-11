#pragma once
#include "Engine/Platform/Platform.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <vector>

API std::vector<char> LoadFile(const std::filesystem::path& FileName, bool IsBinary);