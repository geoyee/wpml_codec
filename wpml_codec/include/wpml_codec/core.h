#pragma once

#ifndef WPML_CODEC_CORE_H
#define WPML_CODEC_CORE_H

#include <wpml_codec/common.h>
#include <wpml_codec/structs.h>

namespace wpml_codec::core
{
inline std::optional<wcs::KMLDocument> LIB_API parseKML(const std::string& kmlPath);
inline bool LIB_API createKML(const wcs::KMLDocument& data, const std::string& kmlPath);
inline std::optional<wcs::WPMLDocument> LIB_API parseWPML(const std::string& wpmlPath);
inline bool LIB_API createWPML(const wcs::WPMLDocument& data, const std::string& wpmlPath);
inline std::optional<wcs::WPMLData> LIB_API parseKMZOfDJI(const std::string& kmzPath);
inline bool LIB_API createKMZOfDJI(const wcs::WPMLData& data, const std::string& kmzPath);
}; // namespace wpml_codec::core

namespace wcc = wpml_codec::core;

#endif // WPML_CODEC_CORE_H
