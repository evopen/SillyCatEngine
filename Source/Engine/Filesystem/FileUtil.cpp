#include "FileUtil.h"

std::vector<char> LoadFile(const std::filesystem::path& FileName, bool IsBinary)
{
    auto CurrentDir = std::filesystem::current_path();
    spdlog::info("Current directory: {}", CurrentDir.string());
    std::fstream file(FileName, std::ios::ate | std::ios::in | (IsBinary ? std::ios::binary : 0));
    if (!file.is_open())
    {
        char err[80];
        strerror_s(err, 80, errno);
        spdlog::error("Error: {}", err);
    }
    const size_t FileSize = file.tellg();
    std::vector<char> buffer(FileSize);
    file.seekg(0);
    file.read(buffer.data(), FileSize);
    file.close();
    return buffer;
}
