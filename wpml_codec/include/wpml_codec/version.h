#pragma once

#ifndef WPML_CODEC_VERSION_H
#define WPML_CODEC_VERSION_H

#include <string_view>

namespace wpml_codec::version
{
constexpr std::string_view BRANCH_NAME = "main";
constexpr std::string_view COMMIT_HASH = "505319b";
constexpr std::string_view COMMIT_TIME = "2025-04-16T17:41:10+08:00";
constexpr std::string_view BUILD_TIME = "2025-04-16T18:13:14";
constexpr std::string_view BUILD_ENV = "Windows 10.0.19045 AMD64";
constexpr std::string_view BUILD_CXX_COMPILER = "MSVC 19.29.30157.0";
constexpr std::string_view BUILD_C_COMPILER = "MSVC 19.29.30157.0";
constexpr std::string_view RELEASE_VERSION = "0.0.1";
} // namespace wpml_codec::version

#endif // WPML_CODEC_VERSION_H
