#include <wpml_codec/utils.h>
#include <unzip.h>
#include <zip.h>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace wpml_codec::utils
{
bool extractKMZ(const std::string& kmzPath, const std::string& outputDir)
{
    unzFile zipFile = unzOpen(kmzPath.c_str());
    if (!zipFile)
    {
        return false;
    }
    if (unzGoToFirstFile(zipFile) != UNZ_OK)
    {
        unzClose(zipFile);
        return false;
    }
    do
    {
        constexpr int FILE_NAME_LENGTH = 256;
        char fileNameInZip[FILE_NAME_LENGTH];
        unz_file_info fileInfo;
        if (unzGetCurrentFileInfo(zipFile, &fileInfo, fileNameInZip, FILE_NAME_LENGTH, nullptr, 0, nullptr, 0) !=
            UNZ_OK)
        {
            unzClose(zipFile);
            return false;
        }
        std::string entryName(fileNameInZip);
        if ((entryName.size() > 4 && entryName.substr(entryName.size() - 4) == ".kml") ||
            (entryName.size() > 5 && entryName.substr(entryName.size() - 5) == ".wpml"))
        {
            fs::path outputPath = fs::path(outputDir) / entryName;
            fs::create_directories(outputPath.parent_path());
            FILE *outFile = fopen(outputPath.string().c_str(), "wb");
            if (!outFile)
            {
                unzClose(zipFile);
                return false;
            }
            if (unzOpenCurrentFile(zipFile) != UNZ_OK)
            {
                fclose(outFile);
                unzClose(zipFile);
                return false;
            }
            char buffer[4096];
            int bytesRead;
            bool error = false;
            while (bytesRead = unzReadCurrentFile(zipFile, buffer, sizeof(buffer)))
            {
                if (bytesRead < 0)
                {
                    error = true;
                    break;
                }
                if (fwrite(buffer, 1, bytesRead, outFile) != bytesRead)
                {
                    error = true;
                    break;
                }
            }
            fclose(outFile);
            unzCloseCurrentFile(zipFile);
            if (error)
            {
                fs::remove(outputPath);
                unzClose(zipFile);
                return false;
            }
        }
    } while (unzGoToNextFile(zipFile) == UNZ_OK);
    unzClose(zipFile);
    return true;
}

bool packageKMZ(const std::string& inputDir, const std::string& kmzPath)
{
    zipFile zipFile = zipOpen(kmzPath.c_str(), APPEND_STATUS_CREATE);
    if (!zipFile)
    {
        return false;
    }
    try
    {
        for (const auto& entry : fs::recursive_directory_iterator(inputDir))
        {
            if (entry.is_regular_file())
            {
                fs::path relativePath = fs::relative(entry.path(), inputDir);
                std::string zipEntryName = relativePath.generic_string();
                FILE *inFile = fopen(entry.path().string().c_str(), "rb");
                if (!inFile)
                {
                    zipClose(zipFile, nullptr);
                    return false;
                }
                fseek(inFile, 0, SEEK_END);
                long fileSize = ftell(inFile);
                fseek(inFile, 0, SEEK_SET);
                zip_fileinfo fileInfo = {};
                if (zipOpenNewFileInZip(zipFile,
                                        zipEntryName.c_str(),
                                        &fileInfo,
                                        nullptr,
                                        0,
                                        nullptr,
                                        0,
                                        nullptr,
                                        Z_DEFLATED,
                                        Z_DEFAULT_COMPRESSION) != ZIP_OK)
                {
                    fclose(inFile);
                    zipClose(zipFile, nullptr);
                    return false;
                }
                char buffer[4096];
                size_t bytesRead;
                bool error = false;
                while (bytesRead = fread(buffer, 1, sizeof(buffer), inFile))
                {
                    if (zipWriteInFileInZip(zipFile, buffer, bytesRead) != ZIP_OK)
                    {
                        error = true;
                        break;
                    }
                }
                fclose(inFile);
                zipCloseFileInZip(zipFile);

                if (error)
                {
                    zipClose(zipFile, nullptr);
                    return false;
                }
            }
        }
    }
    catch (const std::exception&)
    {
        zipClose(zipFile, nullptr);
        return false;
    }
    if (zipClose(zipFile, nullptr) != ZIP_OK)
    {
        return false;
    }
    return true;
}
} // namespace wpml_codec::utils
