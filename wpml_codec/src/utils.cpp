#include <wpml_codec/utils.h>
#include <unzip.h>
#include <iostream>

namespace wpml_codec::utils
{
bool extractKMZ(const std::string& kmzPath, const std::string& outputDir)
{
    unzFile zip = unzOpen(kmzPath.c_str());
    if (zip == nullptr)
    {
        std::cerr << "Cannot open zip file" << std::endl;
        return false;
    }
    unz_global_info gi;
    if (unzGetGlobalInfo(zip, &gi) != UNZ_OK)
    {
        std::cerr << "Cannot read file global info" << std::endl;
        unzClose(zip);
        return false;
    }
    for (uLong i = 0; i < gi.number_entry; ++i)
    {
        if (unzGoToFirstFile(zip) == UNZ_OK)
        {
            for (int j = 0; j < i; ++j)
            {
                if (unzGoToNextFile(zip) != UNZ_OK)
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
        std::string fileName(256, '\0');
        unz_file_info fileInfo;
        if (unzGetCurrentFileInfo(zip, &fileInfo, fileName.data(), fileName.length(), nullptr, 0, nullptr, 0) == UNZ_OK)
        {
            if (unzOpenCurrentFile(zip) == UNZ_OK)
            {
                std::string outputPath = outputDir + "/" + fileName;
                FILE *out = fopen(outputPath.c_str(), "wb");
                if (out == nullptr)
                {
                    std::cerr << "Cannot create output file" << std::endl;
                    unzCloseCurrentFile(zip);
                    continue;
                }
                std::string buffer(8192, '\0');
                int read_bytes;
                while ((read_bytes = unzReadCurrentFile(zip, buffer.data(), buffer.length())) > 0)
                {
                    fwrite(buffer.data(), read_bytes, 1, out);
                }
                fclose(out);
                unzCloseCurrentFile(zip);
            }
            else
            {
                std::cerr << "Cannot open current file in zip" << std::endl;
            }
        }
        else
        {
            std::cerr << "Cannot get current file info" << std::endl;
        }
    }
    unzClose(zip);
    return true;
}
} // namespace wpml_codec::utils
