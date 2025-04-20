#pragma once

#ifndef WPML_CODEC_HPP
#define WPML_CODEC_HPP

#include <wpml_codec/core.h>
#include <wpml_codec/enums.h>
#include <wpml_codec/utils.h>
#include <wpml_codec/version.h>

namespace wcc = wpml_codec::core;
namespace wce = wpml_codec::enums;
namespace wcu = wpml_codec::utils;
namespace wcv = wpml_codec::version;

#define WPML_CODEC_VERSION() wcv::display()

#endif // !WPML_CODEC_HPP
