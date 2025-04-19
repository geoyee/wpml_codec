#pragma once

#ifndef WPML_CODEC_CORE_H
#define WPML_CODEC_CORE_H

#include <wpml_codec/common.h>
#include <wpml_codec/enums.h>
#include <optional>

namespace wpml_codec::core
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

struct LIB_API Document
{
    std::optional<std::string> author;   ///< 文件创建作者
    std::optional<long long> createTime; ///< 文件创建时间（Unix Timestamp）
    std::optional<long long> updateTime; ///< 文件更新时间（Unix Timestamp）
    MissionConfig missionConfig;         ///< 任务信息
};

inline std::optional<Document> LIB_API parseKML(const std::string& kmlPath);
} // namespace wpml_codec::core

#endif // WPML_CODEC_CORE_H
