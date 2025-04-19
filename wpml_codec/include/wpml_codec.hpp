#pragma once

#ifndef WPML_CODEC_HPP
#define WPML_CODEC_HPP

#include <wpml_codec/version.h>
#include <wpml_codec/core.h>
#include <wpml_codec/utils.h>

namespace wcc = wpml_codec::core;
namespace wcu = wpml_codec::utils;
namespace wcv = wpml_codec::version;

#define WPML_CODEC_VERSION() wcv::display()

#endif // !WPML_CODEC_HPP
