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
        SET_OPT_WPML_ARG_E(*curFolder, pFolder, globalWaypointTurnMode, GlobalWaypointTurnMode);
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
        // TODO: Implement
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

bool creatKML(const wcs::KMLDocument& data, const std::string& kmlPath)
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
        xml::XMLElement *missionElement = doc.NewElement("wpml::missionConfig");
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
        xml::XMLElement *droneInfoElement = doc.NewElement("wpml::droneInfo");
        missionElement->InsertEndChild(droneInfoElement);
        GET_NEC_WPML_ARG_N(doc, droneInfoElement, data, missionConfig.droneInfo.droneEnumValue);
        GET_OPT_WPML_ARG_N(doc, droneInfoElement, data, missionConfig.droneInfo.droneSubEnumValue);
        xml::XMLElement *payloadInfoElement = doc.NewElement("wpml::payloadInfo");
        missionElement->InsertEndChild(payloadInfoElement);
        GET_NEC_WPML_ARG_N(doc, payloadInfoElement, data, missionConfig.payloadInfo.payloadEnumValue);
        GET_NEC_WPML_ARG_N(doc, payloadInfoElement, data, missionConfig.payloadInfo.payloadPositionIndex);
        xml::XMLElement *autoRerouteInfoElement = doc.NewElement("wpml::autoRerouteInfo");
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
            xml::XMLElement *waylineCoordinateSysParamElement = doc.NewElement("wpml::waylineCoordinateSysParam");
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
            xml::XMLElement *payloadParamElement = doc.NewElement("wpml::payloadParam");
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
            xml::XMLElement *globalWaypointHeadingParamElement = doc.NewElement("wpml::globalWaypointHeadingParam");
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
                xml::XMLElement *waypointHeadingParamElement = doc.NewElement("wpml::waypointHeadingParam");
                placemarkElement->InsertEndChild(waypointHeadingParamElement);
                GET_NEC_WPML_ARG_E(
                    doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointHeadingMode);
                GET_OPT_WPML_ARG_N(
                    doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointHeadingAngle);
                GET_OPT_WPML_ARG_P(doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointPoiPoint);
                GET_NEC_WPML_ARG_E(
                    doc, waypointHeadingParamElement, pointPlace.waypointHeadingParam, waypointHeadingPathMode);
                xml::XMLElement *waypointTurnParamElement = doc.NewElement("wpml::waypointTurnParam");
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
            xml::XMLElement *waylineCoordinateSysParamElement = doc.NewElement("wpml::waylineCoordinateSysParam");
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
            xml::XMLElement *payloadParamElement = doc.NewElement("wpml::payloadParam");
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
                    xml::XMLElement *overlapElement = doc.NewElement("wpml::overlap");
                    placemarkElement->InsertEndChild(overlapElement);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoLidarOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoLidarOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoCameraOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, orthoCameraOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedLidarOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedLidarOverlapW);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedCameraOverlapH);
                    GET_OPT_WPML_ARG_N(doc, overlapElement, curPlacemark->overlap, inclinedCameraOverlapW);
                    xml::XMLElement *mappingHeadingParamElement = doc.NewElement("wpml::mappingHeadingParam");
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
                    xml::XMLElement *overlapElement = doc.NewElement("wpml::overlap");
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
} // namespace wpml_codec::core
