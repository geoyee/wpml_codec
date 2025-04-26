#pragma once

#ifndef WPML_CODEC_ENUMS_H
#define WPML_CODEC_ENUMS_H

#include <magic_enum/magic_enum.hpp>

namespace wpml_codec::enums
{
enum class ActionActuatorFunc
{
    takePhoto,          ///< 单拍
    startRecord,        ///< 开始录像
    stopRecord,         ///< 结束录像
    focus,              ///< 对焦
    zoom,               ///< 变焦
    customDirName,      ///< 创建新文件夹
    gimbalRotate,       ///< 旋转云台
    rotateYaw,          ///< 飞行器偏航
    hover,              ///< 悬停等待
    gimbalEvenlyRotate, ///< 航段间均匀转动云台pitch角
    accurateShoot,      ///< 精准复拍动作（已暂停维护，建议使用orientedShoot）
    orientedShoot,      ///< 定向拍照动作
    panoShot,           ///< 全景拍照动作
    recordPointCloud    ///< 点云录制操作
};

enum class ActionGroupMode
{
    sequence, ///< 串行执行
};

enum class ActionTriggerType
{
    reachPoint,            ///< 到达航点时执行
    betweenAdjacentPoints, ///< 航段触发，均匀转云台
    multipleTiming,        ///< 等时触发
    multipleDistance,      ///< 等距触发
};

enum class AircraftPathMode
{
    clockwise,        ///< 顺时针旋转
    counterClockwise, ///< 逆时针旋转
};

enum class CoordinateMode
{
    WGS84, ///< WGS84坐标系，当前固定使用
};

enum class ExecuteHeightMode
{
    WGS84,                 ///< 椭球高模式
    relativeToStartPoint,  ///< 相对起飞点高度模式
    realTimeFollowSurface, ///< 使用实时仿地模式
};

enum class ExecuteRCLostAction
{
    goBack,  ///< 返航
    landing, ///< 降落
    hover,   ///< 悬停
};

enum class ExitOnRCLost
{
    goContinue,        ///< 继续执行航线
    executeLostAction, ///< 退出航线，执行失控动作
};

enum class FinishAction
{
    goHome,            ///< 退出航线模式并返航
    noAction,          ///< 退出航线模式
    autoLand,          ///< 退出航线模式并原地降落
    gotoFirstWaypoint, ///< 立即飞向航线起始点，到达后退出航线模式
};

enum class FlyToWaylineMode
{
    safely,       ///< 安全模式
    pointToPoint, ///< 倾斜飞行模式
};

enum class FocusMode
{
    firstPoint, ///< 首个航点自动对焦
    custom,     ///< 标定对焦值对焦
};

enum class GimbalHeadingYawBase
{
    north, ///< 相对地理北
};

enum class GimbalPitchMode
{
    manual,          ///< 手动控制
    usePointSetting, ///< 依照每个航点设置（仅航点飞行）
    fixed,           ///< 固定为用户设置的俯仰角（仅建图航拍）
};

enum class GimbalRotateMode
{
    absoluteAngle, ///< 绝对角度，相对于正北方的角度
};

enum class HeightMode
{
    EGM96,                 ///< 使用海拔高编辑
    relativeToStartPoint,  ///< 使用相对点的高度进行编辑
    aboveGroundLevel,      ///< 使用地形数据，AGL下编辑
    realTimeFollowSurface, ///< 使用实时仿地模式
};

enum class ImageFormat
{
    wide,        ///< 存储广角镜头照片
    zoom,        ///< 存储变焦镜头照片
    ir,          ///< 存储红外镜头照片
    narrow_band, ///< 存储窄带镜头拍摄照片
    visable,     ///< 可见光照片
};

using PayloadLensIndex = ImageFormat;

enum class MappingHeadingMode
{
    fixed,         ///< 固定为用户设置的偏航角
    followWayline, ///< 偏航角跟随航线
};

enum class MeteringMode
{
    average, ///< 全局测光
    spot,    ///< 点测光
};

enum class OrientedPhotoMode
{
    normalPhoto,           ///< 普通拍照
    lowLightSmartShooting, ///< 智能低光拍照
};

enum class PanoShotSubMode
{
    panoShot_360, ///< 全景模式
};

enum class PositioningType
{
    GPS,            ///< 位置数据采集来源为GPS/BDS/GLONASS/GALILEO等
    RTKBaseStation, ///< 采集位置数据时，使用RTK基站进行差分定位
    QianXun,        ///< 采集位置数据时，使用千寻网络RTK进行差分定位
    Custom,         ///< 采集位置数据时，使用自定义网络RTK进行差分定位
};

enum class RecordPointCloudOperate
{
    startRecord,  ///< 开始点云录制
    pauseRecord,  ///< 暂停点云录制
    resumeRecord, ///< 继续点云录制
    stopRecord,   ///< 结束点云录制
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

enum class ShootType
{
    time,     ///< 等时间拍照
    distance, ///< 等间隔拍照
};

enum class TemplateType
{
    waypoint,     ///< 航点飞行
    mapping2d,    ///< 建图航拍
    mapping3d,    ///< 倾斜摄影
    mappingStrip, ///< 航带飞行
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

enum class WaypointTurnMode
{
    coordinateTurn,                           ///< 协调转弯，不过点，提前转弯
    toPointAndStopWithDiscontinuityCurvature, ///< 直线飞行，飞行器到点停
    toPointAndStopWithContinuityCurvature,    ///< 曲线飞行，飞行器到点停
    toPointAndPassWithContinuityCurvature,    ///< 曲线飞行，飞行器过点不停
};

using GlobalWaypointTurnMode = WaypointTurnMode;

}; // namespace wpml_codec::enums

namespace wce = wpml_codec::enums;

#endif // WPML_CODEC_ENUMS_H
