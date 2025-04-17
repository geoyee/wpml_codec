#pragma once

#ifndef WPML_CODEC_UTILS_H
#define WPML_CODEC_UTILS_H

#include <wpml_codec/common.h>

namespace wpml_codec::utils
{
inline bool LIB_API extractKMZ(const std::string& kmzPath, const std::string& outputDir);
inline bool LIB_API packageKMZ(const std::string& inputDir, const std::string& kmzPath);
} // namespace wpml_codec::utils

#endif // WPML_CODEC_UTILS_H
