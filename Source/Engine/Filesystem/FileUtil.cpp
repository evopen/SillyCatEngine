#include "Engine/pch.h"
#include "FileUtil.h"

std::vector<char> LoadFile(const std::filesystem::path& FileName, bool IsBinary)
{
    auto CurrentDir = std::filesystem::current_path();
    spdlog::info("Current directory: {}", CurrentDir.string());
    std::fstream file(FileName, std::ios::ate | std::ios::in | (IsBinary ? std::ios::binary : 0));
    if (!file.is_open())
    {
        throw std::invalid_argument("cannot open " + FileName.string());
    }
    const size_t FileSize = file.tellg();
    std::vector<char> buffer(FileSize);
    file.seekg(0);
    file.read(buffer.data(), FileSize);
    file.close();
    return buffer;
}
