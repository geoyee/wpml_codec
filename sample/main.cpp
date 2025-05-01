#include <wpml_codec.hpp>
#include <iostream>
#include <string>

int main()
{
    WPML_CODEC_VERSION();

    std::string kmzPath = "./data/二维面状航线.kmz";
    std::string saveKmzPath = "./data/二维面状航线导出.kmz";

    auto wpmzData = wcc::parseWPMZ(kmzPath);
    if (!wpmzData.has_value())
    {
        std::cerr << "Can not open " << kmzPath << " to parsed\n";
        return -1;
    }

    bool saveSucc = wcc::createWPMZ(wpmzData.value(), saveKmzPath);
    if (!saveSucc)
    {
        std::cerr << "Can not create " << saveKmzPath << " from wpmzData\n";
        return -1;
    }

    std::cout << "Finished\n";
    return 0;
}
