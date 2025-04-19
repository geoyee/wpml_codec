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
        std::cerr << "isExtract is false\n";
        return -1;
    }
    std::cout << "isExtract is true\n";

    std::vector<std::string> files = wcu::findFiles(outputDir);
    for (const auto& f : files)
    {
        if (wcu::endWith(f, "kml"))
        {
            auto res = wcc::parseKML(f);
            std::cout << res.value().author.value() << "\n";
        }
    }

    bool isPackage = wcu::packageKMZ(outputDir, saveKmzPath);
    if (!isPackage)
    {
        std::cerr << "isPackage is false\n";
        return -1;
    }
    std::cout << "isPackage is true\n";

    return 0;
}
