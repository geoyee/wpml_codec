#pragma once

#ifndef WPML_CODEC_UTILS_H
#define WPML_CODEC_UTILS_H

#include <wpml_codec/common.h>
#include <optional>
#include <vector>

namespace wpml_codec::utils
{
/**********************************************************************/
/*                               Files                                */
/**********************************************************************/

inline bool LIB_API extractKMZ(const std::string& kmzPath, const std::string& outputDir);
inline bool LIB_API packageKMZ(const std::string& inputDir, const std::string& kmzPath);
inline std::vector<std::string> LIB_API findFiles(const std::string& directory);

/**********************************************************************/
/*                               String                               */
/**********************************************************************/

inline std::string LIB_API toLower(const std::string& str);
inline std::string LIB_API toUpper(const std::string& str);
inline std::string LIB_API trim(const std::string& str);
inline bool LIB_API equal(const std::string& str, const std::string& suffix, bool ignoreCase = false);
inline bool LIB_API startWith(const std::string& str, const std::string& suffix, bool ignoreCase = false);
inline bool LIB_API endWith(const std::string& str, const std::string& suffix, bool ignoreCase = false);
inline std::vector<std::string> LIB_API split(const std::string& str, const std::string& separator);
inline std::string LIB_API merge(const std::vector<std::string>& strVec, const std::string& separator = "");
inline std::string LIB_API replace(const std::string& str, const std::string& oldSuffix, const std::string& newSuffix);
inline std::optional<bool> LIB_API toBool(const std::string& str);
inline std::optional<int> LIB_API toInt(const std::string& str);
inline std::optional<long long> LIB_API toLongLong(const std::string& str);
inline std::optional<double> LIB_API toDouble(const std::string& str, size_t decimal = 8);
inline std::vector<double> LIB_API toDoubles(const std::string& str, size_t decimal = 8);
inline std::string LIB_API toString(bool number);
inline std::string LIB_API toString(int number);
inline std::string LIB_API toString(long long number);
inline std::string LIB_API toString(double number, size_t decimal = 8, bool clipZero = true);
inline std::string LIB_API toString(std::vector<double> numbers, size_t decimal = 8, bool clipZero = true);
inline std::string LIB_API escape(const std::string& str);
inline std::string LIB_API removeEscape(const std::string& str);
}; // namespace wpml_codec::utils

namespace wcu = wpml_codec::utils;

#endif // WPML_CODEC_UTILS_H
