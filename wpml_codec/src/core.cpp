#include <wpml_codec/core.h>
#include <wpml_codec/utils.h>
#include <wpml_codec/macros.h>
#include <tinyxml2.h>
#include <iostream>

namespace xml = tinyxml2;

namespace wpml_codec::core
{
std::optional<wcs::KMLDocument> parseKML(const std::string& kmlPath)
{
    wcs::KMLDocument res;
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
    std::string templateType = pFolder->FirstChildElement("wpml:templateType")->GetText();
    if (wcu::equal(templateType, "waypoint"))
    {
        res.folder = std::make_shared<wcs::WaypointFlightKMLFolder>();
        auto curFolder = std::dynamic_pointer_cast<wcs::WaypointFlightKMLFolder>(res.folder);
        SET_OPT_WPML_ARG_E(*curFolder, pFolder, templateType, TemplateType);
        SET_OPT_WPML_ARG_I(*curFolder, pFolder, templateId);
        SET_OPT_WPML_ARG_D(*curFolder, pFolder, autoFlightSpeed);
        SET_OPT_WPML_ARG_E(*curFolder, pFolder, globalWaypointTurnMode, WaypointTurnMode);
        SET_OPT_WPML_ARG_I(*curFolder, pFolder, globalUseStraightLine);
        SET_OPT_WPML_ARG_E(*curFolder, pFolder, gimbalPitchMode, GimbalPitchMode);
        SET_OPT_WPML_ARG_D(*curFolder, pFolder, globalHeight);
        xml::XMLElement *pWaylineCoordinateSysParam = pFolder->FirstChildElement("wpml:waylineCoordinateSysParam");
        if (pWaylineCoordinateSysParam != nullptr)
        {
            SET_OPT_WPML_ARG_E(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, coordinateMode, CoordinateMode);
            SET_OPT_WPML_ARG_E(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, heightMode, HeightMode);
            SET_OPT_WPML_ARG_E(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, positioningType, PositioningType);
            SET_OPT_WPML_ARG_D(curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, globalShootHeight);
            SET_OPT_WPML_ARG_I(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, surfaceFollowModeEnable);
            SET_OPT_WPML_ARG_D(curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, surfaceRelativeHeight);
        }
        xml::XMLElement *pPayloadParam = pFolder->FirstChildElement("wpml:payloadParam");
        if (pPayloadParam != nullptr)
        {
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, payloadPositionIndex);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, focusMode, FocusMode);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, meteringMode, MeteringMode);
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, dewarpingEnable);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, returnMode, ReturnMode);
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, samplingRate);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, scanningMode, ScanningMode);
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, modelColoringEnable);
            SET_OPT_WPML_ARG_ES(curFolder->payloadParam, pPayloadParam, imageFormat, ImageFormat);
        }
        xml::XMLElement *pGlobalWaypointHeadingParam = pFolder->FirstChildElement("wpml:globalWaypointHeadingParam");
        if (pGlobalWaypointHeadingParam != nullptr)
        {
            SET_OPT_WPML_ARG_E(curFolder->globalWaypointHeadingParam,
                               pGlobalWaypointHeadingParam,
                               waypointHeadingMode,
                               WaypointHeadingMode);
            SET_OPT_WPML_ARG_D(
                curFolder->globalWaypointHeadingParam, pGlobalWaypointHeadingParam, waypointHeadingAngle);
            SET_OPT_WPML_ARG_P(curFolder->globalWaypointHeadingParam, pGlobalWaypointHeadingParam, waypointPoiPoint);
            SET_OPT_WPML_ARG_E(curFolder->globalWaypointHeadingParam,
                               pGlobalWaypointHeadingParam,
                               waypointHeadingPathMode,
                               WaypointHeadingPathMode);
        }
        // Step 4: Setup Waypoint Placemark
        std::vector<wcs::WaypointInfoKMLPlacemark> waypointPlacemarks{};
        xml::XMLElement *pPlacemark = pFolder->FirstChildElement("Placemark");
        while (pPlacemark)
        {
            wcs::WaypointInfoKMLPlacemark tmpPlacemark;
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, isRisky);
            SET_OPT_WPML_ARG_KPT(tmpPlacemark, pPlacemark, point);
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, index);
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, useGlobalHeight);
            SET_OPT_WPML_ARG_D(tmpPlacemark, pPlacemark, ellipsoidHeight);
            SET_OPT_WPML_ARG_D(tmpPlacemark, pPlacemark, height);
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, useGlobalSpeed);
            SET_OPT_WPML_ARG_D(tmpPlacemark, pPlacemark, waypointSpeed);
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, useGlobalTurnParam);
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, useStraightLine);
            SET_OPT_WPML_ARG_D(tmpPlacemark, pPlacemark, gimbalPitchAngle);
            xml::XMLElement *pWaypointHeadingParam = pPlacemark->FirstChildElement("wpml:waypointHeadingParam");
            if (pWaypointHeadingParam != nullptr)
            {
                SET_OPT_WPML_ARG_E(
                    tmpPlacemark.waypointHeadingParam, pWaypointHeadingParam, waypointHeadingMode, WaypointHeadingMode);
                SET_OPT_WPML_ARG_D(tmpPlacemark.waypointHeadingParam, pWaypointHeadingParam, waypointHeadingAngle);
                SET_OPT_WPML_ARG_P(tmpPlacemark.waypointHeadingParam, pWaypointHeadingParam, waypointPoiPoint);
                SET_OPT_WPML_ARG_E(tmpPlacemark.waypointHeadingParam,
                                   pWaypointHeadingParam,
                                   waypointHeadingPathMode,
                                   WaypointHeadingPathMode);
            }
            xml::XMLElement *pWaypointTurnParam = pPlacemark->FirstChildElement("wpml:waypointTurnParam");
            if (pWaypointTurnParam != nullptr)
            {
                SET_OPT_WPML_ARG_E(
                    tmpPlacemark.waypointTurnParam, pWaypointTurnParam, waypointTurnMode, WaypointTurnMode);
                SET_OPT_WPML_ARG_D(tmpPlacemark.waypointTurnParam, pWaypointTurnParam, waypointTurnDampingDist);
            }
            waypointPlacemarks.emplace_back(tmpPlacemark);
            pPlacemark = pPlacemark->NextSiblingElement("Placemark");
        }
        curFolder->placemark = waypointPlacemarks;
    }
    else
    {
        res.folder = std::make_shared<wcs::KMLFolder>();
        auto curFolder = std::dynamic_pointer_cast<wcs::KMLFolder>(res.folder);
        SET_OPT_WPML_ARG_E(*curFolder, pFolder, templateType, TemplateType);
        SET_OPT_WPML_ARG_I(*curFolder, pFolder, templateId);
        SET_OPT_WPML_ARG_D(*curFolder, pFolder, autoFlightSpeed);
        xml::XMLElement *pWaylineCoordinateSysParam = pFolder->FirstChildElement("wpml:waylineCoordinateSysParam");
        if (pWaylineCoordinateSysParam != nullptr)
        {
            SET_OPT_WPML_ARG_E(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, coordinateMode, CoordinateMode);
            SET_OPT_WPML_ARG_E(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, heightMode, HeightMode);
            SET_OPT_WPML_ARG_E(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, positioningType, PositioningType);
            SET_OPT_WPML_ARG_D(curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, globalShootHeight);
            SET_OPT_WPML_ARG_I(
                curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, surfaceFollowModeEnable);
            SET_OPT_WPML_ARG_D(curFolder->waylineCoordinateSysParam, pWaylineCoordinateSysParam, surfaceRelativeHeight);
        }
        xml::XMLElement *pPayloadParam = pFolder->FirstChildElement("wpml:payloadParam");
        if (pPayloadParam != nullptr)
        {
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, payloadPositionIndex);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, focusMode, FocusMode);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, meteringMode, MeteringMode);
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, dewarpingEnable);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, returnMode, ReturnMode);
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, samplingRate);
            SET_OPT_WPML_ARG_E(curFolder->payloadParam, pPayloadParam, scanningMode, ScanningMode);
            SET_OPT_WPML_ARG_I(curFolder->payloadParam, pPayloadParam, modelColoringEnable);
            SET_OPT_WPML_ARG_ES(curFolder->payloadParam, pPayloadParam, imageFormat, ImageFormat);
        }
        // Step 4: Setup Placemark
        xml::XMLElement *pPlacemark = pFolder->FirstChildElement("Placemark");
        if (pPlacemark == nullptr)
        {
            std::cerr << "No Placemark element found" << std::endl;
            return std::nullopt;
        }
        switch (curFolder->templateType)
        {
            case wce::TemplateType::mapping2d :
            {
                curFolder->placemark = std::make_shared<wcs::MappingAerialKMLPlacemark>();
                auto curPlacemark = std::dynamic_pointer_cast<wcs::MappingAerialKMLPlacemark>(curFolder->placemark);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, caliFlightEnable);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, elevationOptimizeEnable);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, smartObliqueEnable);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, smartObliqueGimbalPitch);
                SET_OPT_WPML_ARG_E(*curPlacemark, pPlacemark, shootType, ShootType);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, direction);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, margin);
                xml::XMLElement *pOverlap = pPlacemark->FirstChildElement("wpml:overlap");
                if (pOverlap != nullptr)
                {
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoLidarOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoLidarOverlapW);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoCameraOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoCameraOverlapW);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedLidarOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedLidarOverlapW);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedCameraOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedCameraOverlapW);
                }
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, ellipsoidHeight);
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, height);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, facadeWaylineEnable);
                SET_OPT_WPML_ARG_KPN(*curPlacemark, pPlacemark, polygon);
                xml::XMLElement *pMappingHeadingParam = pPlacemark->FirstChildElement("wpml:mappingHeadingParam");
                if (pMappingHeadingParam != nullptr)
                {
                    SET_OPT_WPML_ARG_E(curPlacemark->mappingHeadingParam,
                                       pMappingHeadingParam,
                                       mappingHeadingMode,
                                       MappingHeadingMode);
                    SET_OPT_WPML_ARG_I(curPlacemark->mappingHeadingParam, pMappingHeadingParam, mappingHeadingAngle);
                }
                SET_OPT_WPML_ARG_E(*curPlacemark, pPlacemark, gimbalPitchMode, GimbalPitchMode);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, gimbalPitchAngle);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, quickOrthoMappingEnable);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, quickOrthoMappingPitch);
                break;
            }
            case wce::TemplateType::mapping3d :
            {
                curFolder->placemark = std::make_shared<wcs::ObliquePhotographyKMLPlacemark>();
                auto curPlacemark =
                    std::dynamic_pointer_cast<wcs::ObliquePhotographyKMLPlacemark>(curFolder->placemark);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, caliFlightEnable);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, inclinedGimbalPitch);
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, inclinedFlightSpeed);
                SET_OPT_WPML_ARG_E(*curPlacemark, pPlacemark, shootType, ShootType);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, direction);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, margin);
                xml::XMLElement *pOverlap = pPlacemark->FirstChildElement("wpml:overlap");
                if (pOverlap != nullptr)
                {
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoLidarOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoLidarOverlapW);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoCameraOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, orthoCameraOverlapW);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedLidarOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedLidarOverlapW);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedCameraOverlapH);
                    SET_OPT_WPML_ARG_I(curPlacemark->overlap, pOverlap, inclinedCameraOverlapW);
                }
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, ellipsoidHeight);
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, height);
                SET_OPT_WPML_ARG_KPN(*curPlacemark, pPlacemark, polygon);
                break;
            }
            case wce::TemplateType::mappingStrip :
            {
                curFolder->placemark = std::make_shared<wcs::WaypointSegmentFlightKMLPlacemark>();
                auto curPlacemark =
                    std::dynamic_pointer_cast<wcs::WaypointSegmentFlightKMLPlacemark>(curFolder->placemark);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, caliFlightEnable);
                SET_OPT_WPML_ARG_E(*curPlacemark, pPlacemark, shootType, ShootType);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, direction);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, margin);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, singleLineEnable);
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, cuttingDistance);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, boundaryOptimEnable);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, leftExtend);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, rightExtend);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, includeCenterEnable);
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, ellipsoidHeight);
                SET_OPT_WPML_ARG_D(*curPlacemark, pPlacemark, height);
                SET_OPT_WPML_ARG_I(*curPlacemark, pPlacemark, stripUseTemplateAltitude);
                SET_OPT_WPML_ARG_KLS(*curPlacemark, pPlacemark, lineString);
                break;
            }
            default :
                curFolder->placemark = nullptr;
                break;
        }
    }
    return res;
}

bool createKML(const wcs::KMLDocument& data, const std::string& kmlPath)
{
    try
    {
        xml::XMLDocument doc;
        // Step 1: Create xml
        xml::XMLDeclaration *decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
        doc.InsertFirstChild(decl);
        // Step 2: Create kml Element
        xml::XMLElement *kmlElement = doc.NewElement("kml");
        kmlElement->SetAttribute("xmlns", "http://www.opengis.net/kml/2.2");
        kmlElement->SetAttribute("xmlns:wpml", "http://www.dji.com/wpmz/1.0.6");
        doc.InsertEndChild(kmlElement);
        // Step 3: Create Document Element
        xml::XMLElement *documentElement = doc.NewElement("Document");
        kmlElement->InsertEndChild(documentElement);
        GET_OPT_WPML_ARG_S(doc, documentElement, data, author);
        GET_OPT_WPML_ARG_N(doc, documentElement, data, createTime);
        GET_OPT_WPML_ARG_N(doc, documentElement, data, updateTime);
        // Step 4: Create missionConfig Element
        xml::XMLElement *missionElement = doc.NewElement("wpml:missionConfig");
        documentElement->InsertEndChild(missionElement);
        GET_NEC_WPML_ARG_E(doc, missionElement, data, missionConfig.flyToWaylineMode);
        GET_NEC_WPML_ARG_E(doc, missionElement, data, missionConfig.finishAction);
        GET_NEC_WPML_ARG_E(doc, missionElement, data, missionConfig.exitOnRCLost);
        GET_OPT_WPML_ARG_E(doc, missionElement, data, missionConfig.executeRCLostAction);
        GET_NEC_WPML_ARG_N(doc, missionElement, data, missionConfig.takeOffSecurityHeight);
        GET_NEC_WPML_ARG_N(doc, missionElement, data, missionConfig.globalTransitionalSpeed);
        GET_NEC_WPML_ARG_N(doc, missionElement, data, missionConfig.globalRTHHeight);
        GET_OPT_WPML_ARG_P(doc, missionElement, data, missionConfig.takeOffRefPoint);
        GET_OPT_WPML_ARG_N(doc, missionElement, data, missionConfig.takeOffRefPointAGLHeight);
        xml::XMLElement *droneInfoElement = doc.NewElement("wpml:droneInfo");
        missionElement->InsertEndChild(droneInfoElement);
        GET_NEC_WPML_ARG_N(doc, droneInfoElement, data, missionConfig.droneInfo.droneEnumValue);
        GET_OPT_WPML_ARG_N(doc, droneInfoElement, data, missionConfig.droneInfo.droneSubEnumValue);
        xml::XMLElement *payloadInfoElement = doc.NewElement("wpml:payloadInfo");
        missionElement->InsertEndChild(payloadInfoElement);
        GET_NEC_WPML_ARG_N(doc, payloadInfoElement, data, missionConfig.payloadInfo.payloadEnumValue);
        GET_NEC_WPML_ARG_N(doc, payloadInfoElement, data, missionConfig.payloadInfo.payloadPositionIndex);
        xml::XMLElement *autoRerouteInfoElement = doc.NewElement("wpml:autoRerouteInfo");
        missionElement->InsertEndChild(autoRerouteInfoElement);
        GET_NEC_WPML_ARG_N(doc, autoRerouteInfoElement, data, missionConfig.autoRerouteInfo.missionAutoRerouteMode);
        GET_NEC_WPML_ARG_N(
            doc, autoRerouteInfoElement, data, missionConfig.autoRerouteInfo.transitionalAutoRerouteMode);
        // Step 5: Create Folder Element
        xml::XMLElement *folderElement = doc.NewElement("Folder");
        documentElement->InsertEndChild(folderElement);
        if (data.folder->templateType == wce::TemplateType::waypoint)
        {
            auto curFolder = std::dynamic_pointer_cast<wcs::WaypointFlightKMLFolder>(data.folder);
            GET_NEC_WPML_ARG_E(doc, folderElement, *curFolder, templateType);
            GET_NEC_WPML_ARG_N(doc, folderElement, *curFolder, templateId);
            GET_NEC_WPML_ARG_N(doc, folderElement, *curFolder, autoFlightSpeed);
            GET_NEC_WPML_ARG_E(doc, folderElement, *curFolder, globalWaypointTurnMode);
            GET_OPT_WPML_ARG_N(doc, folderElement, *curFolder, globalUseStraightLine);
            GET_NEC_WPML_ARG_E(doc, folderElement, *curFolder, gimbalPitchMode);
            GET_NEC_WPML_ARG_N(doc, folderElement, *curFolder, globalHeight);
            xml::XMLElement *waylineCoordinateSysParamElement = doc.NewElement("wpml:waylineCoordinateSysParam");
            folderElement->InsertEndChild(waylineCoordinateSysParamElement);
            GET_NEC_WPML_ARG_E(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, coordinateMode);
            GET_NEC_WPML_ARG_E(doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, heightMode);
            GET_OPT_WPML_ARG_E(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, positioningType);
            GET_NEC_WPML_ARG_N(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, globalShootHeight);
            GET_NEC_WPML_ARG_N(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, surfaceFollowModeEnable);
            GET_OPT_WPML_ARG_N(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, surfaceRelativeHeight);
            xml::XMLElement *payloadParamElement = doc.NewElement("wpml:payloadParam");
            folderElement->InsertEndChild(payloadParamElement);
            GET_NEC_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, payloadPositionIndex);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, focusMode);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, meteringMode);
            GET_OPT_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, dewarpingEnable);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, returnMode);
            GET_OPT_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, samplingRate);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, scanningMode);
            GET_OPT_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, modelColoringEnable);
            GET_NEC_WPML_ARG_ES(doc, payloadParamElement, curFolder->payloadParam, imageFormat);
            xml::XMLElement *globalWaypointHeadingParamElement = doc.NewElement("wpml:globalWaypointHeadingParam");
            folderElement->InsertEndChild(globalWaypointHeadingParamElement);
            GET_NEC_WPML_ARG_E(
                doc, globalWaypointHeadingParamElement, curFolder->globalWaypointHeadingParam, waypointHeadingMode);
            GET_OPT_WPML_ARG_N(
                doc, globalWaypointHeadingParamElement, curFolder->globalWaypointHeadingParam, waypointHeadingAngle);
            GET_OPT_WPML_ARG_P(
                doc, globalWaypointHeadingParamElement, curFolder->globalWaypointHeadingParam, waypointPoiPoint);
            GET_NEC_WPML_ARG_E(
                doc, globalWaypointHeadingParamElement, curFolder->globalWaypointHeadingParam, waypointHeadingPathMode);
            // Step 6: Create Waypoint Placemark Element
            for (const auto& pointPlace : curFolder->placemark)
            {
                xml::XMLElement *placemarkElement = doc.NewElement("Placemark");
                folderElement->InsertEndChild(placemarkElement);
                GET_OPT_WPML_ARG_N(doc, placemarkElement, pointPlace, isRisky);
                GET_NEC_WPML_ARG_KPT(doc, placemarkElement, pointPlace, point);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, index);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, useGlobalHeight);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, ellipsoidHeight);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, height);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, useGlobalSpeed);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, waypointSpeed);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, useGlobalHeadingParam);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, useGlobalTurnParam);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, useStraightLine);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, pointPlace, gimbalPitchAngle);
                xml::XMLElement *waypointHeadingParamElement = doc.NewElement("wpml:waypointHeadingParam");
                placemarkElement->InsertEndChild(waypointHeadingParamElement);
                GET_NEC_WPML_ARG_E(
                    doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointHeadingMode);
                GET_OPT_WPML_ARG_N(
                    doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointHeadingAngle);
                GET_OPT_WPML_ARG_P(doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointPoiPoint);
                GET_NEC_WPML_ARG_E(
                    doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointHeadingPathMode);
                xml::XMLElement *waypointTurnParamElement = doc.NewElement("wpml:waypointTurnParam");
                placemarkElement->InsertEndChild(waypointTurnParamElement);
                GET_NEC_WPML_ARG_E(doc, waypointTurnParamElement, pointPlace.waypointTurnParam, waypointTurnMode);
                GET_OPT_WPML_ARG_N(
                    doc, waypointTurnParamElement, pointPlace.waypointTurnParam, waypointTurnDampingDist);
            }
        }
        else
        {
            auto curFolder = std::dynamic_pointer_cast<wcs::KMLFolder>(data.folder);
            GET_NEC_WPML_ARG_E(doc, folderElement, *curFolder, templateType);
            GET_NEC_WPML_ARG_N(doc, folderElement, *curFolder, templateId);
            GET_NEC_WPML_ARG_N(doc, folderElement, *curFolder, autoFlightSpeed);
            xml::XMLElement *waylineCoordinateSysParamElement = doc.NewElement("wpml:waylineCoordinateSysParam");
            folderElement->InsertEndChild(waylineCoordinateSysParamElement);
            GET_NEC_WPML_ARG_E(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, coordinateMode);
            GET_NEC_WPML_ARG_E(doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, heightMode);
            GET_OPT_WPML_ARG_E(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, positioningType);
            GET_NEC_WPML_ARG_N(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, globalShootHeight);
            GET_NEC_WPML_ARG_N(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, surfaceFollowModeEnable);
            GET_OPT_WPML_ARG_N(
                doc, waylineCoordinateSysParamElement, curFolder->waylineCoordinateSysParam, surfaceRelativeHeight);
            xml::XMLElement *payloadParamElement = doc.NewElement("wpml:payloadParam");
            folderElement->InsertEndChild(payloadParamElement);
            GET_NEC_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, payloadPositionIndex);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, focusMode);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, meteringMode);
            GET_OPT_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, dewarpingEnable);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, returnMode);
            GET_OPT_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, samplingRate);
            GET_OPT_WPML_ARG_E(doc, payloadParamElement, curFolder->payloadParam, scanningMode);
            GET_OPT_WPML_ARG_N(doc, payloadParamElement, curFolder->payloadParam, modelColoringEnable);
            GET_NEC_WPML_ARG_ES(doc, payloadParamElement, curFolder->payloadParam, imageFormat);
            // Step 6: Create Placemark Element
            xml::XMLElement *placemarkElement = doc.NewElement("Placemark");
            folderElement->InsertEndChild(placemarkElement);
            switch (data.folder->templateType)
            {
                case wce::TemplateType::mapping2d :
                {
                    auto curPlacemark = std::dynamic_pointer_cast<wcs::MappingAerialKMLPlacemark>(curFolder->placemark);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, caliFlightEnable);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, elevationOptimizeEnable);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, smartObliqueEnable);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, smartObliqueGimbalPitch);
                    GET_NEC_WPML_ARG_E(doc, placemarkElement, *curPlacemark, shootType);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, direction);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, margin);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, ellipsoidHeight);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, height);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, facadeWaylineEnable);
                    GET_NEC_WPML_ARG_KPN(doc, placemarkElement, *curPlacemark, polygon);
                    GET_OPT_WPML_ARG_E(doc, placemarkElement, *curPlacemark, gimbalPitchMode);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, gimbalPitchAngle);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, quickOrthoMappingEnable);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, quickOrthoMappingPitch);
                    xml::XMLElement *overlapElement = doc.NewElement("wpml:overlap");
                    placemarkElement->InsertEndChild(overlapElement);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoLidarOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoLidarOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoCameraOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoCameraOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedLidarOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedLidarOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedCameraOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedCameraOverlapW);
                    xml::XMLElement *mappingHeadingParamElement = doc.NewElement("wpml:mappingHeadingParam");
                    placemarkElement->InsertEndChild(mappingHeadingParamElement);
                    GET_OPT_WPML_ARG_E(
                        doc, mappingHeadingParamElement, curPlacemark->mappingHeadingParam, mappingHeadingMode);
                    GET_OPT_WPML_ARG_N(
                        doc, mappingHeadingParamElement, curPlacemark->mappingHeadingParam, mappingHeadingAngle);
                    break;
                }
                case wce::TemplateType::mapping3d :
                {
                    auto curPlacemark =
                        std::dynamic_pointer_cast<wcs::ObliquePhotographyKMLPlacemark>(curFolder->placemark);
                    GET_OPT_WPML_ARG_N(doc, placemarkElement, *curPlacemark, caliFlightEnable);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, inclinedGimbalPitch);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, inclinedFlightSpeed);
                    GET_NEC_WPML_ARG_E(doc, placemarkElement, *curPlacemark, shootType);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, direction);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, margin);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, ellipsoidHeight);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, height);
                    GET_NEC_WPML_ARG_KPN(doc, placemarkElement, *curPlacemark, polygon);
                    xml::XMLElement *overlapElement = doc.NewElement("wpml:overlap");
                    placemarkElement->InsertEndChild(overlapElement);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoLidarOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoLidarOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoCameraOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoCameraOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedLidarOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedLidarOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedCameraOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedCameraOverlapW);
                    break;
                }
                case wce::TemplateType::mappingStrip :
                {
                    auto curPlacemark =
                        std::dynamic_pointer_cast<wcs::WaypointSegmentFlightKMLPlacemark>(curFolder->placemark);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, caliFlightEnable);
                    GET_NEC_WPML_ARG_E(doc, placemarkElement, *curPlacemark, shootType);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, direction);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, margin);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, singleLineEnable);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, cuttingDistance);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, boundaryOptimEnable);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, leftExtend);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, rightExtend);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, includeCenterEnable);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, ellipsoidHeight);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, height);
                    GET_NEC_WPML_ARG_N(doc, placemarkElement, *curPlacemark, stripUseTemplateAltitude);
                    GET_NEC_WPML_ARG_KLS(doc, placemarkElement, *curPlacemark, lineString);
                    break;
                }
                default :
                    curFolder->placemark = nullptr;
                    break;
            }
        }
        // Save file
        doc.SaveFile(kmlPath.c_str());
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::optional<wcs::WPMLDocument> parseWPML(const std::string& wpmlPath)
{
    wcs::WPMLDocument res;
    xml::XMLDocument doc;
    xml::XMLError eResult = doc.LoadFile(wpmlPath.c_str());
    if (eResult != xml::XML_SUCCESS)
    {
        std::cerr << "Failed to load wpml file: " << wpmlPath << std::endl;
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
    std::vector<wcs::WPMLFolder> folders{};
    xml::XMLElement *pFolder = pDocument->FirstChildElement("Folder");
    while (pFolder)
    {
        wcs::WPMLFolder tmpFolder;
        SET_OPT_WPML_ARG_I(tmpFolder, pFolder, templateId);
        SET_OPT_WPML_ARG_I(tmpFolder, pFolder, waylineId);
        SET_OPT_WPML_ARG_D(tmpFolder, pFolder, autoFlightSpeed);
        SET_OPT_WPML_ARG_E(tmpFolder, pFolder, executeHeightMode, ExecuteHeightMode);
        // Step 4: Setup Waypoint Placemark
        std::vector<wcs::WPMLPlacemark> waypointPlacemarks{};
        xml::XMLElement *pPlacemark = pFolder->FirstChildElement("Placemark");
        while (pPlacemark)
        {
            wcs::WPMLPlacemark tmpPlacemark;
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, isRisky);
            SET_OPT_WPML_ARG_KPT(tmpPlacemark, pPlacemark, point);
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, index);
            SET_OPT_WPML_ARG_D(tmpPlacemark, pPlacemark, executeHeight);
            SET_OPT_WPML_ARG_D(tmpPlacemark, pPlacemark, waypointSpeed);
            xml::XMLElement *pWaypointHeadingParam = pPlacemark->FirstChildElement("wpml:waypointHeadingParam");
            if (pWaypointHeadingParam != nullptr)
            {
                SET_OPT_WPML_ARG_E(
                    tmpPlacemark.waypointHeadingParam, pWaypointHeadingParam, waypointHeadingMode, WaypointHeadingMode);
                SET_OPT_WPML_ARG_D(tmpPlacemark.waypointHeadingParam, pWaypointHeadingParam, waypointHeadingAngle);
                SET_OPT_WPML_ARG_P(tmpPlacemark.waypointHeadingParam, pWaypointHeadingParam, waypointPoiPoint);
                SET_OPT_WPML_ARG_E(tmpPlacemark.waypointHeadingParam,
                                   pWaypointHeadingParam,
                                   waypointHeadingPathMode,
                                   WaypointHeadingPathMode);
            }
            xml::XMLElement *pWaypointTurnParam = pPlacemark->FirstChildElement("wpml:waypointTurnParam");
            if (pWaypointTurnParam != nullptr)
            {
                SET_OPT_WPML_ARG_E(
                    tmpPlacemark.waypointTurnParam, pWaypointTurnParam, waypointTurnMode, WaypointTurnMode);
                SET_OPT_WPML_ARG_D(tmpPlacemark.waypointTurnParam, pWaypointTurnParam, waypointTurnDampingDist);
            }
            SET_OPT_WPML_ARG_I(tmpPlacemark, pPlacemark, useStraightLine);
            xml::XMLElement *pActionGroup = pPlacemark->FirstChildElement("wpml:actionGroup");
            if (pActionGroup != nullptr)
            {
                wcs::ActionGroup tmpActionGroup;
                SET_OPT_WPML_ARG_I(tmpActionGroup, pActionGroup, actionGroupId);
                SET_OPT_WPML_ARG_I(tmpActionGroup, pActionGroup, actionGroupStartIndex);
                SET_OPT_WPML_ARG_I(tmpActionGroup, pActionGroup, actionGroupEndIndex);
                SET_OPT_WPML_ARG_E(tmpActionGroup, pActionGroup, actionGroupMode, ActionGroupMode);
                xml::XMLElement *pActionTrigger = pActionGroup->FirstChildElement("wpml:actionTrigger");
                if (pActionTrigger != nullptr)
                {
                    SET_OPT_WPML_ARG_E(
                        tmpActionGroup.actionTrigger, pActionTrigger, actionTriggerType, ActionTriggerType);
                    SET_OPT_WPML_ARG_D(tmpActionGroup.actionTrigger, pActionTrigger, actionTriggerParam);
                }
                xml::XMLElement *pAction = pActionGroup->FirstChildElement("wpml:action");
                if (pAction != nullptr)
                {
                    wcs::Action tmpAction;
                    SET_OPT_WPML_ARG_I(tmpAction, pAction, actionId);
                    SET_OPT_WPML_ARG_E(tmpAction, pAction, actionActuatorFunc, ActionActuatorFunc);
                    switch (tmpAction.actionActuatorFunc)
                    {
                        case wce::ActionActuatorFunc::takePhoto :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::TakePhotoParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::TakePhotoParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, fileSuffix);
                            SET_OPT_WPML_ARG_ES(*curAFP, pAction, payloadLensIndex, ImageFormat);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, useGlobalPayloadLensIndex);
                            break;
                        }
                        case wce::ActionActuatorFunc::startRecord :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::StartRecordParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::StartRecordParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, fileSuffix);
                            SET_OPT_WPML_ARG_ES(*curAFP, pAction, payloadLensIndex, ImageFormat);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, useGlobalPayloadLensIndex);
                            break;
                        }
                        case wce::ActionActuatorFunc::stopRecord :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::StopRecordParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::StopRecordParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_ES(*curAFP, pAction, payloadLensIndex, ImageFormat);
                            break;
                        }
                        case wce::ActionActuatorFunc::focus :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::FocusParam>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::FocusParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, isPointFocus);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, focusX);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, focusY);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, focusRegionWidth);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, focusRegionHeight);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, isInfiniteFocus);
                            break;
                        }
                        case wce::ActionActuatorFunc::zoom :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::ZoomParam>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::ZoomParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, focalLength);
                            break;
                        }
                        case wce::ActionActuatorFunc::customDirName :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::CustomDirNameParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::CustomDirNameParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, directoryName);
                            break;
                        }
                        case wce::ActionActuatorFunc::gimbalRotate :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::GimbalRotateParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::GimbalRotateParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_E(*curAFP, pAction, gimbalHeadingYawBase, GimbalHeadingYawBase);
                            SET_OPT_WPML_ARG_E(*curAFP, pAction, gimbalRotateMode, GimbalRotateMode);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, gimbalPitchRotateEnable);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalPitchRotateAngle);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, gimbalRollRotateEnable);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalRollRotateAngle);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, gimbalYawRotateEnable);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalYawRotateAngle);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, gimbalRotateTimeEnable);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalRotateTime);
                            break;
                        }
                        case wce::ActionActuatorFunc::rotateYaw :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::RotateYawParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::RotateYawParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, aircraftHeading);
                            SET_OPT_WPML_ARG_E(*curAFP, pAction, aircraftPathMode, AircraftPathMode);
                            break;
                        }
                        case wce::ActionActuatorFunc::hover :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::HoverParam>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::HoverParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, hoverTime);
                            break;
                        }
                        case wce::ActionActuatorFunc::gimbalEvenlyRotate :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::GimbalEvenlyRotateParam>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::GimbalEvenlyRotateParam>(
                                tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalPitchRotateAngle);
                            break;
                        }
                        case wce::ActionActuatorFunc::accurateShoot :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::AccurateShootParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::AccurateShootParam>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalPitchRotateAngle);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalYawRotateAngle);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusX);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusY);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusRegionWidth);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusRegionHeight);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, focalLength);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, aircraftHeading);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, accurateFrameValid);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_ES(*curAFP, pAction, payloadLensIndex, ImageFormat);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, useGlobalPayloadLensIndex);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, targetAngle);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, imageWidth);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, imageHeight);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, AFPos);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, gimbalPort);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, accurateCameraType);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, accurateFilePath);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, accurateFileMD5);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, accurateFileSize);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, accurateFileSuffix);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, accurateCameraApertue);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, accurateCameraLuminance);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, accurateCameraShutterTime);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, accurateCameraISO);
                            break;
                        }
                        case wce::ActionActuatorFunc::orientedShoot :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::OrientedShootParams>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::OrientedShootParams>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalPitchRotateAngle);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, gimbalYawRotateAngle);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusX);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusY);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusRegionWidth);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, focusRegionHeight);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, focalLength);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, aircraftHeading);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, accurateFrameValid);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_ES(*curAFP, pAction, payloadLensIndex, ImageFormat);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, useGlobalPayloadLensIndex);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, targetAngle);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, actionUUID);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, imageWidth);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, imageHeight);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, AFPos);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, gimbalPort);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, orientedCameraType);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, orientedFilePath);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, orientedFileMD5);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, orientedFileSize);
                            SET_OPT_WPML_ARG_S(*curAFP, pAction, orientedFileSuffix);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, orientedCameraApertue);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, orientedCameraLuminance);
                            SET_OPT_WPML_ARG_D(*curAFP, pAction, orientedCameraShutterTime);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, orientedCameraISO);
                            SET_OPT_WPML_ARG_E(*curAFP, pAction, orientedPhotoMode, OrientedPhotoMode);
                            break;
                        }
                        case wce::ActionActuatorFunc::panoShot :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::PanoShotParams>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::PanoShotParams>(tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_ES(*curAFP, pAction, payloadLensIndex, ImageFormat);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, useGlobalPayloadLensIndex);
                            SET_OPT_WPML_ARG_E(*curAFP, pAction, panoShotSubMode, PanoShotSubMode);
                            break;
                        }
                        case wce::ActionActuatorFunc::recordPointCloud :
                        {
                            tmpAction.actionActuatorFuncParam = std::make_shared<wcs::RecordPointCloudParams>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::RecordPointCloudParams>(
                                tmpAction.actionActuatorFuncParam);
                            SET_OPT_WPML_ARG_I(*curAFP, pAction, payloadPositionIndex);
                            SET_OPT_WPML_ARG_E(*curAFP, pAction, recordPointCloudOperate, RecordPointCloudOperate);
                            break;
                        }
                        default :
                            break;
                    }
                    tmpActionGroup.action = tmpAction;
                }
                tmpPlacemark.startActionGroup = tmpActionGroup;
            }
            waypointPlacemarks.emplace_back(tmpPlacemark);
            pPlacemark = pPlacemark->NextSiblingElement("Placemark");
        }
        tmpFolder.placemark = waypointPlacemarks;
        folders.emplace_back(tmpFolder);
        pFolder = pFolder->NextSiblingElement("Folder");
    }
    res.folder = folders;
    return res;
}

bool createWPML(const wcs::WPMLDocument& data, const std::string& wpmlPath)
{
    try
    {
        xml::XMLDocument doc;
        // Step 1: Create xml
        xml::XMLDeclaration *decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
        doc.InsertFirstChild(decl);
        // Step 2: Create kml Element
        xml::XMLElement *kmlElement = doc.NewElement("kml");
        kmlElement->SetAttribute("xmlns", "http://www.opengis.net/kml/2.2");
        kmlElement->SetAttribute("xmlns:wpml", "http://www.dji.com/wpmz/1.0.6");
        doc.InsertEndChild(kmlElement);
        // Step 3: Create Document Element
        xml::XMLElement *documentElement = doc.NewElement("Document");
        kmlElement->InsertEndChild(documentElement);
        // Step 4: Create missionConfig Element
        xml::XMLElement *missionElement = doc.NewElement("wpml:missionConfig");
        documentElement->InsertEndChild(missionElement);
        GET_NEC_WPML_ARG_E(doc, missionElement, data, missionConfig.flyToWaylineMode);
        GET_NEC_WPML_ARG_E(doc, missionElement, data, missionConfig.finishAction);
        GET_NEC_WPML_ARG_E(doc, missionElement, data, missionConfig.exitOnRCLost);
        GET_OPT_WPML_ARG_E(doc, missionElement, data, missionConfig.executeRCLostAction);
        GET_NEC_WPML_ARG_N(doc, missionElement, data, missionConfig.takeOffSecurityHeight);
        GET_NEC_WPML_ARG_N(doc, missionElement, data, missionConfig.globalTransitionalSpeed);
        GET_NEC_WPML_ARG_N(doc, missionElement, data, missionConfig.globalRTHHeight);
        xml::XMLElement *droneInfoElement = doc.NewElement("wpml:droneInfo");
        missionElement->InsertEndChild(droneInfoElement);
        GET_NEC_WPML_ARG_N(doc, droneInfoElement, data, missionConfig.droneInfo.droneEnumValue);
        GET_OPT_WPML_ARG_N(doc, droneInfoElement, data, missionConfig.droneInfo.droneSubEnumValue);
        xml::XMLElement *payloadInfoElement = doc.NewElement("wpml:payloadInfo");
        missionElement->InsertEndChild(payloadInfoElement);
        GET_NEC_WPML_ARG_N(doc, payloadInfoElement, data, missionConfig.payloadInfo.payloadEnumValue);
        GET_NEC_WPML_ARG_N(doc, payloadInfoElement, data, missionConfig.payloadInfo.payloadPositionIndex);
        xml::XMLElement *autoRerouteInfoElement = doc.NewElement("wpml:autoRerouteInfo");
        missionElement->InsertEndChild(autoRerouteInfoElement);
        GET_NEC_WPML_ARG_N(doc, autoRerouteInfoElement, data, missionConfig.autoRerouteInfo.missionAutoRerouteMode);
        GET_NEC_WPML_ARG_N(
            doc, autoRerouteInfoElement, data, missionConfig.autoRerouteInfo.transitionalAutoRerouteMode);
        // Step 5: Create Folder Element
        for (const auto& fd : data.folder)
        {
            xml::XMLElement *folderElement = doc.NewElement("Folder");
            documentElement->InsertEndChild(folderElement);
            GET_NEC_WPML_ARG_N(doc, folderElement, fd, templateId);
            GET_NEC_WPML_ARG_N(doc, folderElement, fd, waylineId);
            GET_NEC_WPML_ARG_N(doc, folderElement, fd, autoFlightSpeed);
            GET_NEC_WPML_ARG_E(doc, folderElement, fd, executeHeightMode);
            // Step 5: Create Placemark Element
            for (const auto& wpm : fd.placemark)
            {
                xml::XMLElement *placemarkElement = doc.NewElement("Placemark");
                folderElement->InsertEndChild(placemarkElement);
                GET_OPT_WPML_ARG_N(doc, placemarkElement, wpm, isRisky);
                GET_NEC_WPML_ARG_KPT(doc, placemarkElement, wpm, point);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, wpm, index);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, wpm, executeHeight);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, wpm, waypointSpeed);
                GET_NEC_WPML_ARG_N(doc, placemarkElement, wpm, useStraightLine);
                xml::XMLElement *waypointHeadingParamElement = doc.NewElement("wpml:waypointHeadingParam");
                placemarkElement->InsertEndChild(waypointHeadingParamElement);
                GET_NEC_WPML_ARG_E(doc, placemarkElement, wpm, waypointHeadingParam.waypointHeadingMode);
                GET_OPT_WPML_ARG_N(doc, placemarkElement, wpm, waypointHeadingParam.waypointHeadingAngle);
                GET_OPT_WPML_ARG_P(doc, placemarkElement, wpm, waypointHeadingParam.waypointPoiPoint);
                GET_NEC_WPML_ARG_E(doc, placemarkElement, wpm, waypointHeadingParam.waypointHeadingPathMode);
                xml::XMLElement *waypointTurnParamElement = doc.NewElement("wpml:waypointTurnParam");
                placemarkElement->InsertEndChild(waypointTurnParamElement);
                GET_NEC_WPML_ARG_E(doc, waypointTurnParamElement, wpm, waypointTurnParam.waypointTurnMode);
                GET_OPT_WPML_ARG_N(doc, waypointTurnParamElement, wpm, waypointTurnParam.waypointTurnDampingDist);
                if (wpm.startActionGroup.has_value())
                {
                    auto tmpAG = wpm.startActionGroup.value();
                    xml::XMLElement *actionGroupElement = doc.NewElement("wpml:actionGroup");
                    placemarkElement->InsertEndChild(actionGroupElement);
                    GET_NEC_WPML_ARG_N(doc, actionGroupElement, tmpAG, actionGroupId);
                    GET_NEC_WPML_ARG_N(doc, actionGroupElement, tmpAG, actionGroupStartIndex);
                    GET_NEC_WPML_ARG_N(doc, actionGroupElement, tmpAG, actionGroupEndIndex);
                    GET_NEC_WPML_ARG_E(doc, actionGroupElement, tmpAG, actionGroupMode);
                    xml::XMLElement *actionTriggerElement = doc.NewElement("wpml:actionTrigger");
                    actionGroupElement->InsertEndChild(actionTriggerElement);
                    GET_NEC_WPML_ARG_E(doc, actionTriggerElement, tmpAG, actionTrigger.actionTriggerType);
                    GET_OPT_WPML_ARG_N(doc, actionTriggerElement, tmpAG, actionTrigger.actionTriggerParam);
                    xml::XMLElement *actionElement = doc.NewElement("wpml:action");
                    actionGroupElement->InsertEndChild(actionElement);
                    GET_NEC_WPML_ARG_N(doc, actionElement, tmpAG, action.actionId);
                    GET_NEC_WPML_ARG_E(doc, actionElement, tmpAG, action.actionActuatorFunc);
                    switch (tmpAG.action.actionActuatorFunc)
                    {
                        case wce::ActionActuatorFunc::takePhoto :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::TakePhotoParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::TakePhotoParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, fileSuffix);
                            GET_NEC_WPML_ARG_ES(doc, actionElement, *curAFP, payloadLensIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, useGlobalPayloadLensIndex);
                            break;
                        }
                        case wce::ActionActuatorFunc::startRecord :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::StartRecordParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::StartRecordParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, fileSuffix);
                            GET_NEC_WPML_ARG_ES(doc, actionElement, *curAFP, payloadLensIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, useGlobalPayloadLensIndex);
                            break;
                        }
                        case wce::ActionActuatorFunc::stopRecord :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::StopRecordParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::StopRecordParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_ES(doc, actionElement, *curAFP, payloadLensIndex);
                            break;
                        }
                        case wce::ActionActuatorFunc::focus :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::FocusParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::FocusParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, isPointFocus);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusX);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusY);
                            GET_OPT_WPML_ARG_N(doc, actionElement, *curAFP, focusRegionWidth);
                            GET_OPT_WPML_ARG_N(doc, actionElement, *curAFP, focusRegionHeight);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, isInfiniteFocus);
                            break;
                        }
                        case wce::ActionActuatorFunc::zoom :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::ZoomParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::ZoomParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focalLength);
                            break;
                        }
                        case wce::ActionActuatorFunc::customDirName :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::CustomDirNameParam>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::CustomDirNameParam>(
                                tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, directoryName);
                            break;
                        }
                        case wce::ActionActuatorFunc::gimbalRotate :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::GimbalRotateParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::GimbalRotateParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_E(doc, actionElement, *curAFP, gimbalHeadingYawBase);
                            GET_NEC_WPML_ARG_E(doc, actionElement, *curAFP, gimbalRotateMode);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalPitchRotateEnable);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalPitchRotateAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalRollRotateEnable);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalRollRotateAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalYawRotateEnable);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalYawRotateAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalRotateTimeEnable);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalRotateTime);
                            break;
                        }
                        case wce::ActionActuatorFunc::rotateYaw :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::RotateYawParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::RotateYawParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, aircraftHeading);
                            GET_NEC_WPML_ARG_E(doc, actionElement, *curAFP, aircraftPathMode);
                            break;
                        }
                        case wce::ActionActuatorFunc::hover :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::HoverParam>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::HoverParam>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, hoverTime);
                            break;
                        }
                        case wce::ActionActuatorFunc::gimbalEvenlyRotate :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::GimbalEvenlyRotateParam>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::GimbalEvenlyRotateParam>(
                                tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalPitchRotateAngle);
                            break;
                        }
                        case wce::ActionActuatorFunc::accurateShoot :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::AccurateShootParam>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::AccurateShootParam>(
                                tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalPitchRotateAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalYawRotateAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusX);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusY);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusRegionWidth);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusRegionHeight);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focalLength);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, aircraftHeading);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateFrameValid);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_ES(doc, actionElement, *curAFP, payloadLensIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, useGlobalPayloadLensIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, targetAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, imageWidth);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, imageHeight);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, AFPos);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalPort);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateCameraType);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, accurateFilePath);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, accurateFileMD5);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateFileSize);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, accurateFileSuffix);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateCameraApertue);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateCameraLuminance);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateCameraShutterTime);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateCameraISO);
                            break;
                        }
                        case wce::ActionActuatorFunc::orientedShoot :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::OrientedShootParams>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::OrientedShootParams>(
                                tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalPitchRotateAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalYawRotateAngle);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusX);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusY);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusRegionWidth);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focusRegionHeight);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, focalLength);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, aircraftHeading);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, accurateFrameValid);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_ES(doc, actionElement, *curAFP, payloadLensIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, useGlobalPayloadLensIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, targetAngle);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, actionUUID);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, imageWidth);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, imageHeight);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, AFPos);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, gimbalPort);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, orientedCameraType);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, orientedFilePath);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, orientedFileMD5);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, orientedFileSize);
                            GET_NEC_WPML_ARG_S(doc, actionElement, *curAFP, orientedFileSuffix);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, orientedCameraApertue);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, orientedCameraLuminance);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, orientedCameraShutterTime);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, orientedCameraISO);
                            GET_NEC_WPML_ARG_E(doc, actionElement, *curAFP, orientedPhotoMode);
                            break;
                        }
                        case wce::ActionActuatorFunc::panoShot :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::PanoShotParams>();
                            auto curAFP =
                                std::dynamic_pointer_cast<wcs::PanoShotParams>(tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_ES(doc, actionElement, *curAFP, payloadLensIndex);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, useGlobalPayloadLensIndex);
                            GET_NEC_WPML_ARG_E(doc, actionElement, *curAFP, panoShotSubMode);
                            break;
                        }
                        case wce::ActionActuatorFunc::recordPointCloud :
                        {
                            tmpAG.action.actionActuatorFuncParam = std::make_shared<wcs::RecordPointCloudParams>();
                            auto curAFP = std::dynamic_pointer_cast<wcs::RecordPointCloudParams>(
                                tmpAG.action.actionActuatorFuncParam);
                            GET_NEC_WPML_ARG_N(doc, actionElement, *curAFP, payloadPositionIndex);
                            GET_NEC_WPML_ARG_E(doc, actionElement, *curAFP, recordPointCloudOperate);
                            break;
                        }
                        default :
                            break;
                    }
                }
            }
        }
        // Save file
        doc.SaveFile(wpmlPath.c_str());
        return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

std::optional<wcs::WPMZData> parseWPMZ(const std::string& kmzPath)
{
    std::string outputDir = wcu::getTempDir() + "/" + wcu::getFileName(kmzPath) + wcu::getNowTimestamp();
    wcu::removeFileOrDir(outputDir);
    if (!wcu::makeDir(outputDir))
    {
        return std::nullopt;
    }
    bool isExtract = wcu::extractKMZ(kmzPath, outputDir);
    if (!isExtract)
    {
        wcu::removeFileOrDir(outputDir);
        return std::nullopt;
    }
    wcs::WPMZData res;
    std::vector<std::string> files = wcu::findFiles(outputDir);
    for (const auto& f : files)
    {
        if (wcu::endWith(f, ".kml"))
        {
            auto resKml = wcc::parseKML(f);
            if (!resKml.has_value())
            {
                wcu::removeFileOrDir(outputDir);
                return std::nullopt;
            }
            res.templateKML = std::move(resKml.value());
        }
        else if (wcu::endWith(f, ".wpml"))
        {
            auto resWpml = wcc::parseWPML(f);
            if (!resWpml.has_value())
            {
                wcu::removeFileOrDir(outputDir);
                return std::nullopt;
            }
            res.waylinesWPML = std::move(resWpml.value());
        }
        else if (wcu::endWith(f, "res"))
        {
            res.resDir = f;
        }
    }
    wcu::removeFileOrDir(outputDir);
    return res;
}

bool createWPMZ(const wcs::WPMZData& data, const std::string& kmzPath)
{
    std::string packageDir = wcu::getTempDir() + "/" + wcu::getFileName(kmzPath);
    std::string outputDir = packageDir + "/wpmz";
    wcu::removeFileOrDir(packageDir);
    if (!wcu::makeDir(outputDir))
    {
        return false;
    }
    bool succ = wcc::createKML(data.templateKML, outputDir + "/template.kml") &&
                wcc::createWPML(data.waylinesWPML, outputDir + "/waylines.wpml");
    if (data.resDir.has_value())
    {
        wcu::copyFileOrDir(data.resDir.value(), outputDir);
    }
    if (!succ)
    {
        return false;
    }
    succ = wcu::packageKMZ(packageDir, kmzPath);
    wcu::removeFileOrDir(packageDir);
    return succ;
}
} // namespace wpml_codec::core
