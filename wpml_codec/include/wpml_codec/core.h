#pragma once

#ifndef WPML_CODEC_CORE_H
#define WPML_CODEC_CORE_H

#include <wpml_codec/common.h>
#include <wpml_codec/structs.h>

namespace wpml_codec::core
{
using namespace wpml_codec::structs;

inline std::optional<Document> LIB_API parseKML(const std::string& kmlPath);
} // namespace wpml_codec::core

#endif // WPML_CODEC_CORE_H
