#include <wpml_codec.hpp>
#include <iostream>
#include <string>

int main()
{
    WPML_CODEC_VERSION();

    std::string kmzPath = "./data/二维面状航线.kmz";
    std::string outputDir = "./data/output";
    std::string saveKmzPath = "./data/二维面状航线_2.kmz";

    bool isExtract = wcu::extractKMZ(kmzPath, outputDir);
    if (!isExtract)
    {
        return -1;
    }
    std::cout << "isExtract is ok\n";

    std::vector<std::string> files = wcu::findFiles(outputDir);
    for (const auto& f : files)
    {
        if (wcu::endWith(f, "kml"))
        {
            auto res = wcc::parseKML(f);
            if (res.has_value())
            {
                std::cout << "parseKML is ok\n";
                if (wcc::creatKML(res.value(), outputDir + "/tmp.kml"))
                {
                    std::cout << "creatKML is ok\n";
                }
            }
        }
    }

    bool isPackage = wcu::packageKMZ(outputDir + "/wpmz", saveKmzPath);
    if (!isPackage)
    {
        return -1;
    }
    std::cout << "isPackage is ok\n";

    return 0;
}
