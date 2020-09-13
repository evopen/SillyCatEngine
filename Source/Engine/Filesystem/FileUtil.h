#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"


API std::vector<char> LoadFile(const std::filesystem::path& FileName, bool IsBinary);