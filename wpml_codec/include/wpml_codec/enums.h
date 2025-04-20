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

enum class TemplateType
{
    waypoint,     ///< 航点飞行
    mapping2d,    ///< 建图航拍
    mapping3d,    ///< 倾斜摄影
    mappingStrip, ///< 航带飞行
};

enum class CoordinateMode
{
    WGS84, ///< WGS84坐标系，当前固定使用
};

enum class HeightMode
{
    GM96,                  ///< 使用海拔高编辑
    relativeToStartPoint,  ///< 使用相对点的高度进行编辑
    aboveGroundLevel,      ///< 使用地形数据，AGL下编辑
    realTimeFollowSurface, ///< 使用实时仿地模式
};

enum class PositioningType
{
    GPS,            ///< 位置数据采集来源为GPS/BDS/GLONASS/GALILEO等
    RTKBaseStation, ///< 采集位置数据时，使用RTK基站进行差分定位
    QianXun,        ///< 采集位置数据时，使用千寻网络RTK进行差分定位
    Custom,         ///< 采集位置数据时，使用自定义网络RTK进行差分定位
};

enum class FocusMode
{
    firstPoint, ///< 首个航点自动对焦
    custom,     ///< 标定对焦值对焦
};

enum class MeteringMode
{
    average, ///< 全局测光
    spot,    ///< 点测光
};

enum class ReturnMode
{
    singleReturnStrongest, ///< 单回波
    dualReturn,            ///< 双回波
    tripleReturn,          ///< 三回波
};

enum class ScanningMode
{
    repetitive,    ///< 重复扫描
    nonRepetitive, ///< 非重复扫描
};

enum class ImageFormat
{
    wide,        ///< 存储广角镜头照片
    zoom,        ///< 存储变焦镜头照片
    ir,          ///< 存储红外镜头照片
    narrow_band, ///< 存储窄带镜头拍摄照片
    visable,     ///< 可见光照片
};

enum class GlobalWaypointTurnMode
{
    coordinateTurn,                           ///< 协调转弯，不过点，提前转弯
    toPointAndStopWithDiscontinuityCurvature, ///< 直线飞行，飞行器到点停
    toPointAndStopWithContinuityCurvature,    ///< 曲线飞行，飞行器到点停
    toPointAndPassWithContinuityCurvature,    ///< 曲线飞行，飞行器过点不停
};

enum class GimbalPitchMode
{
    manual,          ///< 手动控制
    usePointSetting, ///< 依照每个航点设置
};

enum class WaypointHeadingMode
{
    followWayline,    ///< 沿航线方向
    manually,         ///< 手动控制
    fixed,            ///< 锁定当前偏航角
    smoothTransition, ///< 自定义
    towardPOI,        ///< 朝向兴趣点
};

enum class WaypointHeadingPathMode
{
    clockwise,        ///< 顺时针旋转飞行器偏航角
    counterClockwise, ///< 逆时针旋转飞行器偏航角
    followBadArc,     ///< 沿最短路径旋转飞行器偏航角
};
}; // namespace wpml_codec::enums

namespace wce = wpml_codec::enums;

#endif // WPML_CODEC_ENUMS_H
