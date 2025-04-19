#include <wpml_codec/core.h>
#include <wpml_codec/utils.h>
#include <tinyxml2.h>

namespace xml = tinyxml2;

namespace wpml_codec::core
{
std::optional<Document> parseKML(const std::string& kmlPath)
{
#define GET_WPML_PARAMS_S(doc, xml, name)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        auto tmp = xml->FirstChildElement("wpml:" #name);                                                              \
        if (tmp != nullptr)                                                                                            \
        {                                                                                                              \
            doc.name = tmp->GetText();                                                                                 \
        }                                                                                                              \
    } while (0)
#define GET_WPML_PARAMS_I(doc, xml, name)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        auto tmp = xml->FirstChildElement("wpml:" #name);                                                              \
        if (tmp != nullptr)                                                                                            \
        {                                                                                                              \
            doc.name = utils::toInt(tmp->GetText());                                                                   \
        }                                                                                                              \
    } while (0)
#define GET_WPML_PARAMS_D(doc, xml, name)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        auto tmp = xml->FirstChildElement("wpml:" #name);                                                              \
        if (tmp != nullptr)                                                                                            \
        {                                                                                                              \
            doc.name = utils::toDouble(tmp->GetText());                                                                \
        }                                                                                                              \
    } while (0)

    Document res;
    xml::XMLDocument doc;
    xml::XMLError eResult = doc.LoadFile(kmlPath.c_str());
    if (eResult != xml::XML_SUCCESS)
    {
        std::cerr << "Failed to load kml file: " << kmlPath << std::endl;
        return std::nullopt;
    }
    xml::XMLElement *pDocument = doc.FirstChildElement("kml")->FirstChildElement("Document");
    if (pDocument == nullptr)
    {
        std::cerr << "No kml element found" << std::endl;
        return std::nullopt;
    }
    // Parse KML
    GET_WPML_PARAMS_S(res, pDocument, author);
    GET_WPML_PARAMS_I(res, pDocument, createTime);
    GET_WPML_PARAMS_I(res, pDocument, updateTime);
    // TODO: Implement
    return res;
}
} // namespace wpml_codec::core
