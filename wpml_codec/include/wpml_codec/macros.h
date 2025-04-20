#pragma once

#ifndef WPML_CODEC_MACROS_H
#define WPML_CODEC_MACROS_H

#define SET_OPT_WPML_ARG_B(doc, xml, name)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto boolVal = utils::toBool(pEle->GetText());                                                             \
            if (boolVal.has_value())                                                                                   \
            {                                                                                                          \
                doc.name = boolVal.value();                                                                            \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SET_OPT_WPML_ARG_D(doc, xml, name)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto doubleVal = utils::toDouble(pEle->GetText());                                                         \
            if (doubleVal.has_value())                                                                                 \
            {                                                                                                          \
                doc.name = doubleVal.value();                                                                          \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SET_OPT_WPML_ARG_E(doc, xml, name, enumType)                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto enumVal = magic_enum::enum_cast<enumType>(pEle->GetText());                                           \
            if (enumVal.has_value())                                                                                   \
            {                                                                                                          \
                doc.name = enumVal.value();                                                                            \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SET_OPT_WPML_ARG_ES(doc, xml, name, enumType)                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto enumsStr = pEle->GetText();                                                                           \
            if (enumsStr != nullptr)                                                                                   \
            {                                                                                                          \
                auto enumStrVec = utils::split(enumsStr, ",");                                                         \
                std::vector<enumType> enumVec(enumStrVec.size());                                                      \
                size_t i = 0;                                                                                          \
                for (const auto& enumStr : enumStrVec)                                                                 \
                {                                                                                                      \
                    auto enumVal = magic_enum::enum_cast<enumType>(enumStr);                                           \
                    if (enumVal.has_value())                                                                           \
                    {                                                                                                  \
                        enumVec[i++] = enumVal.value();                                                                \
                    }                                                                                                  \
                }                                                                                                      \
                doc.name = enumVec;                                                                                    \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SET_OPT_WPML_ARG_I(doc, xml, name)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto intVal = utils::toInt(pEle->GetText());                                                               \
            if (intVal.has_value())                                                                                    \
            {                                                                                                          \
                doc.name = intVal.value();                                                                             \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SET_OPT_WPML_ARG_L(doc, xml, name)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto longVal = utils::toLongLong(pEle->GetText());                                                         \
            if (longVal.has_value())                                                                                   \
            {                                                                                                          \
                doc.name = longVal.value();                                                                            \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SET_OPT_WPML_ARG_P(doc, xml, name)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto reals = utils::toDoubles(pEle->GetText());                                                            \
            if (reals.size() == 3)                                                                                     \
            {                                                                                                          \
                Point p{reals[0], reals[1], reals[2]};                                                                 \
                doc.name = p;                                                                                          \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SET_OPT_WPML_ARG_S(doc, xml, name)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        auto pEle = xml->FirstChildElement("wpml:" #name);                                                             \
        if (pEle != nullptr)                                                                                           \
        {                                                                                                              \
            auto strVal = pEle->GetText();                                                                             \
            if (strVal != nullptr)                                                                                     \
            {                                                                                                          \
                doc.name = strVal;                                                                                     \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#endif // WPML_CODEC_MACROS_H
