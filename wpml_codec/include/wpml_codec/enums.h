#pragma once

#ifndef WPML_CODEC_ENUMS_H
#define WPML_CODEC_ENUMS_H

#include <magic_enum/magic_enum.hpp>

namespace wpml_codec::enums
{
enum class FlyToWaylineMode
{
    safely,       ///< 安全模式
    pointToPoint, ///< 倾斜飞行模式
};

enum class FinishAction
{
    goHome,            ///< 退出航线模式并返航
    noAction,          ///< 退出航线模式
    autoLand,          ///< 退出航线模式并原地降落
    gotoFirstWaypoint, ///< 立即飞向航线起始点，到达后退出航线模式
};

enum class ExitOnRCLost
{
    goContinue,        ///< 继续执行航线
    executeLostAction, ///< 退出航线，执行失控动作
};

enum class ExecuteRCLostAction
{
    goBack,  ///< 返航
    landing, ///< 降落
    hover,   ///< 悬停
};
} // namespace wpml_codec::enums

#endif // WPML_CODEC_ENUMS_H
