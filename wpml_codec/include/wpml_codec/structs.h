#pragma once

#ifndef WPML_CODEC_STRUCTS_H
#define WPML_CODEC_STRUCTS_H

#include <wpml_codec/common.h>
#include <wpml_codec/enums.h>
#include <optional>

namespace wpml_codec::structs
{
using namespace wpml_codec::enums;

struct LIB_API Point
{
    double lat; ///< 纬度
    double lon; ///< 经度
    double alt; ///< 高度
};

struct LIB_API DroneInfo
{
    int droneEnumValue;    ///< 飞行器机型主类型
    int droneSubEnumValue; ///< 飞行器机型子类型
};

struct LIB_API PayloadInfo
{
    int payloadEnumValue;     ///< 负载机型主类型
    int payloadPositionIndex; ///< 负载挂载位置
};

struct LIB_API AutoRerouteInfo
{
    int missionAutoRerouteMode;      ///< 任务航线绕行模式
    int transitionalAutoRerouteMode; ///< 过渡航线绕行模式
};

struct LIB_API MissionConfig
{
    FlyToWaylineMode flyToWaylineMode;                      ///< 飞向首航点模式
    FinishAction finishAction;                              ///< 航线结束动作
    ExitOnRCLost exitOnRCLost;                              ///< 失控是否继续执行航线
    std::optional<ExecuteRCLostAction> executeRCLostAction; ///< 失控动作类型
    double takeOffSecurityHeight;                           ///< 安全起飞高度
    double globalTransitionalSpeed;                         ///< 全局航线过渡速度
    double globalRTHHeight;                                 ///< 全局返航高度
    std::optional<Point> takeOffRefPoint;                   ///< 参考起飞点
    std::optional<double> takeOffRefPointAGLHeight;         ///< 参考起飞点海拔高度
    DroneInfo droneInfo;                                    ///< 飞行器机型信息
    PayloadInfo payloadInfo;                                ///< 负载机型信息
    AutoRerouteInfo autoRerouteInfo;                        ///< 航线绕行
};

struct LIB_API WaylineCoordinateSysParam
{
    CoordinateMode coordinateMode;                  ///< 经纬度坐标系
    HeightMode heightMode;                          ///< 航点高程参考平面
    std::optional<PositioningType> positioningType; ///< 经纬度与高度数据源
    double globalShootHeight;                       ///< 飞行器离被摄面高度（相对地面高）
    int surfaceFollowModeEnable;                    ///< 是否开启仿地飞行
    double surfaceRelativeHeight;                   ///< 仿地飞行离地高度（相对地面高）
};

struct LIB_API PayloadParam
{
    int payloadPositionIndex;                 ///< 负载挂载位置
    std::optional<FocusMode> focusMode;       ///< 负载对焦模式
    std::optional<MeteringMode> meteringMode; ///< 负载测光模式
    std::optional<int> dewarpingEnable;       ///< 是否开启畸变矫正
    std::optional<ReturnMode> returnMode;     ///< 激光雷达回波模式
    std::optional<int> samplingRate;          ///< 负载采样率
    std::optional<ScanningMode> scanningMode; ///< 负载扫描模式
    std::optional<int> modelColoringEnable;   ///< 真彩上色
    std::vector<ImageFormat> imageFormat;     ///< 图片格式列表
};

struct LIB_API GlobalWaypointHeadingParam
{
    WaypointHeadingMode waypointHeadingMode;         ///< 飞行器偏航角模式
    double waypointHeadingAngle;                     ///< 飞行器偏航角度
    Point waypointPoiPoint;                          ///< 兴趣点
    WaypointHeadingPathMode waypointHeadingPathMode; ///< 飞行器偏航角转动方向
};

struct Placemark
{
    // TODO: Implement
};

struct LIB_API Folder
{
    /* 通用模板 */
    TemplateType templateType;                           ///< 预定义模板类型
    int templateId;                                      ///< 模板ID
    double autoFlightSpeed;                              ///< 全局航线飞行速度
    WaylineCoordinateSysParam waylineCoordinateSysParam; ///< 坐标系参数
    PayloadParam payloadParam;                           ///< 负载设置
    /* 航点飞行模板元素 */
    GlobalWaypointTurnMode globalWaypointTurnMode;         ///< 全局航点类型（全局航点转弯模式）
    int globalUseStraightLine;                             ///< 全局航段轨迹是否尽量贴合直线
    GimbalPitchMode gimbalPitchMode;                       ///< 云台俯仰角控制模式
    double globalHeight;                                   ///< 全局航线高度（相对起飞点高度）
    GlobalWaypointHeadingParam globalWaypointHeadingParam; ///< 全局偏航角模式参数
    Placemark placemark;                                   ///< 航点信息
};

struct LIB_API Document
{
    std::optional<std::string> author;   ///< 文件创建作者
    std::optional<long long> createTime; ///< 文件创建时间（Unix Timestamp）
    std::optional<long long> updateTime; ///< 文件更新时间（Unix Timestamp）
    MissionConfig missionConfig;         ///< 任务信息
    Folder folder;                       ///< 模板信息
};
} // namespace wpml_codec::structs

#endif // WPML_CODEC_STRUCTS_H
