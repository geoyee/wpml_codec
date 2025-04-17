#include <wpml_codec.hpp>
#include <iostream>
#include <string>

namespace wcv = wpml_codec::version;
namespace wcu = wpml_codec::utils;

int main()
{
    std::cout << "BRANCH_NAME: " << wcv::BRANCH_NAME << "\n"
              << "COMMIT_HASH: " << wcv::COMMIT_HASH << "\n"
              << "COMMIT_TIME: " << wcv::COMMIT_TIME << "\n"
              << "BUILD_CXX_COMPILER: " << wcv::BUILD_CXX_COMPILER << "\n"
              << "BUILD_C_COMPILER: " << wcv::BUILD_C_COMPILER << "\n"
              << "BUILD_ENV: " << wcv::BUILD_ENV << "\n"
              << "BUILD_TIME: " << wcv::BUILD_TIME << "\n"
              << "RELEASE_VERSION: " << wcv::RELEASE_VERSION << "\n"
              << std::endl;

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

    bool isPackage = wcu::packageKMZ(outputDir, saveKmzPath);
    if (!isPackage)
    {
        std::cerr << "isPackage is false\n";
        return -1;
    }
    std::cout << "isPackage is true\n";

    return 0;
}
