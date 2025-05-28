#include <wpml_codec.hpp>
#include <iostream>
#include <string>

int main()
{
    WPML_CODEC_VERSION();

    std::vector<std::string> fileNames{"mapping3d", "waypoint"};

    for (const auto& fileName : fileNames)
    {
        std::string kmzPath = "./data/" + fileName + ".kmz";
        std::string saveKmzPath = wcu::replace(kmzPath, ".kmz", "-update.kmz");

        auto wpmzData = wcc::parseWPMZ(kmzPath);
        if (!wpmzData.has_value())
        {
            std::cerr << "Can not open " << kmzPath << " to parsed\n";
            return -1;
        }
        std::cout << "Parsed file " << kmzPath << " to wpmzData successfully\n";

        wpmzData.value().templateKML.updateTime = wcu::toLongLong(wcu::getNowTimestamp());
        wpmzData.value().templateKML.missionConfig.takeOffSecurityHeight = 522;
        std::cout << "Update wpmzData successfully\n";

        bool saveSucc = wcc::createWPMZ(wpmzData.value(), saveKmzPath);
        if (!saveSucc)
        {
            std::cerr << "Can not create " << saveKmzPath << " from wpmzData\n";
            return -1;
        }
        std::cout << "Saved new file " << saveKmzPath << " successfully\n";
    }

    std::cout << "Finished" << std::endl;
    return 0;
}
