#pragma once

#ifndef WPML_CODEC_CORE_H
#define WPML_CODEC_CORE_H

#include <wpml_codec/common.h>
#include <wpml_codec/structs.h>

namespace wpml_codec::core
{
inline std::optional<wcs::KMLDocument> LIB_API parseKML(const std::string& kmlPath);
inline bool LIB_API creatKML(const wcs::KMLDocument& data, const std::string& kmlPath);
}; // namespace wpml_codec::core

namespace wcc = wpml_codec::core;

#endif // WPML_CODEC_CORE_H
