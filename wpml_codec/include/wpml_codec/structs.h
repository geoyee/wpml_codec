#pragma once

#ifndef WPML_CODEC_STRUCTS_H
#define WPML_CODEC_STRUCTS_H

#include <wpml_codec/common.h>
#include <wpml_codec/enums.h>
#include <optional>
#include <memory>

namespace wpml_codec::structs
{
/**********************************************************************/
/*                              Custom                                */
/**********************************************************************/

struct LIB_API Point
{
    double lat;                ///< 纬度
    double lon;                ///< 经度
    std::optional<double> alt; ///< 高度
};

using KPoint = Point;

struct LIB_API KLineString
{
    std::vector<KPoint> points; ///< 点集
};

struct LIB_API KPolygon
{
    KLineString outerLine;                ///< 外环
    std::optional<KLineString> innerLine; ///< 内环
};

/**********************************************************************/
/*                              Common                                */
/**********************************************************************/

struct LIB_API DroneInfo
{
    int droneEnumValue;                   ///< 飞行器机型主类型
    std::optional<int> droneSubEnumValue; ///< 飞行器机型子类型
};

struct LIB_API PayloadInfo
{
    int payloadEnumValue;     ///< 负载机型主类型
    int payloadPositionIndex; ///< 负载挂载位置
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

struct LIB_API WaypointHeadingParam
{
    wce::WaypointHeadingMode waypointHeadingMode;         ///< 飞行器偏航角模式
    std::optional<double> waypointHeadingAngle;           ///< 飞行器偏航角度
    std::optional<Point> waypointPoiPoint;                ///< 兴趣点
    wce::WaypointHeadingPathMode waypointHeadingPathMode; ///< 飞行器偏航角转动方向
};

struct LIB_API GlobalWaypointHeadingParam : public WaypointHeadingParam
{
};

struct LIB_API WaypointTurnParam
{
    wce::WaypointTurnMode waypointTurnMode;        ///< 航点类型（航点转弯模式）
    std::optional<double> waypointTurnDampingDist; ///< 航点转弯截距
};

struct LIB_API AutoRerouteInfo
{
    int missionAutoRerouteMode;      ///< 任务航线绕行模式
    int transitionalAutoRerouteMode; ///< 过渡航线绕行模式
};

struct LIB_API ActionTrigger
{
    wce::ActionTriggerType actionTriggerType; ///< 动作触发器类型
    std::optional<double> actionTriggerParam; ///< 动作触发器参数
};

struct LIB_API ActionActuatorFuncParam
{
    virtual ~ActionActuatorFuncParam() = default;
};

struct LIB_API TakePhotoParam : public ActionActuatorFuncParam
{
    int payloadPositionIndex;                            ///< 负载挂载位置
    std::string fileSuffix;                              ///< 拍摄照片文件后缀
    std::vector<wce::PayloadLensIndex> payloadLensIndex; ///< 拍摄照片存储类型
    int useGlobalPayloadLensIndex;                       ///< 是否使用全局存储类型
};

using StartRecordParam = TakePhotoParam;

struct LIB_API StopRecordParam : public ActionActuatorFuncParam
{
    int payloadPositionIndex;                            ///< 负载挂载位置
    std::vector<wce::PayloadLensIndex> payloadLensIndex; ///< 拍摄照片存储类型
};

struct LIB_API FocusParam : public ActionActuatorFuncParam
{
    int payloadPositionIndex;                ///< 负载挂载位置
    int isPointFocus;                        ///< 是否点对焦
    double focusX;                           ///< 对焦点位置
    double focusY;                           ///< 对焦点位置
    std::optional<double> focusRegionWidth;  ///< 对焦区域宽度比
    std::optional<double> focusRegionHeight; ///< 对焦区域宽度比
    int isInfiniteFocus;                     ///< 是否无穷远对焦
};

struct LIB_API ZoomParam : public ActionActuatorFuncParam
{
    int payloadPositionIndex; ///< 负载挂载位置
    double focalLength;       ///< 变焦焦距
};

struct LIB_API CustomDirNameParam : public ActionActuatorFuncParam
{
    int payloadPositionIndex;  ///< 负载挂载位置
    std::string directoryName; ///< 新文件夹的名称
};

struct LIB_API GimbalRotateParam : public ActionActuatorFuncParam
{
    int payloadPositionIndex;                       ///< 负载挂载位置
    wce::GimbalHeadingYawBase gimbalHeadingYawBase; ///< 云台偏航角转动坐标系
    wce::GimbalRotateMode gimbalRotateMode;         ///< 云台转动模式
    int gimbalPitchRotateEnable;                    ///< 是否使能云台Pitch转动
    double gimbalPitchRotateAngle;                  ///< 云台Pitch转动角度
    int gimbalRollRotateEnable;                     ///< 是否使能云台Roll转动
    double gimbalRollRotateAngle;                   ///< 云台Roll转动角度
    int gimbalYawRotateEnable;                      ///< 是否使能云台Yaw转动
    double gimbalYawRotateAngle;                    ///< 云台Yaw转动角度
    int gimbalRotateTimeEnable;                     ///< 是否使能云台转动时间
    double gimbalRotateTime;                        ///< 云台完成转动用时
};

struct LIB_API GimbalEvenlyRotateParam : public ActionActuatorFuncParam
{
    int payloadPositionIndex;      ///< 负载挂载位置
    double gimbalPitchRotateAngle; ///< 云台Pitch转动角度
};

struct LIB_API RotateYawParam : public ActionActuatorFuncParam
{
    double aircraftHeading;                 ///< 飞行器目标偏航角
    wce::AircraftPathMode aircraftPathMode; ///< 飞行器偏航角转动模式
};

struct LIB_API HoverParam : public ActionActuatorFuncParam
{
    double hoverTime; ///< 飞行器悬停等待时间
};

struct LIB_API AccurateShootParam : public ActionActuatorFuncParam
{
    double gimbalPitchRotateAngle;                       ///< 云台Pitch转动角度
    double gimbalYawRotateAngle;                         ///< 云台Yaw转动角度
    int focusX;                                          ///< 目标选中框中心水平坐标
    int focusY;                                          ///< 目标选中框中心竖直坐标
    int focusRegionWidth;                                ///< 目标选中框宽
    int focusRegionHeight;                               ///< 目标选中框高
    double focalLength;                                  ///< 变焦焦距
    double aircraftHeading;                              ///< 飞行器目标偏航角
    int accurateFrameValid;                              ///< 是否框选精准复拍目标
    int payloadPositionIndex;                            ///< 负载挂载位置
    std::vector<wce::PayloadLensIndex> payloadLensIndex; ///< 拍摄照片存储类型
    int useGlobalPayloadLensIndex;                       ///< 是否使用全局存储类型
    double targetAngle;                                  ///< 目标框角度
    int imageWidth;                                      ///< 照片宽度
    int imageHeight;                                     ///< 照片高度
    int AFPos;                                           ///< AF电机位置
    int gimbalPort;                                      ///< 云台端口号
    int accurateCameraType;                              ///< 相机类型
    std::string accurateFilePath;                        ///< 照片文件路径
    std::string accurateFileMD5;                         ///< 照片文件MD5
    int accurateFileSize;                                ///< 照片文件大小
    std::string accurateFileSuffix;                      ///< 照片文件后缀
    int accurateCameraApertue;                           ///< 光圈大小
    int accurateCameraLuminance;                         ///< 环境亮度
    double accurateCameraShutterTime;                    ///< 快门时间
    int accurateCameraISO;                               ///< ISO
};

struct LIB_API OrientedShootParams : public ActionActuatorFuncParam
{
    double gimbalPitchRotateAngle;                       ///< 云台Pitch转动角度
    double gimbalYawRotateAngle;                         ///< 云台Yaw转动角度
    int focusX;                                          ///< 目标选中框中心水平坐标
    int focusY;                                          ///< 目标选中框中心竖直坐标
    int focusRegionWidth;                                ///< 目标选中框宽
    int focusRegionHeight;                               ///< 目标选中框高
    double focalLength;                                  ///< 变焦焦距
    double aircraftHeading;                              ///< 飞行器目标偏航角
    int accurateFrameValid;                              ///< 是否框选精准复拍目标
    int payloadPositionIndex;                            ///< 负载挂载位置
    std::vector<wce::PayloadLensIndex> payloadLensIndex; ///< 拍摄照片存储类型
    int useGlobalPayloadLensIndex;                       ///< 是否使用全局存储类型
    double targetAngle;                                  ///< 目标框角度
    std::string actionUUID;                              ///< 动作唯一标识
    int imageWidth;                                      ///< 照片宽度
    int imageHeight;                                     ///< 照片高度
    int AFPos;                                           ///< AF电机位置
    int gimbalPort;                                      ///< 云台端口号
    int orientedCameraType;                              ///< 相机类型
    std::string orientedFilePath;                        ///< 照片文件路径
    std::string orientedFileMD5;                         ///< 照片文件MD5
    int orientedFileSize;                                ///< 照片文件大小
    std::string orientedFileSuffix;                      ///< 照片文件后缀
    int orientedCameraApertue;                           ///< 光圈大小
    int orientedCameraLuminance;                         ///< 环境亮度
    double orientedCameraShutterTime;                    ///< 快门时间
    int orientedCameraISO;                               ///< ISO
    wce::OrientedPhotoMode orientedPhotoMode;            ///< 拍照模式
};

struct LIB_API PanoShotParams : public ActionActuatorFuncParam
{
    int payloadPositionIndex;                            ///< 负载挂载位置
    std::vector<wce::PayloadLensIndex> payloadLensIndex; ///< 拍摄照片存储类型
    int useGlobalPayloadLensIndex;                       ///< 是否使用全局存储类型
    wce::PanoShotSubMode panoShotSubMode;                ///< 全景拍照模式
};

struct LIB_API RecordPointCloudParams : public ActionActuatorFuncParam
{
    int payloadPositionIndex;                             ///< 负载挂载位置
    wce::RecordPointCloudOperate recordPointCloudOperate; ///< 点云操作
};

struct LIB_API MegaphoneParams : public ActionActuatorFuncParam
{
    int payloadPositionIndex;              ///< 负载挂载位置
    std::string actionUUID;                ///< 动作唯一标识
    int megaphoneOperateType;              ///< 喊话动作开关
    int megaphoneOperateVolume;            ///< 喊话动作音量
    int megaphoneOperateLoop;              ///< 是否单曲循环播放
    std::string megaphoneOperateFilePath;  ///< 喊话音频文件在kmz中对应的路径
    std::string megaphoneFileName;         ///< 喊话音频文件对应的名称
    std::string megaphoneFileOriginalName; ///< 喊话音频文件播放时显示的名字
    std::string megaphoneFileMd5;          ///< 喊话音频文件的md5值
    int megaphoneFileBitrate;              ///< 喊话音频文件的压缩比特率（1-6，目前仅支持4）
};

struct LIB_API SearchlightParams : public ActionActuatorFuncParam
{
    int payloadPositionIndex;              ///< 负载挂载位置
    std::optional<std::string> actionUUID; ///< 动作唯一标识
    int searchlightOperateType;            ///< 探照灯操作类型（0-2）
    int searchlightBrightness;             ///< 亮度
};

struct LIB_API Action
{
    int actionId;                                                     ///< 动作id
    wce::ActionActuatorFunc actionActuatorFunc;                       ///< 动作类型
    std::shared_ptr<ActionActuatorFuncParam> actionActuatorFuncParam; ///< 动作参数
};

struct LIB_API ActionGroup
{
    int actionGroupId;                    ///< 动作组id
    int actionGroupStartIndex;            ///< 动作组开始生效的航点
    int actionGroupEndIndex;              ///< 动作组结束生效的航点
    wce::ActionGroupMode actionGroupMode; ///< 动作执行模式
    ActionTrigger actionTrigger;          ///< 动作组触发器
    Action action;                        ///< 动作列表
};

/**********************************************************************/
/*                                KML                                 */
/**********************************************************************/

struct LIB_API KMLMissionConfig
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
    std::optional<double> surfaceRelativeHeight;         ///< 仿地飞行离地高度（相对地面高）
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

struct LIB_API KMLPlacemark
{
    virtual ~KMLPlacemark() = default;
};

struct LIB_API MappingAerialKMLPlacemark : public KMLPlacemark
{
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
};

struct LIB_API ObliquePhotographyKMLPlacemark : public KMLPlacemark
{
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
};

struct LIB_API WaypointSegmentFlightKMLPlacemark : public KMLPlacemark
{
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
};

struct LIB_API WaypointInfoKMLPlacemark : public KMLPlacemark
{
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
};

struct LIB_API KMLFolder
{
    wce::TemplateType templateType;                      ///< 预定义模板类型
    int templateId;                                      ///< 模板ID
    double autoFlightSpeed;                              ///< 全局航线飞行速度
    WaylineCoordinateSysParam waylineCoordinateSysParam; ///< 坐标系参数
    PayloadParam payloadParam;                           ///< 负载设置
    std::shared_ptr<KMLPlacemark> placemark;             ///< 除航点飞行外的其他任务模板

    virtual ~KMLFolder() = default;
};

struct LIB_API WaypointFlightKMLFolder : public KMLFolder
{
    wce::GlobalWaypointTurnMode globalWaypointTurnMode;    ///< 全局航点类型（全局航点转弯模式）
    std::optional<int> globalUseStraightLine;              ///< 全局航段轨迹是否尽量贴合直线
    wce::GimbalPitchMode gimbalPitchMode;                  ///< 云台俯仰角控制模式
    double globalHeight;                                   ///< 全局航线高度（相对起飞点高度）
    GlobalWaypointHeadingParam globalWaypointHeadingParam; ///< 全局偏航角模式参数
    std::vector<WaypointInfoKMLPlacemark> placemark;       ///< 航点信息
};

struct LIB_API KMLDocument
{
    std::optional<std::string> author;   ///< 文件创建作者
    std::optional<long long> createTime; ///< 文件创建时间（Unix Timestamp）
    std::optional<long long> updateTime; ///< 文件更新时间（Unix Timestamp）
    KMLMissionConfig missionConfig;      ///< 任务信息
    std::shared_ptr<KMLFolder> folder;   ///< 模板信息
};

/**********************************************************************/
/*                               WPML                                 */
/**********************************************************************/

struct LIB_API WPMLMissionConfig
{
    wce::FlyToWaylineMode flyToWaylineMode;                      ///< 飞向首航点模式
    wce::FinishAction finishAction;                              ///< 航线结束动作
    wce::ExitOnRCLost exitOnRCLost;                              ///< 失控是否继续执行航线
    std::optional<wce::ExecuteRCLostAction> executeRCLostAction; ///< 失控动作类型
    double takeOffSecurityHeight;                                ///< 安全起飞高度
    double globalTransitionalSpeed;                              ///< 全局航线过渡速度
    double globalRTHHeight;                                      ///< 全局返航高度
    DroneInfo droneInfo;                                         ///< 飞行器机型信息
    PayloadInfo payloadInfo;                                     ///< 负载机型信息
    AutoRerouteInfo autoRerouteInfo;                             ///< 航线绕行
};

struct LIB_API WPMLPlacemark
{
    std::optional<int> isRisky;                ///< 是否危险点
    KPoint point;                              ///< 航点经纬度
    int index;                                 ///< 航点序号
    double executeHeight;                      ///< 航点执行高度
    double waypointSpeed;                      ///< 航点飞行速度
    WaypointHeadingParam waypointHeadingParam; ///< 偏航角模式参数
    WaypointTurnParam waypointTurnParam;       ///< 航点类型（航点转弯模式）
    int useStraightLine;                       ///< 该航段是否贴合直线
};

struct LIB_API WPMLFolder
{
    int templateId;                           ///< 模板ID
    int waylineId;                            ///< 航线ID
    double autoFlightSpeed;                   ///< 全局航线飞行速度
    wce::ExecuteHeightMode executeHeightMode; ///< 执行高度模式
    std::vector<WPMLPlacemark> placemark;     ///< 航点信息
    ActionGroup startActionGroup;             ///< 航线初始动作
};

struct LIB_API WPMLDocument
{
    WPMLMissionConfig missionConfig; ///< 任务信息
    WPMLFolder folder;               ///< 航线信息
};
}; // namespace wpml_codec::structs

namespace wcs = wpml_codec::structs;

#endif // WPML_CODEC_STRUCTS_H
