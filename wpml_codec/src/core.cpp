#include <wpml_codec/core.h>
#include <wpml_codec/utils.h>
#include <wpml_codec/macros.h>
#include <tinyxml2.h>

namespace xml = tinyxml2;

namespace wpml_codec::core
{
std::optional<wcs::Document> parseKML(const std::string& kmlPath)
{
    wcs::Document res;
    xml::XMLDocument doc;
    xml::XMLError eResult = doc.LoadFile(kmlPath.c_str());
    if (eResult != xml::XML_SUCCESS)
    {
        std::cerr << "Failed to load kml file: " << kmlPath << std::endl;
        return std::nullopt;
    }
    // Parse KML
    // Step 1: Implement File Creation Information
    xml::XMLElement *pDocument = doc.FirstChildElement("kml")->FirstChildElement("Document");
    if (pDocument == nullptr)
    {
        std::cerr << "No Document element found" << std::endl;
        return std::nullopt;
    }
    SET_OPT_WPML_ARG_S(res, pDocument, author);
    SET_OPT_WPML_ARG_L(res, pDocument, createTime);
    SET_OPT_WPML_ARG_L(res, pDocument, updateTime);
    // Step 2: Setup Mission Configuration
    xml::XMLElement *pMissionConfig = pDocument->FirstChildElement("wpml:missionConfig");
    if (pMissionConfig == nullptr)
    {
        std::cerr << "No wpml:missionConfig element found" << std::endl;
        return std::nullopt;
    }
    SET_OPT_WPML_ARG_E(res.missionConfig, pMissionConfig, flyToWaylineMode, FlyToWaylineMode);
    SET_OPT_WPML_ARG_E(res.missionConfig, pMissionConfig, finishAction, FinishAction);
    SET_OPT_WPML_ARG_E(res.missionConfig, pMissionConfig, exitOnRCLost, ExitOnRCLost);
    SET_OPT_WPML_ARG_E(res.missionConfig, pMissionConfig, executeRCLostAction, ExecuteRCLostAction);
    SET_OPT_WPML_ARG_D(res.missionConfig, pMissionConfig, takeOffSecurityHeight);
    SET_OPT_WPML_ARG_D(res.missionConfig, pMissionConfig, globalTransitionalSpeed);
    SET_OPT_WPML_ARG_D(res.missionConfig, pMissionConfig, globalRTHHeight);
    SET_OPT_WPML_ARG_P(res.missionConfig, pMissionConfig, takeOffRefPoint);
    SET_OPT_WPML_ARG_D(res.missionConfig, pMissionConfig, takeOffRefPointAGLHeight);
    xml::XMLElement *pDroneInfo = pMissionConfig->FirstChildElement("wpml:droneInfo");
    if (pDroneInfo != nullptr)
    {
        SET_OPT_WPML_ARG_I(res.missionConfig.droneInfo, pDroneInfo, droneEnumValue);
        SET_OPT_WPML_ARG_I(res.missionConfig.droneInfo, pDroneInfo, droneSubEnumValue);
    }
    xml::XMLElement *pPayloadInfo = pMissionConfig->FirstChildElement("wpml:payloadInfo");
    if (pPayloadInfo != nullptr)
    {
        SET_OPT_WPML_ARG_I(res.missionConfig.payloadInfo, pPayloadInfo, payloadEnumValue);
        SET_OPT_WPML_ARG_I(res.missionConfig.payloadInfo, pPayloadInfo, payloadPositionIndex);
    }
    xml::XMLElement *pAutoRerouteInfo = pMissionConfig->FirstChildElement("wpml:autoRerouteInfo");
    if (pAutoRerouteInfo != nullptr)
    {
        SET_OPT_WPML_ARG_I(res.missionConfig.autoRerouteInfo, pAutoRerouteInfo, missionAutoRerouteMode);
        SET_OPT_WPML_ARG_I(res.missionConfig.autoRerouteInfo, pAutoRerouteInfo, transitionalAutoRerouteMode);
    }
    // Step 3: Setup A Folder for Waypoint Template
    xml::XMLElement *pFolder = pDocument->FirstChildElement("Folder");
    if (pFolder == nullptr)
    {
        std::cerr << "No Folder element found" << std::endl;
        return std::nullopt;
    }
    SET_OPT_WPML_ARG_E(res.folder, pFolder, templateType, TemplateType);
    SET_OPT_WPML_ARG_I(res.folder, pFolder, templateId);
    SET_OPT_WPML_ARG_D(res.folder, pFolder, autoFlightSpeed);
    xml::XMLElement *pWaylineCoordinateSysParam = pFolder->FirstChildElement("wpml:waylineCoordinateSysParam");
    if (pWaylineCoordinateSysParam != nullptr)
    {
        SET_OPT_WPML_ARG_E(
            res.folder.waylineCoordinateSysParam, pWaylineCoordinateSysParam, coordinateMode, CoordinateMode);
        SET_OPT_WPML_ARG_E(res.folder.waylineCoordinateSysParam, pWaylineCoordinateSysParam, heightMode, HeightMode);
        SET_OPT_WPML_ARG_E(
            res.folder.waylineCoordinateSysParam, pWaylineCoordinateSysParam, positioningType, PositioningType);
        SET_OPT_WPML_ARG_D(res.folder.waylineCoordinateSysParam, pWaylineCoordinateSysParam, globalShootHeight);
        SET_OPT_WPML_ARG_I(res.folder.waylineCoordinateSysParam, pWaylineCoordinateSysParam, surfaceFollowModeEnable);
        SET_OPT_WPML_ARG_D(res.folder.waylineCoordinateSysParam, pWaylineCoordinateSysParam, surfaceRelativeHeight);
    }
    xml::XMLElement *pPayloadParam = pFolder->FirstChildElement("wpml:payloadParam");
    if (pPayloadParam != nullptr)
    {
        SET_OPT_WPML_ARG_I(res.folder.payloadParam, pPayloadParam, payloadPositionIndex);
        SET_OPT_WPML_ARG_E(res.folder.payloadParam, pPayloadParam, focusMode, FocusMode);
        SET_OPT_WPML_ARG_E(res.folder.payloadParam, pPayloadParam, meteringMode, MeteringMode);
        SET_OPT_WPML_ARG_I(res.folder.payloadParam, pPayloadParam, dewarpingEnable);
        SET_OPT_WPML_ARG_E(res.folder.payloadParam, pPayloadParam, returnMode, ReturnMode);
        SET_OPT_WPML_ARG_I(res.folder.payloadParam, pPayloadParam, samplingRate);
        SET_OPT_WPML_ARG_E(res.folder.payloadParam, pPayloadParam, scanningMode, ScanningMode);
        SET_OPT_WPML_ARG_I(res.folder.payloadParam, pPayloadParam, modelColoringEnable);
        SET_OPT_WPML_ARG_ES(res.folder.payloadParam, pPayloadParam, imageFormat, ImageFormat);
    }
    // Step 4: Setup Placemark
    xml::XMLElement *pPlacemark = pFolder->FirstChildElement("Placemark");
    if (pPlacemark == nullptr)
    {
        std::cerr << "No Placemark element found" << std::endl;
        return std::nullopt;
    }
    SET_OPT_WPML_ARG_I(res.folder.placemark, pPlacemark, caliFlightEnable);
    SET_OPT_WPML_ARG_I(res.folder.placemark, pPlacemark, inclinedGimbalPitch);
    SET_OPT_WPML_ARG_D(res.folder.placemark, pPlacemark, inclinedFlightSpeed);
    SET_OPT_WPML_ARG_E(res.folder.placemark, pPlacemark, shootType, ShootType);
    SET_OPT_WPML_ARG_I(res.folder.placemark, pPlacemark, direction);
    SET_OPT_WPML_ARG_I(res.folder.placemark, pPlacemark, margin);
    xml::XMLElement *pOverlap = pPlacemark->FirstChildElement("wpml:overlap");
    if (pOverlap != nullptr)
    {
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, orthoLidarOverlapH);
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, orthoLidarOverlapW);
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, orthoCameraOverlapH);
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, orthoCameraOverlapW);
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, inclinedLidarOverlapH);
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, inclinedLidarOverlapW);
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, inclinedCameraOverlapH);
        SET_OPT_WPML_ARG_I(res.folder.placemark.overlap, pOverlap, inclinedCameraOverlapW);
    }
    SET_OPT_WPML_ARG_D(res.folder.placemark, pPlacemark, ellipsoidHeight);
    SET_OPT_WPML_ARG_D(res.folder.placemark, pPlacemark, height);
    SET_OPT_WPML_ARG_KPN(res.folder.placemark, pPlacemark, polygon);
    // TODO: Implement
    return res;
}

bool creatKML(const wcs::Document& data, const std::string& kmlPath)
{
    try
    {
        xml::XMLDocument doc;
        // 1. Create xml
        xml::XMLDeclaration *decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
        doc.InsertFirstChild(decl);
        // 2. Create kml Element
        xml::XMLElement *kmlElement = doc.NewElement("kml");
        kmlElement->SetAttribute("xmlns", "http://www.opengis.net/kml/2.2");
        kmlElement->SetAttribute("xmlns:wpml", "http://www.dji.com/wpmz/1.0.6");
        doc.InsertEndChild(kmlElement);
        // 3. Create Document Element
        xml::XMLElement *documentElement = doc.NewElement("Document");
        kmlElement->InsertEndChild(documentElement);
        GET_OPT_WPML_ARG_S(doc, documentElement, author);
        GET_OPT_WPML_ARG_N(doc, documentElement, createTime);
        GET_OPT_WPML_ARG_N(doc, documentElement, updateTime);
        // TODO: Implement
        // End
        doc.SaveFile(kmlPath.c_str());
        return true;
    }
    catch (...)
    {
        return false;
    }
}
} // namespace wpml_codec::core
