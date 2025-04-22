#pragma once

#ifndef WPML_CODEC_STRUCTS_H
#define WPML_CODEC_STRUCTS_H

#include <wpml_codec/common.h>
#include <wpml_codec/enums.h>
#include <optional>

namespace wpml_codec::structs
{
struct LIB_API Point
{
    double lat;                ///< 纬度
    double lon;                ///< 经度
    std::optional<double> alt; ///< 高度
};

struct LIB_API KPoint : public Point
{
};

struct LIB_API KLineString
{
    std::vector<KPoint> points; ///< 点集
};

struct LIB_API KPolygon
{
    KLineString outerLine;                ///< 外环
    std::optional<KLineString> innerLine; ///< 内环
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
    wce::FlyToWaylineMode flyToWaylineMode;                      ///< 飞向首航点模式
    wce::FinishAction finishAction;                              ///< 航线结束动作
    wce::ExitOnRCLost exitOnRCLost;                              ///< 失控是否继续执行航线
    std::optional<wce::ExecuteRCLostAction> executeRCLostAction; ///< 失控动作类型
    double takeOffSecurityHeight;                                ///< 安全起飞高度
    double globalTransitionalSpeed;                              ///< 全局航线过渡速度
    double globalRTHHeight;                                      ///< 全局返航高度
    std::optional<Point> takeOffRefPoint;                        ///< 参考起飞点
    std::optional<double> takeOffRefPointAGLHeight;              ///< 参考起飞点海拔高度
    DroneInfo droneInfo;                                         ///< 飞行器机型信息
    PayloadInfo payloadInfo;                                     ///< 负载机型信息
    AutoRerouteInfo autoRerouteInfo;                             ///< 航线绕行
};

struct LIB_API WaylineCoordinateSysParam
{
    wce::CoordinateMode coordinateMode;                  ///< 经纬度坐标系
    wce::HeightMode heightMode;                          ///< 航点高程参考平面
    std::optional<wce::PositioningType> positioningType; ///< 经纬度与高度数据源
    double globalShootHeight;                            ///< 飞行器离被摄面高度（相对地面高）
    int surfaceFollowModeEnable;                         ///< 是否开启仿地飞行
    double surfaceRelativeHeight;                        ///< 仿地飞行离地高度（相对地面高）
};

struct LIB_API PayloadParam
{
    int payloadPositionIndex;                      ///< 负载挂载位置
    std::optional<wce::FocusMode> focusMode;       ///< 负载对焦模式
    std::optional<wce::MeteringMode> meteringMode; ///< 负载测光模式
    std::optional<int> dewarpingEnable;            ///< 是否开启畸变矫正
    std::optional<wce::ReturnMode> returnMode;     ///< 激光雷达回波模式
    std::optional<int> samplingRate;               ///< 负载采样率
    std::optional<wce::ScanningMode> scanningMode; ///< 负载扫描模式
    std::optional<int> modelColoringEnable;        ///< 真彩上色
    std::vector<wce::ImageFormat> imageFormat;     ///< 图片格式列表
};

struct LIB_API GlobalWaypointHeadingParam
{
    wce::WaypointHeadingMode waypointHeadingMode;         ///< 飞行器偏航角模式
    double waypointHeadingAngle;                          ///< 飞行器偏航角度
    Point waypointPoiPoint;                               ///< 兴趣点
    wce::WaypointHeadingPathMode waypointHeadingPathMode; ///< 飞行器偏航角转动方向
};

struct LIB_API Overlap
{
    std::optional<int> orthoLidarOverlapH;     ///< 激光航向重叠率（正射）
    std::optional<int> orthoLidarOverlapW;     ///< 激光旁向重叠率（正射）
    std::optional<int> orthoCameraOverlapH;    ///< 可见光航向重叠率（正射）
    std::optional<int> orthoCameraOverlapW;    ///< 可见光旁向重叠率（正射）
    std::optional<int> inclinedLidarOverlapH;  ///< 激光航向重叠率（倾斜）
    std::optional<int> inclinedLidarOverlapW;  ///< 激光旁向重叠率（倾斜）
    std::optional<int> inclinedCameraOverlapH; ///< 可见光航向重叠率（倾斜）
    std::optional<int> inclinedCameraOverlapW; ///< 可见光旁向重叠率（倾斜）
};

struct LIB_API MappingHeadingParam
{
    std::optional<wce::MappingHeadingMode> mappingHeadingMode; ///< 飞行器偏航角模式
    std::optional<int> mappingHeadingAngle;                    ///< 飞行器偏航角
};

struct LIB_API WaypointHeadingParam
{
    wce::WaypointHeadingMode waypointHeadingMode;         ///< 飞行器偏航角模式
    std::optional<double> waypointHeadingAngle;           ///< 飞行器偏航角度
    std::optional<Point> waypointPoiPoint;                ///< 兴趣点
    wce::WaypointHeadingPathMode waypointHeadingPathMode; ///< 飞行器偏航角转动方向
};

struct LIB_API WaypointTurnParam
{
    wce::WaypointTurnMode waypointTurnMode;        ///< 航点类型（航点转弯模式）
    std::optional<double> waypointTurnDampingDist; ///< 航点转弯截距
};

struct LIB_API Placemark
{
#if 0
    /* 建图航拍模板元素 */
    std::optional<int> caliFlightEnable;                 ///< 是否开启标定飞行
    int elevationOptimizeEnable;                         ///< 是否开启高程优化
    std::optional<int> smartObliqueEnable;               ///< 是否开启智能摆拍
    std::optional<int> smartObliqueGimbalPitch;          ///< 智能摆拍拍摄俯仰角
    wce::ShootType shootType;                            ///< 拍照模式（定时或定距）
    int direction;                                       ///< 航线方向
    int margin;                                          ///< 测区外扩距离
    Overlap overlap;                                     ///< 重叠率参数
    double ellipsoidHeight;                              ///< 全局航线高度（椭球高）
    double height;                                       ///< 全局航线高度
    std::optional<int> facadeWaylineEnable;              ///< 是否开启斜立面
    KPolygon polygon;                                    ///< 测区多边形
    MappingHeadingParam mappingHeadingParam;             ///< 建图航拍飞行器朝向参数
    std::optional<wce::GimbalPitchMode> gimbalPitchMode; ///< 云台俯仰角模式
    std::optional<int> gimbalPitchAngle;                 ///< 云台俯仰角度
    std::optional<int> quickOrthoMappingEnable;          ///< 正射智能摆拍开关
    std::optional<int> quickOrthoMappingPitch;           ///< 正射智能摆拍角度
#endif

    /* 倾斜摄影模板元素 */
    std::optional<int> caliFlightEnable; ///< 是否开启标定飞行
    int inclinedGimbalPitch;             ///< 云台俯仰角度（倾斜）
    double inclinedFlightSpeed;          ///< 航线飞行速度（倾斜）
    wce::ShootType shootType;            ///< 拍照模式（定时或定距）
    int direction;                       ///< 航线方向
    int margin;                          ///< 测区外扩距离
    Overlap overlap;                     ///< 重叠率参数
    double ellipsoidHeight;              ///< 全局航线高度（椭球高）
    double height;                       ///< 全局航线高度
    KPolygon polygon;                    ///< 测区多边形

#if 0
    /* 航带飞行模板元素 */
    int caliFlightEnable;         ///< 是否开启标定飞行
    wce::ShootType shootType;     ///< 拍照模式（定时或定距）
    int direction;                ///< 航线方向
    int margin;                   ///< 测区外扩距离
    int singleLineEnable;         ///< 是否开启单航线飞行
    double cuttingDistance;       ///< 每个子航带航线长度
    int boundaryOptimEnable;      ///< 是否开启边缘优化
    int leftExtend;               ///< 航带左侧外扩距离
    int rightExtend;              ///< 航带右侧外扩距离
    int includeCenterEnable;      ///< 是否包含中心线
    double ellipsoidHeight;       ///< 全局航线高度（椭球高）
    double height;                ///< 全局航线高度
    int stripUseTemplateAltitude; ///< 是否开启变高航带
    KLineString lineString;       ///< 航点信息
    /* 航点飞行模板元素 */
    std::optional<int> isRisky;                ///< 是否危险点
    KPoint point;                              ///< 航点经纬度
    int index;                                 ///< 航点序号
    int useGlobalHeight;                       ///< 是否使用全局高度
    double ellipsoidHeight;                    ///< 航点高度（椭球高）
    double height;                             ///< 航点高度
    int useGlobalSpeed;                        ///< 是否使用全局飞行速度
    double waypointSpeed;                      ///< 航点飞行速度
    int useGlobalHeadingParam;                 ///< 是否使用全局偏航角模式参数
    WaypointHeadingParam waypointHeadingParam; ///< 偏航角模式参数
    int useGlobalTurnParam;                    ///< 是否使用全局航点类型（全局航点转弯模式）
    WaypointTurnParam waypointTurnParam;       ///< 航点类型（航点转弯模式）
    int useStraightLine;                       ///< 该航段是否贴合直线
    double gimbalPitchAngle;                   ///< 航点云台俯仰角
#endif
};

struct LIB_API Folder
{
    /* 模板共用元素 */
    wce::TemplateType templateType;                      ///< 预定义模板类型
    int templateId;                                      ///< 模板ID
    double autoFlightSpeed;                              ///< 全局航线飞行速度
    WaylineCoordinateSysParam waylineCoordinateSysParam; ///< 坐标系参数
    PayloadParam payloadParam;                           ///< 负载设置

#if 0
    /* 航点飞行模板元素 */
    wce::WaypointTurnMode globalWaypointTurnMode;          ///< 全局航点类型（全局航点转弯模式）
    std::optional<int> globalUseStraightLine;              ///< 全局航段轨迹是否尽量贴合直线
    wce::GimbalPitchMode gimbalPitchMode;                  ///< 云台俯仰角控制模式
    double globalHeight;                                   ///< 全局航线高度（相对起飞点高度）
    GlobalWaypointHeadingParam globalWaypointHeadingParam; ///< 全局偏航角模式参数
#endif

    Placemark placemark; ///< 航点信息
};

struct LIB_API Document
{
    std::optional<std::string> author;   ///< 文件创建作者
    std::optional<long long> createTime; ///< 文件创建时间（Unix Timestamp）
    std::optional<long long> updateTime; ///< 文件更新时间（Unix Timestamp）
    MissionConfig missionConfig;         ///< 任务信息
    Folder folder;                       ///< 模板信息
};
}; // namespace wpml_codec::structs

namespace wcs = wpml_codec::structs;

#endif // WPML_CODEC_STRUCTS_H
