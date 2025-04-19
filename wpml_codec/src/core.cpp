#include <wpml_codec/core.h>
#include <wpml_codec/utils.h>
#include <wpml_codec/macro.h>
#include <tinyxml2.h>

namespace xml = tinyxml2;

namespace wpml_codec::core
{
std::optional<Document> parseKML(const std::string& kmlPath)
{
    Document res;
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

    // TODO: Implement
    return res;
}
} // namespace wpml_codec::core
